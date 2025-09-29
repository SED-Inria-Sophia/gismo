# Generic C++ Build System Pattern

## Overview

This document presents a generic, reusable pattern for structuring C++ projects with CMake, extracted from successful real-world implementations. The pattern emphasizes modularity, modern CMake practices, and maintainability.

## Directory Structure Template

```
project-root/
├── CMakeLists.txt              # Root project configuration
├── cmake/
│   ├── ProjectExport.cmake     # Export configuration
│   └── ProjectConfig.cmake.in  # Package config template
└── src/
    ├── CMakeLists.txt          # Source delegation (minimal)
    └── project-name/           # Main source namespace
        ├── CMakeLists.txt      # Component orchestration
        ├── Component1/         # Foundational component
        ├── Component2/         # Header-only utilities
        ├── Component3/         # Feature component
        │   ├── SubModule1/     # Optional: sub-components
        │   └── SubModule2/     # Optional: sub-components
        └── Component4/         # Top-level feature
```

## Core CMake Patterns

### 1. Root Project Configuration

```cmake
cmake_minimum_required(VERSION 3.27.0)

project(ProjectName
  VERSION 1.0.0
  LANGUAGES CXX)

# Modern C++ standard
set(CMAKE_CXX_STANDARD 20)
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

# RPATH configuration for runtime library location
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Enable compile commands export for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Modern CMake features
include(GenerateExportHeader)

# Add source directory
add_subdirectory(src)

# Export configuration
include(ProjectExport)
export(PACKAGE ${PROJECT_NAME})
```

### 2. Component Template Pattern

Each component follows this standardized pattern:

```cmake
### Component CMakeLists.txt Template

project(ComponentName
VERSION
  ${${CMAKE_PROJECT_NAME}_VERSION}
LANGUAGES
  CXX)

## Create Library Target
# Choose ONE of these based on component type:

# Option A: Shared Library (most common)
add_library(${PROJECT_NAME} SHARED)

# Option B: Interface Library (header-only)
add_library(${PROJECT_NAME} INTERFACE)

# Option C: Static Library (if needed)
add_library(${PROJECT_NAME} STATIC)

## Target Properties
set_target_properties(${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME ${CMAKE_PROJECT_NAME}${PROJECT_NAME}
    VERSION     ${${PROJECT_NAME}_VERSION}
    SOVERSION   ${${PROJECT_NAME}_VERSION_MAJOR})

## Export Header (for shared/static libraries only)
generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}${PROJECT_NAME}Export.h")

## Namespace Alias
add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

## Source Files Management
target_sources(${PROJECT_NAME}
  PRIVATE
    # Implementation files (.cpp)
    Implementation1.cpp
    Implementation2.cpp

  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${PROJECT_BINARY_DIR}        # For generated headers
        ${CMAKE_CURRENT_SOURCE_DIR}  # For source headers
      FILES
        # Generated export header (if applicable)
        ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}${PROJECT_NAME}Export.h
        # Public headers
        PublicHeader1.h
        PublicHeader2.h
        # Directory-style header (optional)
        ${PROJECT_NAME}
)

## Include Directories
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>  # Build: src/ directory
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>     # Install: include/ directory
)

## Dependencies
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_PROJECT_NAME}::PrivateDependency
  PUBLIC
    ${CMAKE_PROJECT_NAME}::PublicDependency
    # External libraries as needed
)

## Sub-components (if applicable)
add_subdirectory(SubModule1)
add_subdirectory(SubModule2)

## Installation
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${CMAKE_PROJECT_NAME}${PROJECT_NAME}-targets
  LIBRARY     DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE     DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME     DESTINATION ${CMAKE_INSTALL_BINDIR}
  FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/${PROJECT_NAME}
  INCLUDES    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT      ${CMAKE_PROJECT_NAME}${PROJECT_NAME}-targets
  FILE        ${CMAKE_PROJECT_NAME}${PROJECT_NAME}Targets.cmake
  NAMESPACE   ${CMAKE_PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
)

export(
  EXPORT    ${CMAKE_PROJECT_NAME}${PROJECT_NAME}-targets
  NAMESPACE ${CMAKE_PROJECT_NAME}::
  FILE      ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}${PROJECT_NAME}Targets.cmake
)
```

### 3. Sub-component Pattern

For components with sub-modules:

```cmake
### Sub-component CMakeLists.txt Template

# No project() declaration - contributes to parent

target_sources(${PROJECT_NAME}  # References parent project name
  PRIVATE
    # Sub-component implementation
    SubImplementation.cpp

  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ..  # Relative to parent component directory
      FILES
        SubHeader.h
        SubUtility.h
)
```

## Naming Convention Standards

### 1. Project Structure
- **Root Project**: `PascalCase` or `kebab-case` (e.g., `MyProject`, `my-project`)
- **Components**: `PascalCase` matching directory names (e.g., `Core`, `Utilities`, `Graphics`)
- **Sub-components**: `PascalCase` in subdirectories (e.g., `Rendering`, `Physics`)

### 2. Target Names
- **Library Targets**: `${CMAKE_PROJECT_NAME}${COMPONENT_NAME}` (e.g., `MyProjectCore`)
- **Alias Names**: `${CMAKE_PROJECT_NAME}::${COMPONENT_NAME}` (e.g., `MyProject::Core`)
- **Export Names**: `${CMAKE_PROJECT_NAME}${COMPONENT_NAME}Export.h`

### 3. File Names
- **Headers**: `PascalCase.h` or `snake_case.h` (be consistent)
- **Sources**: Match header names with `.cpp` extension
- **CMake Files**: `PascalCase` for projects, `snake_case` for utilities

## Dependency Management Patterns

### 1. Dependency Hierarchy Design
```
Foundation Layer (no dependencies)
 ↓
Core Layer (depends on Foundation)
 ↓
Feature Layers (depend on Core, may depend on each other)
 ↓
Application Layer (depends on Features)
```

### 2. Public vs Private Dependencies
- **Public**: Dependencies that consumers need to know about
- **Private**: Internal implementation dependencies
- **Interface**: Dependencies for header-only libraries

```cmake
target_link_libraries(MyComponent
  PUBLIC
    MyProject::Core          # Consumers need this
    SomeExternalLibrary::API # Exposed in public headers
  PRIVATE
    MyProject::Internal      # Implementation detail
    SomeExternalLibrary::Impl # Not exposed to consumers
)
```

## Component Type Guidelines

### 1. Interface Libraries (Header-Only)
**When to use:**
- Template-heavy code
- Utility functions that can be inlined
- Configuration and constants
- Logging frameworks

**Pattern:**
```cmake
add_library(ComponentName INTERFACE)
# No PRIVATE sources
# All headers in PUBLIC FILE_SET
# No export header generation
```

### 2. Shared Libraries
**When to use:**
- Large implementations
- Plugin architectures
- Reduce compile times
- Runtime library updates needed

**Pattern:**
```cmake
add_library(ComponentName SHARED)
# Has PRIVATE .cpp sources
# Generate export header
# Standard installation
```

### 3. Static Libraries
**When to use:**
- Better optimization opportunities
- Simpler deployment
- No ABI compatibility concerns

**Pattern:**
```cmake
add_library(ComponentName STATIC)
# Similar to shared but no SOVERSION
```

## Modern CMake Features Used

### 1. FILE_SET HEADERS (CMake 3.23+)
- Proper header dependency tracking
- Automatic installation of public headers
- Better IDE support

### 2. Generator Expressions
- `$<BUILD_INTERFACE:...>` vs `$<INSTALL_INTERFACE:...>`
- Conditional compilation flags
- Platform-specific settings

### 3. Target-Based Design
- No global variables for dependencies
- Transitive property propagation
- Clean consumer interface

## Package Configuration Pattern

### ProjectExport.cmake Template
```cmake
include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
INSTALL_DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

write_basic_package_version_file(
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
VERSION
  ${${PROJECT_NAME}_VERSION}
COMPATIBILITY
  AnyNewerVersion)

install(
  FILES
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
```

### ProjectConfig.cmake.in Template
```cmake
@PACKAGE_INIT@

include(CMakeFindDependencyMacro)

# Find external dependencies
# find_dependency(SomeExternalLibrary REQUIRED)

# Include component targets
include("${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@Component1Targets.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@Component2Targets.cmake")
# ... include all component targets

check_required_components(@PROJECT_NAME@)
```

## Best Practices Summary

### 1. Project Organization
- ✅ One component per directory
- ✅ Clear dependency hierarchy
- ✅ Consistent naming throughout
- ✅ Separate public/private interfaces

### 2. CMake Structure
- ✅ Use modern CMake features (3.20+)
- ✅ Target-based design (no global variables)
- ✅ Proper export/import for consumers
- ✅ Generator expressions for build vs install

### 3. Scalability
- ✅ Easy to add new components
- ✅ Clear patterns to follow
- ✅ Supports both monolithic and modular builds
- ✅ Works with package managers

### 4. Maintainability
- ✅ Minimal duplication between components
- ✅ Self-documenting structure
- ✅ Clear separation of concerns
- ✅ Consistent error handling

## Getting Started Checklist

1. **Setup Root Project**
   - [ ] Configure CMakeLists.txt with project info
   - [ ] Set C++ standard and compiler requirements
   - [ ] Configure output directories
   - [ ] Setup RPATH and installation

2. **Create Component Structure**
   - [ ] Design dependency hierarchy
   - [ ] Create component directories
   - [ ] Choose library types (shared/static/interface)
   - [ ] Implement component CMakeLists.txt files

3. **Configure Dependencies**
   - [ ] Link components with proper PUBLIC/PRIVATE
   - [ ] Setup include directories
   - [ ] Handle external dependencies

4. **Setup Installation**
   - [ ] Configure export headers
   - [ ] Setup target exports
   - [ ] Create package configuration
   - [ ] Test find_package() usage

5. **Validate Build System**
   - [ ] Test clean builds
   - [ ] Verify installation works
   - [ ] Test as imported package
   - [ ] Check with different build types

---

*This pattern provides a solid foundation for C++ projects of any size, from small libraries to large applications with multiple components.*