# GISMO Architecture Development Plan

## Executive Summary

This document provides a comprehensive development plan for transforming the GISMO library architecture from the current hierarchical structure with circular dependencies to a clean, flattened modular architecture. The new design eliminates the problematic `gsCore` module by distributing its functionality across independent, purpose-built modules.

**Current Critical Issues:**
- 6 circular dependencies involving `gsCore`
- 68 external dependencies in `gsAssembler` alone
- Monolithic `gsCore` module creating architectural bottlenecks

**Proposed Solution:**
- Flatten architecture to single-level modules
- Break `gsCore` into 7 specialized modules
- Eliminate circular dependencies through careful interface design
- Maintain backward compatibility through header forwarding

## Current Architecture Analysis

### Module Dependency Map

```
Current Modules (src/):
├── gsAssembler (68 deps) → Heavy dependencies on gsCore, gsPde, gsMatrix
├── gsCore (45 deps) → Central but creates circular dependencies
├── gsPde (32 deps) → Depends on gsCore, gsAssembler
├── gsIO (24 deps) → File I/O utilities, depends on gsCore
├── gsNurbs (22 deps) → NURBS functionality, depends on gsCore
├── gsSolver (21 deps) → Depends on gsAssembler, gsCore
├── gsUtils (20 deps) → Utilities, circular dependency with gsCore
├── gsModeling (19 deps) → Geometric modeling
├── gsHSplines (17 deps) → Hierarchical splines
├── gsMSplines (15 deps) → Multipatch splines
├── gsOptimizer (14 deps) → Optimization algorithms
├── gsMultiGrid (12 deps) → Multigrid methods
├── gsDomain (11 deps) → Domain definitions
├── gsIeti (8 deps) → IETI methods
├── gsTensor (6 deps) → Tensor operations
├── gsParallel (5 deps) → Parallelization
├── gsMesh2 (2 deps) → Mesh operations
└── gsMatrix (1 dep) → Matrix operations, circular with gsCore
```

### Circular Dependencies Identified

1. **gsCore ↔ gsMatrix**: Core matrix operations interdependency
2. **gsCore ↔ gsUtils**: Utility functions circular reference
3. **gsCore ↔ gsAssembler**: Assembly operations interdependency
4. **gsCore ↔ gsPde**: PDE definitions circular reference
5. **gsCore ↔ gsIO**: I/O operations interdependency
6. **gsCore ↔ gsMesh2**: Mesh operations circular reference

### Optional Modules Analysis

**Highly Optional (can be disabled):**
- `gsIeti`: Specialized iterative methods
- `gsOptimizer`: Non-core optimization
- `gsParallel`: Parallelization features

**External Optional Modules (optional/):**
- `gsCoDiPack`, `gsHLBFGS`, `gsIpOpt`, `gsOpenCascade`
- `gsOpennurbs`, `gsSpectra`, `gsTrilinos`, etc.

## Target Architecture Design

### Flattened Module Structure

The new architecture eliminates `gsCore` by creating specialized modules:

```
New Flattened Architecture:
├── gsBase          → Basic types, memory, smart pointers
├── gsMath          → Mathematical utilities, constants
├── gsGeometry      → Geometric primitives, transformations
├── gsBasis         → Basis function definitions
├── gsFunction      → Function spaces, evaluation
├── gsTopology      → Mesh topology, connectivity
├── gsCompiler      → JIT compilation support
├── gsAssembler     → Assembly operations (refactored)
├── gsPde          → PDE definitions (refactored)
├── gsIO           → File I/O (refactored)
├── gsNurbs        → NURBS functionality
├── gsSolver       → Linear/nonlinear solvers
├── gsUtils        → High-level utilities
├── gsModeling     → Geometric modeling
├── gsHSplines     → Hierarchical splines
├── gsMSplines     → Multipatch splines
├── gsOptimizer    → Optimization algorithms
├── gsMultiGrid    → Multigrid methods
├── gsDomain       → Domain definitions
├── gsIeti         → IETI methods
├── gsTensor       → Tensor operations
├── gsParallel     → Parallelization
├── gsMesh2        → Mesh operations
└── gsMatrix       → Matrix operations
```

### Module Responsibility Matrix

| Module | Primary Responsibility | Key Files to Migrate |
|--------|------------------------|---------------------|
| **gsBase** | Foundation types, memory management | `gsMemory.h`, `gsForwardDeclarations.h` |
| **gsMath** | Mathematical constants, basic math | `gsMath.h`, `gsConstants.h` |
| **gsGeometry** | Points, vectors, geometric primitives | `gsGeometry.h`, `gsAffineFunction.h` |
| **gsBasis** | Basis function interfaces | `gsBasis.h`, `gsBasisRefs.h` |
| **gsFunction** | Function evaluation, spaces | `gsFunction.h`, `gsFuncData.h` |
| **gsTopology** | Mesh topology, boundaries | `gsBoundary.h`, `gsBoxTopology.h` |
| **gsCompiler** | JIT compilation utilities | `gsJITCompiler.h`, compilation logic |

## Development Implementation Plan

### Phase 1: Foundation Modules (Weeks 1-3)

**Step 1.1: Create gsBase Module**
- Move core types from `gsCore/gsMemory.h`
- Create `src/gsBase/gsMemory.h`
- Migrate forward declarations
- Update CMakeLists.txt

**Step 1.2: Create gsMath Module**
- Extract mathematical utilities from `gsCore/gsMath.h`
- Create `src/gsMath/gsMath.h`
- Move constants and basic math functions

**Step 1.3: Create gsGeometry Module**
- Move geometric primitives from `gsCore/gsGeometry.h`
- Migrate affine functions and transformations
- Create clean geometric interfaces

### Phase 2: Function and Basis Modules (Weeks 4-6)

**Step 2.1: Create gsBasis Module**
- Extract basis definitions from `gsCore/gsBasis.h`
- Move `gsBasisRefs.h` functionality
- Create clean basis interfaces

**Step 2.2: Create gsFunction Module**
- Migrate function evaluation from `gsCore/gsFunction.h`
- Move `gsFuncData.h` and related functionality
- Establish function space interfaces

**Step 2.3: Create gsTopology Module**
- Extract boundary and topology from `gsCore/gsBoundary.h`
- Move mesh topology definitions
- Create connectivity interfaces

### Phase 3: Compiler and Refactoring (Weeks 7-9)

**Step 3.1: Create gsCompiler Module**
- Move JIT compilation from `gsCore/`
- Create isolated compilation interfaces
- Ensure no circular dependencies

**Step 3.2: Refactor Dependent Modules**
- Update `gsAssembler` to use new module interfaces
- Refactor `gsPde` dependencies
- Update `gsIO` module dependencies
- Fix `gsMatrix` circular dependency

### Phase 4: Integration and Testing (Weeks 10-12)

**Step 4.1: Update Build System**
- Modify all `CMakeLists.txt` files
- Update include paths throughout codebase
- Create compatibility headers for transition

**Step 4.2: Comprehensive Testing**
- Run full test suite with new architecture
- Verify no circular dependencies remain
- Performance regression testing

**Step 4.3: Documentation Update**
- Update all documentation for new structure
- Create migration guide for users
- Update build instructions

## Detailed File Migration Plan

### gsCore Content Distribution

**Current gsCore Files → Target Modules:**

```
gsCore/gsMemory.h           → gsBase/gsMemory.h
gsCore/gsForwardDeclarations.h → gsBase/gsForwardDeclarations.h
gsCore/gsMath.h             → gsMath/gsMath.h
gsCore/gsConstants.h        → gsMath/gsConstants.h
gsCore/gsGeometry.h         → gsGeometry/gsGeometry.h
gsCore/gsAffineFunction.h   → gsGeometry/gsAffineFunction.h
gsCore/gsBasis.h            → gsBasis/gsBasis.h
gsCore/gsBasisRefs.h        → gsBasis/gsBasisRefs.h
gsCore/gsFunction.h         → gsFunction/gsFunction.h
gsCore/gsFuncData.h         → gsFunction/gsFuncData.h
gsCore/gsBoundary.h         → gsTopology/gsBoundary.h
gsCore/gsBoxTopology.h      → gsTopology/gsBoxTopology.h
gsCore/gsJITCompiler.h      → gsCompiler/gsJITCompiler.h
```

### CMakeLists.txt Updates Required

**Root CMakeLists.txt:**
- Update GISMO_SOURCES list
- Remove gsCore references
- Add new module directories

**Individual Module CMakeLists.txt:**
- Create CMakeLists.txt for each new module
- Define proper target dependencies
- Ensure no circular target dependencies

### Header Compatibility Strategy

**Transition Compatibility:**
Create forwarding headers in `gsCore/` that include new module headers:

```cpp
// gsCore/gsMemory.h (compatibility header)
#ifndef GSCORE_GSMEMORY_H
#define GSCORE_GSMEMORY_H
#warning "gsCore/gsMemory.h is deprecated. Use gsBase/gsMemory.h instead"
#include <gsBase/gsMemory.h>
#endif
```

## Dependency Resolution Strategy

### Module Dependency Rules

1. **Foundation Layer**: `gsBase`, `gsMath` have no internal dependencies
2. **Geometry Layer**: `gsGeometry` depends only on Foundation
3. **Function Layer**: `gsBasis`, `gsFunction` depend on Foundation + Geometry
4. **Application Layer**: All other modules can depend on lower layers

### Circular Dependency Elimination

**gsCore ↔ gsMatrix Resolution:**
- Move matrix utilities to `gsBase`
- Keep high-level matrix operations in `gsMatrix`
- `gsMatrix` depends on `gsBase`, not vice versa

**gsCore ↔ gsAssembler Resolution:**
- Move assembly interfaces to `gsFunction`
- `gsAssembler` implements interfaces from `gsFunction`
- No reverse dependency

## Inter-Module Dependency Reality

**Answer to Standalone Question**: No, modules will NOT be completely standalone. The new architecture creates a clean dependency hierarchy:

```
Dependency Layers (Bottom to Top):
Layer 1: gsBase, gsMath (no dependencies)
Layer 2: gsGeometry (depends on Layer 1)
Layer 3: gsBasis, gsFunction, gsTopology (depends on Layers 1-2)
Layer 4: gsCompiler, gsMatrix, gsIO (depends on Layers 1-3)
Layer 5: gsAssembler, gsPde, gsSolver (depends on Layers 1-4)
Layer 6: All other modules (depends on appropriate lower layers)
```

**Benefits of This Structure:**
- Clear dependency direction (never circular)
- Logical functional grouping
- Easier testing and maintenance
- Plugin-style architecture for optional modules

## Build System Changes

### New CMake Structure

**Root src/CMakeLists.txt:**
```cmake
# Foundation modules (no dependencies)
add_subdirectory(gsBase)
add_subdirectory(gsMath)

# Geometry layer
add_subdirectory(gsGeometry)

# Function layer
add_subdirectory(gsBasis)
add_subdirectory(gsFunction)
add_subdirectory(gsTopology)

# Infrastructure layer
add_subdirectory(gsCompiler)
add_subdirectory(gsMatrix)
add_subdirectory(gsIO)

# Application layer
add_subdirectory(gsAssembler)
add_subdirectory(gsPde)
add_subdirectory(gsSolver)
# ... other modules
```

### Target Dependencies

**Example Module CMakeLists.txt (gsFunction):**
```cmake
set(gsFunction_HEADERS
    gsFunction.h
    gsFuncData.h
    # ... other headers
)

set(gsFunction_SOURCES
    gsFunction.cpp
    # ... other sources
)

add_library(gsFunction ${gsFunction_HEADERS} ${gsFunction_SOURCES})
target_link_libraries(gsFunction PUBLIC gsBase gsMath gsGeometry)
target_include_directories(gsFunction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
```

## Testing Strategy

### Circular Dependency Verification

**Automated Check:**
Create CMake function to detect circular dependencies:

```cmake
function(check_circular_deps)
    # Use CMake's dependency graph to verify no cycles
    # Fail build if circular dependencies detected
endfunction()
```

### Module Isolation Testing

**Unit Tests per Module:**
- Each module gets isolated unit tests
- Test module functionality without dependencies
- Integration tests verify module interactions

### Performance Validation

**Benchmarking:**
- Compare build times before/after refactoring
- Runtime performance regression testing
- Memory usage analysis

## Migration Timeline

**Total Estimated Duration: 12 weeks**

| Week | Phase | Deliverable |
|------|-------|-------------|
| 1-3 | Foundation | gsBase, gsMath, gsGeometry modules |
| 4-6 | Functions | gsBasis, gsFunction, gsTopology modules |
| 7-9 | Infrastructure | gsCompiler, refactored existing modules |
| 10-12 | Integration | Complete build system, tests, documentation |

## Risk Assessment

### Technical Risks

**High Risk:**
- Breaking existing user code during transition
- Performance regression from increased modularity
- Complex build system interactions

**Medium Risk:**
- Incomplete dependency mapping leading to missed dependencies
- Template instantiation issues across module boundaries

**Low Risk:**
- Documentation updates
- Optional module compatibility

### Mitigation Strategies

1. **Compatibility Headers**: Maintain old include paths during transition
2. **Incremental Migration**: Module-by-module approach with testing
3. **Performance Monitoring**: Continuous benchmarking during development
4. **Rollback Plan**: Keep original structure until validation complete

## Success Metrics

### Quantitative Goals

- **Zero circular dependencies** (verified by automated tools)
- **Build time improvement** (target: 10% faster)
- **Reduced header inclusion** (target: 20% fewer includes per module)
- **Test coverage maintenance** (maintain current coverage levels)

### Qualitative Goals

- Clean, logical module organization
- Improved developer experience
- Better documentation structure
- Easier new feature integration

## Conclusion

This architecture transformation will modernize GISMO's codebase by eliminating problematic circular dependencies and creating a clean, layered module structure. While modules will not be completely standalone, the new hierarchy provides clear dependency direction and better maintainability.

The 12-week development plan provides a systematic approach to implementing this transformation while maintaining backward compatibility and system stability throughout the migration process.

**Next Steps:**
1. Approve this architectural plan
2. Begin Phase 1 implementation with gsBase module creation
3. Establish automated dependency checking
4. Set up continuous integration for architecture validation

---

**Document Version**: 1.0
**Last Updated**: Analysis Date
**Status**: Proposed Architecture - Awaiting Implementation