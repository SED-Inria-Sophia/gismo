# GISMO Build System Audit & Modernization Implementation Plan

## Executive Summary

This document provides a comprehensive audit of the GISMO build system and outlines an implementation plan to transform the architecture from the current monolithic structure with circular dependencies to a clean, modern CMake-based modular architecture following the Generic C++ Build Pattern.

### Critical Issues Identified

1. **Circular Dependencies**: 6+ circular dependency chains involving `gsCore`
2. **Unclear Optional Dependencies**: Optional modules in `optional/` directory have poorly defined external dependencies
3. **Monolithic Build**: Old-style object library pattern prevents proper modular compilation
4. **Mixed Architecture**: Coexistence of old (`src/gs*`) and new (`src/gismo/*`) module structures
5. **Naming Inconsistency**: Mix of `gs`-prefixed and clean PascalCase names
6. **Build System Complexity**: Over-complicated CMake configuration with unclear dependency chains

### Goals

- **Zero circular dependencies** through proper architectural layering
- **Clear external dependency management** for optional modules
- **Modern CMake patterns** (3.20+) with target-based design
- **Clean naming conventions** following Generic C++ Build Pattern
- **Validation use case** to ensure architecture works end-to-end

---

## Part I: Current State Audit

### 1.1 Directory Structure Analysis

#### Old Architecture (src/gs*)

```
src/
├── gsCore/           # 83 files - Foundation layer with circular deps
├── gsMatrix/         # 19 files - Matrix operations (circular with gsCore)
├── gsUtils/          # 20 files - Utilities (circular with gsCore)
├── gsAssembler/      # 67 files - Assembly operations (circular with gsCore)
├── gsPde/            # 17 files - PDE definitions (circular with gsCore)
├── gsIO/             # 39 files - I/O operations (circular with gsCore)
├── gsNurbs/          # 40 files - NURBS geometry
├── gsHSplines/       # 29 files - Hierarchical splines
├── gsMSplines/       # 15 files - Multi-patch splines
├── gsSolver/         # 45 files - Linear/nonlinear solvers
├── gsModeling/       # 62 files - Geometric modeling
├── gsOptimizer/      # 9 files - Optimization algorithms
├── gsMultiGrid/      # 8 files - Multigrid methods
├── gsIeti/           # 14 files - IETI domain decomposition
├── gsTensor/         # 9 files - Tensor operations
├── gsParallel/       # 9 files - Parallel computing
├── gsDomain/         # 22 files - Domain definitions
└── gsMesh2/          # 12 files - Mesh operations
```

**Issues**:
- All modules built as `OBJECT` libraries (old CMake pattern)
- No proper target-based dependency management
- Circular includes prevent modular compilation
- Single global include directory (`src/`)

#### New Architecture (src/gismo/)

```
src/gismo/
└── Module1/           # ✅ Modern module (INTERFACE library)
│   ├── CMakeLists.txt
│   └── Component1.h   # ⚠️ Not yet implemented
└── CMakeLists.txt    # Orchestration file
```

**Status**:
- `Module1` module is an example of modern pattern
- Clean naming convention adopted (no `gs` prefix)
- Modern CMake with FILE_SET HEADERS
- Proper namespace aliases (`gismo::Module1`)

### 1.2 Dependency Analysis

#### Circular Dependencies Identified

From `src/DEPENDENCY_MAP.md`:

1. **gsCore ↔ gsMatrix**
   - `gsCore/gsLinearAlgebra.h` → includes `gsMatrix/` headers
   - `gsMatrix/gsFiberMatrix.h` → includes `gsCore/gsLinearAlgebra.h`

2. **gsCore ↔ gsUtils**
   - `gsCore/gsForwardDeclarations.h` → `gsUtils/gsUtils.h`
   - `gsUtils/gsUtils.h` → `gsCore/gsExport.h`, `gsCore/gsDebug.h`

3. **gsCore ↔ gsAssembler**
   - `gsCore/gsMultiBasis.h` → `gsAssembler/gsAssemblerOptions.h`
   - `gsAssembler/*` → 18+ includes from `gsCore/`

4. **gsCore ↔ gsPde**
   - `gsCore/gsMultiBasis.h` → `gsPde/gsBoundaryConditions.h`
   - `gsPde/gsPde.h` → `gsCore/gsMultiPatch.h`

5. **gsCore ↔ gsIO**
   - `gsCore/gsJITCompiler.h` → `gsIO/gsXml.h`
   - `gsIO/*` → 16+ includes from `gsCore/`

6. **gsCore ↔ gsMesh2**
   - `gsCore/gsBoxTopology.h` → `gsMesh2/gsProperty.h`
   - `gsMesh2/gsSurfMesh.h` → `gsCore/gsLinearAlgebra.h`

#### Dependency Layers (Current Reality)

```
❌ BROKEN: All modules effectively at same level due to circular deps

Layer Mixing:
- gsCore depends on: gsMatrix, gsUtils, gsAssembler, gsPde, gsIO, gsMesh2
- gsAssembler depends on: gsCore, gsDomain, gsHSplines, gsIO, gsMatrix, gsMSplines, gsPde
- gsIO depends on: gsCore, gsAssembler, gsDomain, gsHSplines, gsMatrix, gsMSplines
```

### 1.3 Optional Modules Analysis

#### Optional Modules (optional/)

```
optional/
├── gsCoDiPack/       # Automatic differentiation (CoDiPack library)
├── gsHLBFGS/         # L-BFGS optimization
├── gsIpOpt/          # IPOPT optimization
├── gsJSON/           # JSON support
├── gsMultiPrecision/ # Multi-precision arithmetic (GMP/MPFR)
├── gsOpenCascade/    # OpenCascade CAD integration
├── gsOpennurbs/      # OpenNURBS file format
├── gsOptim/          # Optimization library
├── gsParasolid/      # Parasolid CAD integration
├── gsSpectra/        # Eigenvalue computations
├── gsTrilinos/       # Trilinos integration
├── gsUniversal/      # Universal format support
└── gsXBraid/         # XBraid parallel-in-time
```

**Issues**:
- Each optional has own `CMakeLists.txt` but inconsistent patterns
- External dependencies fetched at configure time (gsFetch)
- No clear PUBLIC/PRIVATE dependency specification
- Unclear which optionals depend on which gsCore components
- Template instantiations added globally (`GISMO_EXTRA_INSTANCE`)

#### Example: gsCoDiPack Issues

```cmake
# From optional/gsCoDiPack/CMakeLists.txt
add_library(${PROJECT_NAME} OBJECT gsCoDiPack_.cpp)  # ❌ OBJECT library (old pattern)

# ❌ Global include directory modification
set (GISMO_INCLUDE_DIRS ${GISMO_INCLUDE_DIRS} ${CODIPACK_INCLUDE_DIR}
  CACHE INTERNAL "Gismo include directories" FORCE)

# ❌ Global template instantiation modification
set(GISMO_EXTRA_INSTANCE ${GISMO_EXTRA_INSTANCE}
    "codi::RealForwardGen<${instance}>;..."
    CACHE INTERNAL "Additional instantiations")
```

### 1.4 Build System Configuration

#### Root CMakeLists.txt Analysis

**Issues**:
1. Duplicate project() calls and version management
2. Mixed CMake minimum version requirements (2.8.12 to 3.20)
3. Duplicate build type configuration
4. Global cache variable pollution
5. Complex option system across multiple files

```cmake
# CMakeLists.txt lines 4-10: ❌ Confusing version logic
cmake_minimum_required(VERSION 3.20.0)
if(CMAKE_VERSION VERSION_LESS "3.19")
  cmake_minimum_required(VERSION 2.8.12)  # Contradictory!
else()
  cmake_minimum_required(VERSION 3.1...3.10)  # Also contradictory!
endif()
```

#### src/CMakeLists.txt Analysis

**Current**: Delegates to new `src/gismo/` but has commented-out old build system

```cmake
# Use new modular gismo architecture
add_subdirectory(gismo)

# Original build system preserved below (commented out during transition)
# FOREACH(subdir ${SUBDIRS})
#   add_library(${GM_NAME} OBJECT ...)  # Old OBJECT library pattern
# ENDFOREACH()
```

### 1.5 Test and Example Analysis

#### Examples Structure

- **58 example files** in `examples/`
- All use `#include <gismo.h>` (monolithic header)
- Built with `add_gismo_executable()` macro
- Examples include: `bSplineBasis_example.cpp`, `basis_example.cpp`, `fitting_example.cpp`

#### Unit Tests Structure

- **30+ unit test files** in `unittests/`
- Use UnitTest++ framework (`gismo_unittest.h`)
- Tests include: `gsKnotVectors_test.cpp`, `gsMatrixOp_test.cpp`, `gsExpressions_test.cpp`

---

## Part II: Validation Use Case

### 2.1 Selected Use Case: Matrix Operations

**Rationale**:
- Fundamental to all GISMO operations
- Currently involved in circular dependency (gsCore ↔ gsMatrix)
- Used extensively in tests and examples
- Provides clear validation path

**Validation Artifacts**:
1. **Unit Test**: `unittests/gsMatrixOp_test.cpp` (49 lines, simple)
2. **Example**: `examples/bSplineBasis_example.cpp` (116 lines, uses matrix operations)

### 2.2 Use Case Dependencies

#### Current gsMatrixOp_test.cpp Dependencies

```cpp
#include "gismo_unittest.h"  // → includes <gismo.h>

// Actually uses:
// - gsMatrix<> (from gsCore/gsLinearAlgebra.h or gsMatrix/)
// - gsLinearOperator<> (from gsSolver/)
// - makeMatrixOp() (from gsSolver/)
```

#### Desired New Architecture Dependencies

```
Test: gsMatrixOp_test
  ↓
gismo::Common (Memory, Forward declarations)
  ↓
gismo::Math (Matrix operations)
  ↓
gismo::Solver (Linear operators)
```

### 2.3 Use Case Validation Criteria

**Success Metrics**:
1. ✅ Test compiles with new modular includes
2. ✅ No circular dependencies in module graph
3. ✅ Test passes with identical results
4. ✅ Build time for test ≤ current build time
5. ✅ Clear, explicit dependencies in CMake

### 2.4 Complex Multi-Optional Use Case: Full Research Configuration

**Rationale**:
- Validates the architecture with a realistic research-oriented build
- Tests complex interdependencies between multiple optional modules
- Mirrors actual CI configuration used in production
- Ensures scalability of the optional module system

**Command to Validate**:
```bash
cmake ../ -D GISMO_OPTIONAL="gsModule;gsOpennurbs;gsSpectra;gsElasticity;gsKLShell;gsStructuralAnalysis;gsUnstructuredSplines;gsPolynomial;gsHLBFGS"
```

#### Optional Module Descriptions

| Module | Purpose | External Dependencies | Core Dependencies |
|--------|---------|----------------------|-------------------|
| `gsModule` | Base extension framework | None | `gismo::Common` |
| `gsOpennurbs` | OpenNURBS file support | OpenNURBS library | `gismo::IO`, `gismo::Nurbs` |
| `gsSpectra` | Eigenvalue computations | Spectra-C++ | `gismo::Math`, `gismo::Solver` |
| `gsElasticity` | Elasticity simulations | None | `gismo::Assembler`, `gismo::PDE` |
| `gsKLShell` | Kirchhoff-Love shell analysis | None | `gismo::Elasticity`, `gismo::Assembler` |
| `gsStructuralAnalysis` | Structural analysis tools | None | `gismo::KLShell`, `gismo::Solver` |
| `gsUnstructuredSplines` | Unstructured spline support | None | `gismo::Basis`, `gismo::MSplines` |
| `gsPolynomial` | Polynomial basis functions | None | `gismo::Basis`, `gismo::Math` |
| `gsHLBFGS` | L-BFGS optimization | None | `gismo::Optimizer`, `gismo::Math` |

#### Target Architecture for Complex Build

```
Layer 0: Foundation
  gismo::Common

Layer 1: Mathematics
  gismo::Math
  └── gsPolynomial (extends polynomial functionality)

Layer 2: Geometry & I/O
  gismo::Geometry
  gismo::IO
  └── gsOpennurbs (extends I/O with OpenNURBS support)

Layer 3: Basis Functions
  gismo::Basis
  └── gsUnstructuredSplines (extends basis functions)
  gismo::Nurbs
  gismo::MSplines

Layer 4: Solvers & Analysis
  gismo::Solver
  ├── gsSpectra (extends eigenvalue capabilities)
  └── gsHLBFGS (extends optimization)

Layer 5: Physical Simulations
  gismo::Assembler
  gismo::PDE
  └── gsElasticity (extends PDE with elasticity)
      └── gsKLShell (extends elasticity with shell theory)
          └── gsStructuralAnalysis (high-level structural tools)

Layer 6: Extension Framework
  gsModule (provides base extension infrastructure)
```

#### Complex Build Validation Criteria

**Architecture Requirements**:
1. ✅ All optional modules build without circular dependencies
2. ✅ Clear layered dependency structure maintained
3. ✅ External dependencies (OpenNURBS, Spectra) properly isolated
4. ✅ No global namespace pollution from optional modules
5. ✅ Optional modules can be enabled/disabled independently
6. ✅ Build configuration completes without errors
7. ✅ All optional module interdependencies resolved correctly

**Build System Requirements**:
```cmake
# Each optional module follows modern CMake pattern
add_library(gsElasticity SHARED)  # or STATIC based on GISMO_BUILD_SHARED
add_library(gismo::gsElasticity ALIAS gsElasticity)

# Clear dependency specification
target_link_libraries(gsElasticity
  PUBLIC
    gismo::Assembler
    gismo::PDE
  PRIVATE
    gismo::Math  # Implementation detail
)

# Proper installation and export
install(TARGETS gsElasticity
  EXPORT gismo-optional-targets
  COMPONENT optional-modules
)
```

**Success Metrics**:
1. ✅ `cmake ../` configuration succeeds with all 9 optional modules
2. ✅ Dependency graph validates with no cycles
3. ✅ Build time scales reasonably with number of modules
4. ✅ Each module can be imported independently: `find_package(gismo COMPONENTS gsElasticity)`
5. ✅ Documentation builds with all optional modules included
6. ✅ Examples using multiple optional modules compile and run
7. ✅ Unit tests for all optional modules pass

**Integration Test Case**:
```cpp
// Example integration test using multiple optional modules
#include <gismo/Assembler/Assembler>
#include <gsElasticity/gsElasticity>
#include <gsKLShell/gsKLShell>
#include <gsSpectra/gsSpectra>
#include <gsHLBFGS/gsHLBFGS>

int main() {
    // Load geometry with OpenNURBS
    gismo::io::gsOpennurbsReader reader("shell.3dm");
    auto geometry = reader.readGeometry();

    // Setup KL shell problem
    gismo::elasticity::gsKLShell shell(geometry);
    shell.setMaterial(youngsModulus, poissonRatio);

    // Assemble system
    auto assembler = shell.createAssembler();
    assembler.assemble();

    // Solve eigenvalue problem with Spectra
    gismo::spectra::EigenSolver solver(assembler.matrix());
    auto eigenvalues = solver.compute(10);  // First 10 modes

    // Optimize design with L-BFGS
    gismo::hlbfgs::Optimizer optimizer;
    auto optimizedShell = optimizer.minimize(shell, designVariables);

    return 0;
}
```

**Rollback Strategy for Complex Build**:
- If any optional module fails: exclude it and continue with others
- Graceful degradation: dependent modules automatically disabled
- Clear error messages indicating which dependencies are missing
- Option to build with subset: `GISMO_OPTIONAL="gsOpennurbs;gsSpectra"` still works

---

## Part III: Target Architecture Design

### 3.1 Generic C++ Build Pattern Integration

Following `src/gismo/GENERIC_CPP_BUILD_PATTERN.md`:

#### Directory Structure Template

```
gismo/
├── CMakeLists.txt              # Root project configuration
├── cmake/
│   ├── gismoExport.cmake      # Export configuration
│   └── gismoConfig.cmake.in   # Package config template
└── src/
    ├── CMakeLists.txt         # Source delegation
    └── gismo/                 # Main source namespace
        ├── CMakeLists.txt     # Component orchestration
        ├── Common/            # Foundation (no dependencies)
        ├── Math/              # Mathematics (depends on Common)
        ├── Geometry/          # Geometry primitives (depends on Math)
        ├── Basis/             # Basis functions (depends on Math, Geometry)
        ├── Function/          # Function spaces (depends on Basis)
        ├── Domain/            # Domains and topology (depends on Geometry)
        ├── Assembler/         # Assembly (depends on Function, Domain)
        ├── Solver/            # Solvers (depends on Assembler)
        ├── IO/                # Input/Output (depends on Function)
        ├── Nurbs/             # NURBS geometry (depends on Basis)
        ├── HSplines/          # Hierarchical splines (depends on Nurbs)
        ├── MSplines/          # Multi-patch splines (depends on Nurbs)
        ├── Modeling/          # Geometric modeling (depends on Nurbs)
        ├── Optimizer/         # Optimization (depends on Solver)
        ├── MultiGrid/         # Multigrid methods (depends on Solver)
        └── Parallel/          # Parallelization (depends on Solver)
```

### 3.2 Naming Convention Standards

Following `src/gismo/NAMING_CONVENTIONS.md`:

#### Module Names
- ✅ **PascalCase**: `Common`, `Math`, `Geometry`, `Basis`
- ❌ **No gs prefix**: `Matrix` not `gsMatrix`, `Solver` not `gsSolver`

#### Header Files
- ✅ **PascalCase.h**: `Memory.h`, `Constants.h`, `LinearAlgebra.h`
- ❌ **No gs prefix**: `Matrix.h` not `gsMatrix.h`

#### Module Header Files
- ✅ **No extension**: `Common`, `Math`, `Geometry` (convenience headers)

#### Generated Files
- ✅ **Keep gs prefix**: `gsConfig.h` (maintains compatibility)

### 3.3 Component Template Pattern

Each module follows standardized pattern:

```cmake
### Module CMakeLists.txt Template

project(ModuleName
  VERSION ${${CMAKE_PROJECT_NAME}_VERSION}
  LANGUAGES CXX)

## Choose library type
add_library(${PROJECT_NAME} SHARED)  # or INTERFACE for header-only

## Namespace alias
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

## Source files
target_sources(${PROJECT_NAME}
  PRIVATE
    Implementation.cpp
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        PublicHeader.h
        ${PROJECT_NAME}  # Main module header
)

## Dependencies
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_PROJECT_NAME}::PublicDep
  PRIVATE
    ${CMAKE_PROJECT_NAME}::PrivateDep
)

## Installation
install(TARGETS ${PROJECT_NAME}
  EXPORT ${CMAKE_PROJECT_NAME}${PROJECT_NAME}-targets
  FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/${PROJECT_NAME}
)
```

### 3.4 Dependency Layers (Target Architecture)

```
Layer 0 (Foundation):
  Common         # Memory, forward declarations, debug utilities
    ↓
Layer 1 (Mathematics):
  Math           # Constants, matrix operations, linear algebra
    ↓
Layer 2 (Geometry):
  Geometry       # Points, vectors, transformations
    ↓
Layer 3 (Basis & Functions):
  Basis          # Basis function interfaces
  Function       # Function evaluation
  Domain         # Domain definitions
    ↓
Layer 4 (Splines):
  Nurbs          # NURBS geometry
  HSplines       # Hierarchical splines
  MSplines       # Multi-patch splines
    ↓
Layer 5 (Assembly):
  Assembler      # Matrix assembly
  PDE            # PDE definitions
    ↓
Layer 6 (Solvers):
  Solver         # Linear/nonlinear solvers
  MultiGrid      # Multigrid methods
    ↓
Layer 7 (Applications):
  Modeling       # Geometric modeling
  Optimizer      # Optimization
  Parallel       # Parallelization
    ↓
Layer 8 (I/O):
  IO             # File I/O, visualization
```

**Rules**:
- Modules only depend on lower layers
- No circular dependencies allowed
- Optional modules extend existing layers

---

## Part IV: Implementation Plan

### 4.1 Phase 1: Foundation Modules (Weeks 1-5)

#### Task 1.0: Create Common Module (Week 1)

**Objective**: Establish foundation layer with zero dependencies

**Steps**:
1. Create `src/gismo/Common/` directory structure
2. Extract from `gsCore/`:
   - `gsMemory.h` → `Common/Memory.h`
   - `gsForwardDeclarations.h` → `Common/ForwardDeclarations.h`
   - `gsDebug.h` → `Common/Debug.h`
   - `gsExport.h` → `Common/Export.h`
   - `gsAssert.h` → `Common/Assert.h`
   - `gsConfig.h` → `Common/Config.h`
3. Extract from `gsMatrix/`:
   - `gsEigenDeclarations.h` → `Common/EigenDeclarations.h` (Eigen forward declarations)
   - `gsMatrixAddons.h` → `Common/MatrixAddons.h` (Eigen MatrixBase extensions)
   - `gsPlainObjectBaseAddons.h` → `Common/PlainObjectBaseAddons.h` (Eigen PlainObjectBase extensions)
4. Extract from `gsUtils/`:
   - `gsUtils.h` → `Common/Utils.h` (string utilities, type utilities, macros)
   - `gsStopwatch.h` → `Common/Stopwatch.h` (timing utilities)
   - `gsCombinatorics.h` → `Common/Combinatorics.h` (mathematical utilities)
   - `gsSortedVector.h` → `Common/SortedVector.h` (container utilities)
   - `gsBoundedPriorityQueue.h` → `Common/BoundedPriorityQueue.h` (container utilities)
5. Remove all non-foundation dependencies from extracted headers
6. Create `Common/CMakeLists.txt` following template
7. Establish clean PascalCase naming convention

**Note**: Other gsUtils files will be migrated to appropriate higher layers:
- `gsFunctionWithDerivatives.h` → `Function/` module (depends on gsFunction)
- `gsPointGrid.h` → `Geometry/` or `Math/` module (depends on linear algebra)
- `gsL2Projection.h` → `Assembler/` module (depends on gsExprAssembler)
- `gsQuasiInterpolate.h` → `Assembler/` module (likely depends on assembly)
- `gsMesh/*` → `Geometry/` or separate `Mesh/` module (geometric structures)

**Files to create**:
```
src/gismo/Common/
├── CMakeLists.txt
├── Common                      # Main header (convenience include)
├── Memory.h                    # Memory management utilities
├── ForwardDeclarations.h       # Forward declarations
├── Debug.h                     # Debug macros and utilities
├── Export.h                    # Symbol export/import macros
├── Assert.h                    # Assertion macros
├── Config.h                    # Build configuration
├── Types.h                     # Fundamental type definitions
├── EigenDeclarations.h         # Eigen forward declarations
├── MatrixAddons.h              # Eigen MatrixBase extensions
├── PlainObjectBaseAddons.h     # Eigen PlainObjectBase extensions
├── Utils.h                     # String utilities, type utilities, macros
├── Stopwatch.h                 # Timing and profiling utilities
├── Combinatorics.h             # Mathematical combinatorial functions
├── SortedVector.h              # Sorted container utilities
└── BoundedPriorityQueue.h      # Priority queue utilities
```

**CMakeLists.txt**:
```cmake
project(Common VERSION ${${CMAKE_PROJECT_NAME}_VERSION} LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  INTERFACE
    FILE_SET HEADERS
      BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Common
        Memory.h
        ForwardDeclarations.h
        Debug.h
        Export.h
        Assert.h
        Config.h
        Types.h
        EigenDeclarations.h
        MatrixAddons.h
        PlainObjectBaseAddons.h
        Utils.h
        Stopwatch.h
        Combinatorics.h
        SortedVector.h
        BoundedPriorityQueue.h
)

# No dependencies - this is the foundation layer
target_compile_features(${PROJECT_NAME}
  INTERFACE
    cxx_std_17
)

# Installation rules
install(TARGETS ${PROJECT_NAME}
  EXPORT ${CMAKE_PROJECT_NAME}${PROJECT_NAME}-targets
  FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/${PROJECT_NAME}
)
```

**Validation**:
- Compile standalone without any GISMO dependencies
- Verify clean header includes (no circular references)
- Check naming convention compliance

#### Task 1.1: Create Math Module (Week 2)

**Objective**: Break gsCore ↔ gsMatrix circular dependency

**Steps**:
1. Create `src/gismo/Math/` directory
2. Extract from `gsCore/`:
   - `gsLinearAlgebra.h` → `Math/LinearAlgebra.h`
   - `gsMath.h` → `Math/Constants.h`
   - `gsVector.h`, `gsMatrix.h` from `gsMatrix/`
3. Extract from `gsMatrix/`:
   - Core matrix classes → `Math/Matrix.h`
   - Matrix views → `Math/MatrixView.h`
   - Sparse matrix → `Math/SparseMatrix.h`
4. Extract from `gsUtils/`:
   - `gsPointGrid.h` → `Math/PointGrid.h` (structured point generation)
5. Create `Math/CMakeLists.txt` following template
6. Update `Common` to remove matrix dependencies

**Files to create**:
```
src/gismo/Math/
├── CMakeLists.txt
├── Math                    # Main header
├── LinearAlgebra.h         # Eigen integration
├── Constants.h             # Mathematical constants
├── Matrix.h                # Dense matrix
├── Vector.h                # Vector operations
├── SparseMatrix.h          # Sparse matrix
├── MatrixView.h            # Matrix views
└── PointGrid.h             # Structured point generation
```

**CMakeLists.txt**:
```cmake
project(Math VERSION ${${CMAKE_PROJECT_NAME}_VERSION} LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  INTERFACE
    FILE_SET HEADERS
      BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Math
        LinearAlgebra.h
        Constants.h
        Matrix.h
        Vector.h
        SparseMatrix.h
        MatrixView.h
)

target_link_libraries(${PROJECT_NAME}
  INTERFACE
    ${CMAKE_PROJECT_NAME}::Common
    Eigen3::Eigen
)

# Installation omitted for brevity (follows pattern)
```

**Validation**:
- Compile `gsMatrixOp_test` with new `Math` module
- Verify no circular includes

#### Task 1.2: Create Geometry Module (Week 3)

**Objective**: Extract geometry primitives from gsCore

**Steps**:
1. Create `src/gismo/Geometry/` directory
2. Extract from `gsCore/`:
   - `gsGeometry.h` → `Geometry/Geometry.h`
   - `gsAffineFunction.h` → `Geometry/AffineFunction.h`
   - Basic geometric types
3. Create CMakeLists.txt with dependencies on `Common`, `Math`

**Files to create**:
```
src/gismo/Geometry/
├── CMakeLists.txt
├── Geometry               # Main header
├── Point.h                # Point types
├── Curve.h                # Curve interface
├── Surface.h              # Surface interface
├── Transform.h            # Transformations
└── AffineFunction.h       # Affine functions
```

#### Task 1.3: Create Basis Module (Week 4)

**Objective**: Extract basis function interfaces

**Steps**:
1. Create `src/gismo/Basis/` directory
2. Extract from `gsCore/`:
   - `gsBasis.h` → `Basis/Basis.h`
   - `gsBasisRefs.h` → `Basis/References.h`
   - `gsBoundary.h` → `Basis/Boundary.h`
3. Create CMakeLists.txt

**Files to create**:
```
src/gismo/Basis/
├── CMakeLists.txt
├── Basis                  # Main header
├── Basis.h                # Basis interface
├── References.h           # Basis references
├── Boundary.h             # Boundary handling
└── Evaluator.h            # Evaluation interface
```

#### Task 1.4: Create Function Module (Week 5)

**Objective**: Extract function evaluation and spaces

**Steps**:
1. Create `src/gismo/Function/` directory
2. Extract from `gsCore/`:
   - `gsFunction.h` → `Function/Function.h`
   - `gsFuncData.h` → `Function/FunctionData.h`
3. Create CMakeLists.txt

**Files to create**:
```
src/gismo/Function/
├── CMakeLists.txt
├── Function               # Main header
├── Function.h             # Function interface
├── FunctionData.h         # Function data
└── FunctionSpace.h        # Function spaces
```

### 4.2 Phase 2: Domain & Topology (Week 6)

#### Task 2.1: Create Domain Module

**Objective**: Extract domain definitions from gsDomain

**Steps**:
1. Create `src/gismo/Domain/` directory
2. Migrate from `gsDomain/`:
   - Domain iterators
   - Tensor domains
   - Hierarchical domains
3. Remove `gs` prefix from all files
4. Create modern CMakeLists.txt

**Files to create**:
```
src/gismo/Domain/
├── CMakeLists.txt
├── Domain                 # Main header
├── Domain.h               # Domain interface
├── Iterator.h             # Domain iterators
└── TensorDomain.h         # Tensor domains
```

### 4.3 Phase 3: Spline Modules (Weeks 7-9)

#### Task 3.1: Create Nurbs Module (Week 7)

**Objective**: Port gsNurbs to modern architecture

**Steps**:
1. Create `src/gismo/Nurbs/` directory
2. Migrate from `gsNurbs/`:
   - NURBS basis functions
   - B-spline implementations
   - Knot vectors
3. Remove `gs` prefix
4. Depend on `Basis`, `Geometry`, `Domain`

#### Task 3.2: Create HSplines Module (Week 8)

**Objective**: Port gsHSplines

**Steps**:
1. Create `src/gismo/HSplines/` directory
2. Migrate hierarchical spline implementations
3. Depend on `Nurbs`

#### Task 3.3: Create MSplines Module (Week 9)

**Objective**: Port gsMSplines

**Steps**:
1. Create `src/gismo/MSplines/` directory
2. Migrate multi-patch spline implementations
3. Depend on `Nurbs`

### 4.4 Phase 4: Assembly & Solver (Weeks 10-12)

#### Task 4.1: Create Assembler Module (Week 10)

**Objective**: Break gsCore ↔ gsAssembler circular dependency

**Steps**:
1. Create `src/gismo/Assembler/` directory
2. Migrate from `gsAssembler/`:
   - Assembly operations
   - Quadrature rules
   - Expression templates
3. Depend on `Function`, `Domain`, `Math`

#### Task 4.2: Create PDE Module (Week 11)

**Objective**: Break gsCore ↔ gsPde circular dependency

**Steps**:
1. Create `src/gismo/PDE/` directory
2. Migrate PDE definitions
3. Depend on `Assembler`

#### Task 4.3: Create Solver Module (Week 12)

**Objective**: Port gsSolver

**Steps**:
1. Create `src/gismo/Solver/` directory
2. Migrate solvers and preconditioners
3. Depend on `Assembler`, `Math`

### 4.5 Phase 5: High-Level Modules (Weeks 13-15)

#### Task 5.1: Create Modeling Module (Week 13)

**Objective**: Port gsModeling

**Steps**:
1. Create `src/gismo/Modeling/` directory
2. Migrate geometric modeling tools
3. Depend on `Nurbs`, `HSplines`

#### Task 5.2: Create Optimizer Module (Week 14)

**Objective**: Port gsOptimizer

**Steps**:
1. Create `src/gismo/Optimizer/` directory
2. Migrate optimization algorithms
3. Depend on `Solver`

#### Task 5.3: Create MultiGrid Module (Week 15)

**Objective**: Port gsMultiGrid

**Steps**:
1. Create `src/gismo/MultiGrid/` directory
2. Migrate multigrid methods
3. Depend on `Solver`

### 4.6 Phase 6: I/O & Parallel (Weeks 16-17)

#### Task 6.1: Create IO Module (Week 16)

**Objective**: Break gsCore ↔ gsIO circular dependency

**Steps**:
1. Create `src/gismo/IO/` directory
2. Migrate from `gsIO/`:
   - File I/O operations
   - XML handling
   - Visualization output
3. Depend on `Function`, `Geometry` (not Core!)

#### Task 6.2: Create Parallel Module (Week 17)

**Objective**: Port gsParallel

**Steps**:
1. Create `src/gismo/Parallel/` directory
2. Migrate MPI/parallel implementations
3. Depend on `Solver`

### 4.7 Phase 7: Optional Modules Refactoring (Weeks 18-21)

#### Task 7.1: Audit Optional Dependencies (Week 18)

**Steps**:
1. Create dependency matrix for each optional
2. Identify required GISMO modules
3. Document external library dependencies
4. Create `optional/DEPENDENCIES.md`

#### Task 7.2: Refactor Optional CMake (Weeks 19-20)

**Objective**: Modernize optional module builds

**Template for each optional**:
```cmake
# optional/CoDiPack/CMakeLists.txt
project(CoDiPack VERSION ${${CMAKE_PROJECT_NAME}_VERSION} LANGUAGES CXX)

# Fetch external dependency
include(gismoFetch)
gismo_fetch_directory(CoDiPack URL ... DESTINATION external)

# Create modern library target
add_library(${PROJECT_NAME} INTERFACE)  # Header-only
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Define dependencies explicitly
target_link_libraries(${PROJECT_NAME}
  INTERFACE
    ${CMAKE_PROJECT_NAME}::Common
    ${CMAKE_PROJECT_NAME}::Math
)

# Include directories (modern)
target_include_directories(${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CODIPACK_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)
```

**Steps**:
1. Convert each optional from OBJECT to SHARED/STATIC/INTERFACE
2. Define explicit target dependencies
3. Remove global cache variable pollution
4. Use modern CMake patterns

**Handling Complex Multi-Optional Builds**:

For the command `cmake ../ -D GISMO_OPTIONAL="gsModule;gsOpennurbs;gsSpectra;gsElasticity;gsKLShell;gsStructuralAnalysis;gsUnstructuredSplines;gsPolynomial;gsHLBFGS"`, the system must:

1. **Parse module list and validate dependencies**:
   ```cmake
   # optional/CMakeLists.txt
   set(AVAILABLE_OPTIONALS gsOpennurbs gsSpectra gsHLBFGS) # Currently implemented
   set(MISSING_OPTIONALS gsModule gsElasticity gsKLShell gsStructuralAnalysis gsUnstructuredSplines gsPolynomial)

   foreach(module ${GISMO_OPTIONAL_LIST})
     if(module IN_LIST MISSING_OPTIONALS)
       message(WARNING "Optional module '${module}' is not yet implemented. Skipping.")
       continue()
     endif()

     if(module IN_LIST AVAILABLE_OPTIONALS)
       set(GISMO_WITH_${module} ON)
     endif()
   endforeach()
   ```

2. **Graceful dependency resolution**:
   ```cmake
   # For modules that depend on missing ones
   if(GISMO_WITH_gsKLShell AND NOT GISMO_WITH_gsElasticity)
     message(WARNING "gsKLShell requires gsElasticity which is not available. Disabling gsKLShell.")
     set(GISMO_WITH_gsKLShell OFF)
   endif()
   ```

3. **Clear build summary**:
   ```cmake
   message(STATUS "=== GISMO Optional Modules Configuration ===")
   message(STATUS "Requested: ${GISMO_OPTIONAL}")
   message(STATUS "Available: ${ENABLED_OPTIONALS}")
   message(STATUS "Missing/Skipped: ${SKIPPED_OPTIONALS}")
   message(STATUS "===========================================")
   ```

#### Task 7.3: Document Optional Module System (Week 21)

**Create documentation**:
1. `optional/README.md` - Overview of optional system
2. `optional/DEPENDENCIES.md` - Dependency matrix
3. Individual README per optional

### 4.8 Phase 8: Testing & Validation (Weeks 22-25)

#### Task 8.1: Validate Use Cases (Week 22)

**Primary Validation Test**: `gsMatrixOp_test`

**Steps**:
1. Build with new modular architecture
2. Update includes from `<gismo.h>` to specific modules:
   ```cpp
   #include <gismo/Common/Common>
   #include <gismo/Math/Math>
   #include <gismo/Solver/Solver>
   ```
3. Run test and verify results match
4. Measure build time vs. old architecture

**Complex Multi-Optional Validation**:

**Command**:
```bash
cmake ../ -D GISMO_OPTIONAL="gsModule;gsOpennurbs;gsSpectra;gsElasticity;gsKLShell;gsStructuralAnalysis;gsUnstructuredSplines;gsPolynomial;gsHLBFGS"
```

**Validation Steps**:
1. Ensure all 9 optional modules are properly recognized by CMake
2. Verify dependency resolution between optional modules
3. Check that missing optional modules (not yet implemented) are gracefully handled
4. Validate build system generates proper error/warning messages
5. Test subset builds work: `GISMO_OPTIONAL="gsOpennurbs;gsSpectra;gsHLBFGS"`
6. Measure configuration time with complex optional setup
7. Verify no circular dependencies in full build graph

#### Task 8.2: Update All Unit Tests (Week 23)

**Steps**:
1. For each unit test in `unittests/`:
   - Identify actual dependencies
   - Replace `<gismo.h>` with specific module includes
   - Update CMake to link specific targets
   - Verify test passes
2. Create `unittests/MIGRATION.md` documenting changes

#### Task 8.3: Update Examples (Week 24)

**Steps**:
1. For each example in `examples/`:
   - Identify actual dependencies
   - Replace monolithic include
   - Update CMake
   - Verify builds and runs
2. Create `examples/MIGRATION.md`

#### Task 8.4: Comprehensive Testing (Week 25)

**Test Suite**:
1. ✅ All unit tests pass
2. ✅ All examples build and run
3. ✅ No circular dependencies (automated check)
4. ✅ Build time comparison (target: ≤10% slower initial, faster incremental)
5. ✅ Installation test (install + use as package)
6. ✅ Documentation builds

**Automated Dependency Check**:
```cmake
# cmake/CheckCircularDeps.cmake
function(gismo_check_circular_dependencies)
  # Use CMake's dependency graph
  # Fail build if circular dependencies detected
  # Generate dependency visualization
endfunction()
```

### 4.9 Phase 9: Documentation & Migration (Weeks 26-27)

#### Task 9.1: Update Build Documentation (Week 26)

**Documents to create/update**:
1. `docs/BUILD.md` - Modern build instructions
2. `docs/ARCHITECTURE.md` - Architecture overview
3. `docs/MODULES.md` - Module reference
4. `docs/DEPENDENCIES.md` - Dependency graph
5. `MIGRATION.md` - User migration guide

#### Task 9.2: Create Migration Tools (Week 27)

**Tools**:
1. `tools/migrate_includes.py` - Script to update includes
2. `tools/analyze_deps.py` - Dependency analyzer
3. `tools/check_naming.py` - Naming convention checker

#### Task 9.3: Backward Compatibility Layer

**Strategy**:
- Keep old `gsCore/*.h` headers as forwarding headers
- Deprecation warnings
- Compatibility targets in CMake

**Example forwarding header**:
```cpp
// src/gsCore/gsMemory.h (deprecated)
#ifndef GISMO_GSCORE_GSMEMORY_H
#define GISMO_GSCORE_GSMEMORY_H

#pragma message("WARNING: gsCore/gsMemory.h is deprecated. " \
                "Use gismo/Common/Memory.h instead.")

#include <gismo/Common/Memory.h>

// Namespace compatibility
namespace gismo {
  using namespace gismo; // Already there, just for documentation
}

#endif // GISMO_GSCORE_GSMEMORY_H
```

---

## Part V: Build System Improvements

### 5.1 Root CMakeLists.txt Cleanup

**Issues to fix**:
1. Remove contradictory cmake_minimum_required
2. Single version source of truth
3. Clean option management
4. Remove duplicate build type configuration

**Improved root CMakeLists.txt**:
```cmake
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(gismo
    VERSION 25.7.0
    LANGUAGES CXX
    DESCRIPTION "Geometry + Simulation Modules"
    HOMEPAGE_URL "http://gismo.github.io/"
)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Output directories
include(GNUInstallDirs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# RPATH
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Modern CMake features
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(GenerateExportHeader)

# Options
include(cmake/gismoOptions.cmake)

# Dependencies
find_package(Eigen3 REQUIRED)

# Source tree
add_subdirectory(src)

# Optional components
if(GISMO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GISMO_BUILD_UNITTESTS)
    enable_testing()
    add_subdirectory(unittests)
endif()

# Optional modules
if(GISMO_WITH_OPTIONALS)
    add_subdirectory(optional)
endif()

# Export configuration
include(cmake/gismoExport.cmake)
export(PACKAGE ${PROJECT_NAME})

# Status
include(cmake/gismoStatus.cmake)
```

### 5.2 Component Orchestration

**src/gismo/CMakeLists.txt** (orchestration file):
```cmake
# Layer 0: Foundation
add_subdirectory(Common)

# Layer 1: Mathematics
add_subdirectory(Math)

# Layer 2: Geometry
add_subdirectory(Geometry)

# Layer 3: Basis & Functions
add_subdirectory(Basis)
add_subdirectory(Function)
add_subdirectory(Domain)

# Layer 4: Splines
add_subdirectory(Nurbs)
add_subdirectory(HSplines)
add_subdirectory(MSplines)

# Layer 5: Assembly
add_subdirectory(Assembler)
add_subdirectory(PDE)

# Layer 6: Solvers
add_subdirectory(Solver)
add_subdirectory(MultiGrid)

# Layer 7: Applications
add_subdirectory(Modeling)
add_subdirectory(Optimizer)
add_subdirectory(Parallel)

# Layer 8: I/O
add_subdirectory(IO)

# Convenience: Create umbrella target
add_library(gismo INTERFACE)
target_link_libraries(gismo INTERFACE
    gismo::Common
    gismo::Math
    gismo::Geometry
    gismo::Basis
    gismo::Function
    gismo::Domain
    gismo::Nurbs
    gismo::Assembler
    gismo::Solver
    gismo::IO
)
add_library(gismo::gismo ALIAS gismo)
```

### 5.3 Optional Modules Integration

**optional/CMakeLists.txt**:
```cmake
# Optional modules - each is truly optional

# Automatic differentiation
if(GISMO_WITH_CODIPACK)
    add_subdirectory(CoDiPack)
endif()

# Optimization
if(GISMO_WITH_IPOPT)
    add_subdirectory(IpOpt)
endif()

if(GISMO_WITH_HLBFGS)
    add_subdirectory(HLBFGS)
endif()

# Multi-precision
if(GISMO_WITH_MULTIPRECISION)
    add_subdirectory(MultiPrecision)
endif()

# CAD integration
if(GISMO_WITH_OPENCASCADE)
    add_subdirectory(OpenCascade)
endif()

if(GISMO_WITH_OPENNURBS)
    add_subdirectory(OpenNurbs)
endif()

# Eigensolvers
if(GISMO_WITH_SPECTRA)
    add_subdirectory(Spectra)
endif()

# Trilinos
if(GISMO_WITH_TRILINOS)
    add_subdirectory(Trilinos)
endif()

# Parallel-in-time
if(GISMO_WITH_XBRAID)
    add_subdirectory(XBraid)
endif()
```

---

## Part VI: Success Metrics & Validation

### 6.1 Quantitative Metrics

| Metric | Current | Target | Validation Method |
|--------|---------|--------|-------------------|
| Circular dependencies | 6+ | 0 | Automated CMake check |
| Build time (clean) | Baseline | ≤110% | CI benchmarks |
| Build time (incremental) | Baseline | ≤50% | Change one header, rebuild |
| Header includes per module | ~50 | ≤20 | Static analysis |
| Test coverage | 75% | ≥75% | gcov/lcov |
| Module independence | 0% | 90% | Dependency graph |

### 6.2 Qualitative Metrics

- ✅ Clear module responsibilities
- ✅ Logical dependency flow
- ✅ Easy to add new features
- ✅ Simple to understand for new developers
- ✅ Modern CMake best practices
- ✅ Consistent naming throughout

### 6.3 Validation Checklist

#### Per-Module Validation

For each module:
- [ ] CMakeLists.txt follows template pattern
- [ ] Clean PascalCase naming (no `gs` prefix)
- [ ] Main module header with no extension
- [ ] Explicit PUBLIC/PRIVATE dependencies
- [ ] No circular dependencies
- [ ] Proper namespace alias (`gismo::Module`)
- [ ] FILE_SET HEADERS configured
- [ ] Installation rules defined
- [ ] Export targets created

#### System-Wide Validation

- [ ] All unit tests pass
- [ ] All examples build and run
- [ ] Documentation builds
- [ ] Install + find_package works
- [ ] No circular dependencies (automated check)
- [ ] Build time acceptable
- [ ] Code coverage maintained

---

## Part VII: Risk Management

### 7.1 Technical Risks

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| Breaking user code | High | High | Compatibility layer, deprecation warnings |
| Performance regression | Medium | Medium | Continuous benchmarking |
| Template issues across modules | Medium | High | Careful header organization, explicit instantiation |
| Build time increase | Medium | Medium | Incremental migration, precompiled headers |
| Incomplete dependency mapping | Low | High | Thorough analysis, automated tools |
| Missing optional modules in complex builds | Medium | Low | Graceful degradation, clear error messages |

### 7.2 Schedule Risks

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| Underestimated complexity | Medium | Medium | 20% time buffer, phased approach |
| Unforeseen dependencies | Low | Medium | Early validation use case |
| Resource availability | Medium | High | Clear documentation, modular tasks |

### 7.3 Rollback Strategy

**If critical issues arise**:
1. Keep old architecture in parallel during transition
2. CMake option to switch between old/new: `GISMO_USE_LEGACY_BUILD`
3. Can revert per-module (not all-or-nothing)
4. Git branches for each phase

---

## Part VIII: Timeline Summary

**Total Duration**: 26 weeks (6.5 months)

| Phase | Weeks | Deliverable |
|-------|-------|-------------|
| 1. Foundation | 1-4 | Math, Geometry, Basis, Function modules |
| 2. Domain | 5 | Domain module |
| 3. Splines | 6-8 | Nurbs, HSplines, MSplines |
| 4. Assembly | 9-11 | Assembler, PDE, Solver |
| 5. High-Level | 12-14 | Modeling, Optimizer, MultiGrid |
| 6. I/O & Parallel | 15-16 | IO, Parallel |
| 7. Optionals | 17-20 | Refactored optional modules |
| 8. Testing | 21-24 | Full test suite validation |
| 9. Documentation | 25-26 | Docs, migration tools |

**Milestones**:
- Week 4: Foundation modules complete, matrix use case validates
- Week 11: Core functionality migrated
- Week 16: All primary modules migrated
- Week 20: Optional modules modernized
- Week 24: All tests passing, complex multi-optional build validates
- Week 26: Production-ready

---

## Part IX: Next Steps

### Immediate Actions (Week 1)

1. **Approve this plan** - Team review and sign-off
2. **Set up CI/CD** - Automated dependency checking
3. **Create feature branch** - `feature/modern-architecture`
4. **Begin Task 1.1** - Create Math module
5. **Validate use case** - Get gsMatrixOp_test working with Math module

### Development Workflow

1. **Create module** following template
2. **Update dependencies** in existing code
3. **Write/update tests** for module
4. **Run validation suite** (tests + examples)
5. **Document changes** in module README
6. **Code review** before merge
7. **Update progress** in this document

### Communication Plan

- **Weekly status updates** - Progress on implementation plan
- **Architecture decisions** - Document in ADR (Architecture Decision Records)
- **Breaking changes** - Announce in CHANGELOG with migration guide
- **Community engagement** - RFC for major changes

---

## Part X: Appendices

### Appendix A: Module Dependency Matrix

| Module | Layer | Depends On |
|--------|-------|------------|
| Common | 0 | Eigen3 |
| Math | 1 | Common, Eigen3 |
| Geometry | 2 | Math, Common |
| Basis | 3 | Geometry, Math, Common |
| Function | 3 | Basis, Geometry, Math, Common |
| Domain | 3 | Geometry, Math, Common |
| Nurbs | 4 | Basis, Domain, Geometry |
| HSplines | 4 | Nurbs, Basis, Domain |
| MSplines | 4 | Nurbs, Basis, Domain |
| Assembler | 5 | Function, Domain, Math |
| PDE | 5 | Assembler, Function |
| Solver | 6 | Assembler, Math |
| MultiGrid | 6 | Solver, Nurbs |
| Modeling | 7 | Nurbs, HSplines, Optimizer |
| Optimizer | 7 | Solver, Math |
| Parallel | 7 | Solver, MPI |
| IO | 8 | Function, Geometry (minimal deps) |

### Appendix B: File Migration Checklist

For each file in gsCore:

```
[ ] gsMemory.h → Common/Memory.h
[ ] gsForwardDeclarations.h → Common/ForwardDeclarations.h
[ ] gsDebug.h → Common/Debug.h
[ ] gsExport.h → Common/Export.h
[ ] gsLinearAlgebra.h → Math/LinearAlgebra.h
[ ] gsMath.h → Math/Constants.h
[ ] gsGeometry.h → Geometry/Geometry.h
[ ] gsAffineFunction.h → Geometry/AffineFunction.h
[ ] gsBasis.h → Basis/Basis.h
[ ] gsBasisRefs.h → Basis/References.h
[ ] gsFunction.h → Function/Function.h
[ ] gsFuncData.h → Function/FunctionData.h
[ ] gsBoundary.h → Basis/Boundary.h (or Domain/)
[ ] gsBoxTopology.h → Domain/BoxTopology.h
[ ] gsJITCompiler.h → IO/JITCompiler.h (or separate Compiler module)
[ ] ... (complete list: ~83 files)
```

### Appendix C: Validation Use Case Details

**gsMatrixOp_test.cpp Modified for New Architecture**:

```cpp
/** @file gsMatrixOp_test.cpp
    @brief Tests for Matrix operations with new modular architecture
*/

// New modular includes
#include <gismo/Common/Common>    // Forward declarations, memory
#include <gismo/Math/Math>        // Matrix operations
#include <gismo/Solver/Solver>    // Linear operators

#include "gismo_unittest.h"

SUITE(gsMatrixOp_test)
{
    TEST(DenseMatrix)
    {
        gismo::math::Matrix<> A(3,3);  // New namespace
        A << 2,2,3,  4,5,6,  7,8,10;

        auto Aop = gismo::solver::makeMatrixOp(A);

        A(0,0) = 1;

        gismo::math::Matrix<> C;
        Aop->toMatrix(C);

        CHECK((A - C).norm() <= 1.e-10);
    }

    // ... rest of tests
}
```

**CMakeLists.txt for test**:
```cmake
add_executable(gsMatrixOp_test gsMatrixOp_test.cpp)
target_link_libraries(gsMatrixOp_test
    PRIVATE
        gismo::Common
        gismo::Math
        gismo::Solver
        UnitTest++
)
add_test(NAME gsMatrixOp_test COMMAND gsMatrixOp_test)
```

### Appendix D: References

- **Generic C++ Build Pattern**: `src/gismo/GENERIC_CPP_BUILD_PATTERN.md`
- **Naming Conventions**: `src/gismo/NAMING_CONVENTIONS.md`
- **Architecture Analysis**: `src/ARCHITECTURE.md`
- **Dependency Map**: `src/DEPENDENCY_MAP.md`
- **CMake Documentation**: https://cmake.org/cmake/help/latest/
- **Modern CMake Guide**: https://cliutils.gitlab.io/modern-cmake/

---

## Document Version Control

- **Version**: 1.0
- **Date**: 2024
- **Status**: Implementation Plan - Ready for Approval
- **Authors**: GISMO Architecture Team
- **Next Review**: After Phase 1 completion

---

**END OF AGENTS.MD**
