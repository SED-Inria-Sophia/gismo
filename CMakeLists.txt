# GISMO - Geometry + Simulation Modules
# Simplified CMakeLists.txt for modern architecture testing

cmake_minimum_required(VERSION 3.20)

project(gismo
    VERSION 25.7.0
    LANGUAGES CXX
    DESCRIPTION "Geometry + Simulation Modules"
)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Find Eigen3 (required for Common module)
find_package(Eigen3 REQUIRED)

# Add source directory
add_subdirectory(src)

# Add test directory
enable_testing()
add_subdirectory(tst)

# Status message
message(STATUS "GISMO simplified build configured")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
