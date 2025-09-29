# GismoOptions.cmake
# GISMO-specific build options and feature detection

# Build options
option(GISMO_BUILD_LIB "Build G+Smo as a library" ON)
option(GISMO_BUILD_SHARED "Build G+Smo as shared library" OFF)
option(GISMO_BUILD_STATIC "Build G+Smo as static library" ON)
option(GISMO_BUILD_EXAMPLES "Build G+Smo examples" ON)
option(GISMO_BUILD_UNITTESTS "Build G+Smo unit tests" OFF)
option(GISMO_BUILD_PCH "Build with precompiled headers" OFF)

# Component options
option(GISMO_WITH_COMMON "Build Common module (required)" ON)
option(GISMO_WITH_MATH "Build Math module" ON)
option(GISMO_WITH_GEOMETRY "Build Geometry module" ON)
option(GISMO_WITH_MATRIX "Build Matrix module" ON)
option(GISMO_WITH_BASIS "Build Basis module" ON)
option(GISMO_WITH_FUNCTION "Build Function module" ON)
option(GISMO_WITH_TOPOLOGY "Build Topology module" ON)
option(GISMO_WITH_IO "Build IO module" ON)
option(GISMO_WITH_ASSEMBLER "Build Assembler module" ON)
option(GISMO_WITH_PDE "Build PDE module" ON)
option(GISMO_WITH_SOLVER "Build Solver module" ON)
option(GISMO_WITH_NURBS "Build NURBS module" ON)
option(GISMO_WITH_UTILS "Build Utils module" ON)
option(GISMO_WITH_MODELING "Build Modeling module" ON)
option(GISMO_WITH_HSPLINES "Build HSplines module" ON)
option(GISMO_WITH_MSPLINES "Build MSplines module" ON)
option(GISMO_WITH_OPTIMIZER "Build Optimizer module" ON)
option(GISMO_WITH_MULTIGRID "Build MultiGrid module" ON)
option(GISMO_WITH_DOMAIN "Build Domain module" ON)
option(GISMO_WITH_TENSOR "Build Tensor module" ON)
option(GISMO_WITH_MESH2 "Build Mesh2 module" ON)

# Optional modules (disabled by default)
option(GISMO_WITH_IETI "Build IETI module" OFF)
option(GISMO_WITH_PARALLEL "Build Parallel module" OFF)
option(GISMO_WITH_COMPILER "Build JIT Compiler module" OFF)

# External library detection
find_package(Eigen3 REQUIRED)
if(NOT TARGET Eigen3::Eigen)
    # Fallback for older Eigen3 versions
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    set_target_properties(Eigen3::Eigen PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${EIGEN3_INCLUDE_DIR}")
endif()

# Optional external libraries
find_package(OpenMP)
if(OpenMP_CXX_FOUND AND GISMO_WITH_PARALLEL)
    set(GISMO_HAS_OPENMP TRUE)
else()
    set(GISMO_HAS_OPENMP FALSE)
endif()

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(GISMO_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(GISMO_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(GISMO_CXX_FLAGS "/W3")
endif()

# Feature detection
include(CheckCXXCompilerFlag)
if(COMMAND check_cxx_compiler_flag)
    check_cxx_compiler_flag("-std=c++17" GISMO_HAS_CXX17)
    set(GISMO_HAS_CONSTEXPR TRUE)  # C++17 has constexpr
    set(GISMO_HAS_AUTO TRUE)       # C++17 has auto
endif()

# Generate config header
# configure_file(
#     "${CMAKE_CURRENT_LIST_DIR}/gsConfig.h.in"
#     "${CMAKE_BINARY_DIR}/src/gismo/Core/Config.h"
#     @ONLY
# )
