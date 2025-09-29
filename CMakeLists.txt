# GISMO - Geometry + Simulation Modules
# Modern CMake Architecture Implementation

cmake_minimum_required(VERSION 3.20.0)

# Set policies for modern CMake
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)  # project() command manages VERSION
endif()
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)  # option() honors normal variables
endif()
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)  # find_package() uses <PackageName>_ROOT
endif()

# Project declaration with version
project(gismo
    VERSION 24.12.0
    LANGUAGES CXX
    DESCRIPTION "Geometry + Simulation Modules"
    HOMEPAGE_URL "http://gismo.github.io/"
)

# Modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directory standardization
include(GNUInstallDirs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Build type defaults
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Installation prefix handling
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH
        "Installation prefix" FORCE)
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

# Include GISMO-specific options and feature detection
include(cmake/GismoOptions.cmake)

# Create build directory structure
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/gismo/Core)

# Add source directory
add_subdirectory(src)

# Optional directories based on build options
if(GISMO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GISMO_BUILD_UNITTESTS)
    enable_testing()
    add_subdirectory(unittests)
endif()

# Handle optional external modules
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/optional" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/optional")
    message(STATUS "Found optional modules directory")
    # Optional modules are handled separately and can be included as needed
endif()

# Export configuration
include(cmake/gismoExport.cmake)

# Export package for use from build tree
export(PACKAGE ${PROJECT_NAME})

# Status summary
message(STATUS "")
message(STATUS "GISMO Configuration Summary:")
message(STATUS "  Version: ${gismo_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Components:")
message(STATUS "  Core: ${GISMO_WITH_CORE}")
message(STATUS "  Math: ${GISMO_WITH_MATH}")
message(STATUS "  Geometry: ${GISMO_WITH_GEOMETRY}")
message(STATUS "  Matrix: ${GISMO_WITH_MATRIX}")
message(STATUS "  Basis: ${GISMO_WITH_BASIS}")
message(STATUS "  Function: ${GISMO_WITH_FUNCTION}")
message(STATUS "  IO: ${GISMO_WITH_IO}")
message(STATUS "  Assembler: ${GISMO_WITH_ASSEMBLER}")
message(STATUS "")
message(STATUS "Optional modules:")
message(STATUS "  IETI: ${GISMO_WITH_IETI}")
message(STATUS "  Parallel: ${GISMO_WITH_PARALLEL}")
message(STATUS "  Compiler: ${GISMO_WITH_COMPILER}")
message(STATUS "")