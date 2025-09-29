# GISMO Modern CMake Architecture Proposal

## Overview

This proposal adapts the GISMO library architecture to follow modern C++ build system patterns as outlined in the Generic C++ Build Pattern. The new design transforms GISMO from its current object library approach to a target-based modular architecture with proper dependency management.

## Current Architecture Issues

### Problems with Current Build System
1. **Object Libraries**: Current use of `OBJECT` libraries limits modularity and proper dependency tracking
2. **Global Include Directories**: All modules share the same include scope, creating hidden dependencies
3. **Monolithic Build**: All modules compiled together, making selective builds impossible
4. **CMake Anti-patterns**: Uses deprecated approaches like global variables and directory-based includes
5. **Export Limitations**: Difficult to use GISMO as a proper CMake package

### Migration Benefits
- **Proper Target Dependencies**: Clear, explicit dependency relationships between modules
- **Selective Building**: Enable/disable modules as needed
- **Package Export**: Full CMake package support with `find_package(GISMO)`
- **Modern CMake**: Leverage CMake 3.20+ features for better tooling support
- **Performance**: Incremental builds and parallel compilation improvements

## Proposed Directory Structure

Following the Generic C++ Build Pattern, GISMO will be restructured as:

```
gismo/
├── CMakeLists.txt              # Root project configuration
├── cmake/
│   ├── gismoExport.cmake       # Export configuration
│   ├── gismoConfig.cmake.in    # Package config template
│   └── gismoOptions.cmake      # Build options and feature detection
└── src/
    ├── CMakeLists.txt          # Source delegation (minimal)
    └── gismo/                  # Main source namespace (NEW)
        ├── CMakeLists.txt      # Component orchestration
        ├── Core/               # Foundation layer (replaces gsCore base)
        ├── Math/               # Mathematical utilities
        ├── Geometry/           # Geometric primitives
        ├── Basis/              # Basis function definitions
        ├── Function/           # Function spaces
        ├── Topology/           # Mesh topology and boundaries
        ├── Compiler/           # JIT compilation
        ├── Matrix/             # Matrix operations
        ├── IO/                 # File I/O operations
        ├── Assembler/          # Assembly operations
        ├── Pde/                # PDE definitions
        ├── Solver/             # Linear/nonlinear solvers
        ├── Utils/              # High-level utilities
        ├── Nurbs/              # NURBS functionality
        ├── Modeling/           # Geometric modeling
        ├── HSplines/           # Hierarchical splines
        ├── MSplines/           # Multipatch splines
        ├── Optimizer/          # Optimization algorithms
        ├── MultiGrid/          # Multigrid methods
        ├── Domain/             # Domain definitions
        ├── Ieti/               # IETI methods (optional)
        ├── Tensor/             # Tensor operations
        ├── Parallel/           # Parallelization (optional)
        └── Mesh2/              # Mesh operations
```

## Component Architecture Design

### 1. Foundation Components (Layer 1 - No Dependencies)

#### gismo::Core
**Purpose**: Core types, memory management, forward declarations
**Type**: Interface Library (header-only)
**Files Migrated from gsCore**:
- `gsMemory.h` → `Core/gsMemory.h`
- `gsForwardDeclarations.h` → `Core/gsForwardDeclarations.h`
- `gsExport.h` → `Core/gsExport.h`

```cmake
# src/gismo/Core/CMakeLists.txt
project(Core VERSION ${gismo_VERSION} LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(gismo::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  INTERFACE
    FILE_SET HEADERS
      BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
      FILES
        Core/gsMemory.h
        Core/gsForwardDeclarations.h
        Core/gsExport.h
        Core/gsConfig.h
)

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Installation and export configuration...
```

#### gismo::Math
**Purpose**: Mathematical constants, basic mathematical operations
**Type**: Interface Library (header-only)
**Dependencies**: `gismo::Core`

### 2. Core Components (Layer 2)

#### gismo::Geometry
**Purpose**: Points, vectors, geometric primitives, transformations
**Type**: Shared Library
**Dependencies**: `gismo::Core`, `gismo::Math`
**Files**: Geometric operations requiring implementations

#### gismo::Matrix
**Purpose**: Matrix operations and linear algebra
**Type**: Shared Library
**Dependencies**: `gismo::Core`, `gismo::Math`
**External Dependencies**: Eigen3

```cmake
# src/gismo/Matrix/CMakeLists.txt
project(Matrix VERSION ${gismo_VERSION} LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED)
add_library(gismo::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  OUTPUT_NAME gismo${PROJECT_NAME}
  VERSION ${gismo_VERSION}
  SOVERSION ${gismo_VERSION_MAJOR}
)

generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/gismo${PROJECT_NAME}Export.h"
)

target_sources(${PROJECT_NAME}
  PRIVATE
    gsMatrix.cpp
    gsLinearAlgebra.cpp
    gsSparseMatrix.cpp
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${PROJECT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
      FILES
        ${PROJECT_BINARY_DIR}/gismo${PROJECT_NAME}Export.h
        Matrix/gsMatrix.h
        Matrix/gsLinearAlgebra.h
        Matrix/gsSparseMatrix.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    gismo::Core
    gismo::Math
    Eigen3::Eigen
)
```

### 3. Function Components (Layer 3)

#### gismo::Basis
**Purpose**: Basis function interfaces and definitions
**Type**: Shared Library
**Dependencies**: `gismo::Core`, `gismo::Math`, `gismo::Geometry`

#### gismo::Function
**Purpose**: Function spaces, evaluation, and function data
**Type**: Shared Library
**Dependencies**: `gismo::Core`, `gismo::Math`, `gismo::Geometry`, `gismo::Basis`

#### gismo::Topology
**Purpose**: Mesh topology, boundaries, connectivity
**Type**: Shared Library
**Dependencies**: `gismo::Core`, `gismo::Math`, `gismo::Geometry`

### 4. Infrastructure Components (Layer 4)

#### gismo::IO
**Purpose**: File I/O, XML parsing, data serialization
**Type**: Shared Library
**Dependencies**: `gismo::Core`, `gismo::Math`, `gismo::Geometry`, `gismo::Function`

#### gismo::Compiler
**Purpose**: Just-in-time compilation support
**Type**: Shared Library (Optional)
**Dependencies**: `gismo::Core`

### 5. Application Components (Layer 5+)

#### gismo::Assembler
**Purpose**: Assembly operations, DOF mapping
**Type**: Shared Library
**Dependencies**: `gismo::Core`, `gismo::Math`, `gismo::Matrix`, `gismo::Function`, `gismo::Basis`

#### gismo::Pde
**Purpose**: PDE definitions and problem setup
**Type**: Shared Library
**Dependencies**: `gismo::Core`, `gismo::Function`, `gismo::Assembler`

#### gismo::Solver
**Purpose**: Linear and nonlinear solvers
**Type**: Shared Library
**Dependencies**: `gismo::Core`, `gismo::Matrix`, `gismo::Assembler`

### 6. Feature Components (Layer 6)

All remaining modules (`Nurbs`, `Modeling`, `HSplines`, etc.) follow similar patterns, depending on appropriate lower-layer components.

## Root CMakeLists.txt Transformation

```cmake
cmake_minimum_required(VERSION 3.20.0)

project(gismo
  VERSION 24.12.0  # Update version scheme
  LANGUAGES CXX
  DESCRIPTION "Geometry + Simulation Modules"
  HOMEPAGE_URL "http://gismo.github.io/"
)

# Modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory standardization
include(GNUInstallDirs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Build type defaults
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Installation prefix handling
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install prefix" FORCE)
endif()

# RPATH configuration
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Enable compile commands export
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Modern CMake features
include(GenerateExportHeader)

# GISMO-specific options and dependencies
include(cmake/GismoOptions.cmake)

# Add source directory
add_subdirectory(src)

# Optional directories
if(GISMO_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(GISMO_BUILD_UNITTESTS)
  add_subdirectory(unittests)
endif()

# Export configuration
include(cmake/GismoExport.cmake)
export(PACKAGE ${PROJECT_NAME})
```

## Component Dependencies Matrix

| Component | Dependencies | Type | Purpose |
|-----------|--------------|------|---------|
| **Core** | None | Interface | Foundation types, memory |
| **Math** | Core | Interface | Mathematical utilities |
| **Geometry** | Core, Math | Shared | Geometric primitives |
| **Matrix** | Core, Math, Eigen3 | Shared | Linear algebra |
| **Basis** | Core, Math, Geometry | Shared | Basis functions |
| **Function** | Core, Math, Geometry, Basis | Shared | Function spaces |
| **Topology** | Core, Math, Geometry | Shared | Mesh topology |
| **IO** | Core, Math, Function | Shared | File I/O |
| **Compiler** | Core | Shared | JIT compilation |
| **Assembler** | Core, Math, Matrix, Function, Basis | Shared | Assembly operations |
| **Pde** | Core, Function, Assembler | Shared | PDE definitions |
| **Solver** | Core, Matrix, Assembler | Shared | Solvers |
| **Nurbs** | Core, Math, Geometry, Basis, Function | Shared | NURBS functionality |
| **Utils** | Core, Math, Matrix, IO | Shared | High-level utilities |

## Migration Implementation Plan

### Phase 1: Foundation Setup (Weeks 1-2)
1. **Create new directory structure** under `src/gismo/`
2. **Implement Core module** with core types and forward declarations
3. **Implement Math module** with mathematical utilities
4. **Update root CMakeLists.txt** with modern CMake patterns

### Phase 2: Core Infrastructure (Weeks 3-4)
1. **Implement Geometry module** with geometric primitives
2. **Implement Matrix module** with linear algebra operations
3. **Create proper CMake targets** with exports and installation
4. **Test basic build and installation**

### Phase 3: Function Layer (Weeks 5-6)
1. **Implement Basis module** with basis function interfaces
2. **Implement Function module** with function spaces
3. **Implement Topology module** with mesh topology
4. **Implement IO module** with file operations

### Phase 4: Application Layer (Weeks 7-9)
1. **Implement Assembler module** with assembly operations
2. **Implement Pde module** with PDE definitions
3. **Implement Solver module** with solver interfaces
4. **Test integration between application modules**

### Phase 5: Feature Modules (Weeks 10-11)
1. **Migrate remaining modules** (Nurbs, Modeling, etc.)
2. **Implement optional modules** (Ieti, Parallel, etc.)
3. **Create proper optional dependencies**

### Phase 6: Integration and Testing (Week 12)
1. **Comprehensive testing** of new build system
2. **Performance benchmarking** comparison
3. **Documentation updates**
4. **Create migration guide** for users

## Build System Features

### 1. Selective Module Building
```cmake
# Users can selectively enable/disable modules
option(GISMO_WITH_NURBS "Build NURBS module" ON)
option(GISMO_WITH_OPTIMIZER "Build Optimizer module" ON)
option(GISMO_WITH_IETI "Build IETI module" OFF)  # Optional by default
option(GISMO_WITH_PARALLEL "Build Parallel module" OFF)  # Optional by default
```

### 2. Package Configuration
Users can consume GISMO as a proper CMake package:

```cmake
# Consumer project
find_package(gismo REQUIRED COMPONENTS Core Math Matrix Assembler)

target_link_libraries(MyApp
  PRIVATE
    gismo::Core
    gismo::Math
    gismo::Matrix
    gismo::Assembler
)
```

### 3. Component-Specific Configuration
```cmake
# Advanced users can configure individual components
find_package(gismo REQUIRED)

if(TARGET gismo::Nurbs)
  target_link_libraries(MyApp PRIVATE gismo::Nurbs)
endif()

if(TARGET gismo::Optimizer)
  target_link_libraries(MyApp PRIVATE gismo::Optimizer)
endif()
```

## Backward Compatibility Strategy

### 1. Compatibility Headers
Create forwarding headers to maintain existing include paths during transition:

```cpp
// Legacy: gsCore/gsMemory.h
#ifndef GSCORE_GSMEMORY_H
#define GSCORE_GSMEMORY_H
#warning "gsCore/gsMemory.h is deprecated. Use gismo/Core/gsMemory.h instead"
#include <gismo/Core/gsMemory.h>
#endif
```

### 2. CMake Compatibility
Provide compatibility targets for existing build scripts:

```cmake
# Create legacy aliases for existing projects
add_library(gsCore INTERFACE)
target_link_libraries(gsCore INTERFACE
  gismo::Core
  gismo::Math
  gismo::Geometry
  gismo::Function
)
```

## Benefits of New Architecture

### 1. Development Experience
- **Clear Dependencies**: No hidden or circular dependencies
- **Faster Builds**: Incremental compilation and parallel builds
- **Better IDE Support**: Modern CMake provides better IntelliSense
- **Modular Development**: Work on individual components independently

### 2. Distribution and Packaging
- **Package Manager Support**: Compatible with Conan, vcpkg, etc.
- **Proper Exports**: Full CMake package configuration
- **Selective Installation**: Install only needed components
- **Version Management**: Per-component versioning support

### 3. Maintenance and Testing
- **Isolated Testing**: Test individual components separately
- **Clear Interfaces**: Well-defined component APIs
- **Dependency Tracking**: Automatic rebuilds when dependencies change
- **Documentation**: Self-documenting through CMake structure

## Risk Mitigation

### 1. Technical Risks
- **Breaking Changes**: Minimize through compatibility headers and gradual migration
- **Performance Impact**: Benchmark throughout migration process
- **Complex Dependencies**: Use automated dependency validation

### 2. Migration Risks
- **User Disruption**: Provide clear migration guides and compatibility layers
- **Build System Issues**: Extensive testing on multiple platforms
- **Timeline Overrun**: Incremental approach with working builds at each phase

## Success Metrics

### 1. Technical Metrics
- **Zero circular dependencies** (automated verification)
- **Build time improvement** (target: 15% faster clean builds)
- **Memory usage reduction** (reduced template instantiations)
- **Package manager compatibility** (successful Conan/vcpkg integration)

### 2. Developer Experience Metrics
- **Reduced build errors** from dependency issues
- **Improved IDE support** (better autocomplete, navigation)
- **Easier new component addition** (standardized CMake patterns)
- **Simpler maintenance** (clear component boundaries)

## Conclusion

This proposal transforms GISMO into a modern, modular C++ library following established best practices. The new architecture eliminates circular dependencies, provides proper CMake package support, and creates a foundation for sustainable long-term development.

The 12-week implementation plan provides a systematic approach to migration while maintaining backward compatibility and ensuring continuous integration throughout the process.

**Next Steps:**
1. Review and approve this architectural proposal
2. Set up development branch for architecture migration
3. Begin Phase 1 implementation with foundation modules
4. Establish automated testing and validation pipeline

---

**Document Version**: 1.0
**Based On**: Generic C++ Build Pattern v1.0
**Target CMake Version**: 3.20+
**Proposed Implementation**: Q1 2025