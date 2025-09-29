# GISMO Component Orchestration CMakeLists.txt
# This file manages all GISMO components following modern CMake patterns

# Check that we're building Core (required)
if(NOT GISMO_WITH_CORE)
    message(FATAL_ERROR "GISMO Core module is required and cannot be disabled")
endif()

# Foundation Layer - Layer 1 (no dependencies)
message(STATUS "Building GISMO Foundation Layer...")

# Core module (required)
add_subdirectory(Core)

# # Math module
# if(GISMO_WITH_MATH)
#     add_subdirectory(Math)
# endif()

# # Infrastructure Layer - Layer 2 (depends on Foundation)
# message(STATUS "Building GISMO Infrastructure Layer...")

# # Geometry module
# if(GISMO_WITH_GEOMETRY)
#     add_subdirectory(Geometry)
# endif()

# Matrix module (TODO: implement)
#if(GISMO_WITH_MATRIX)
#    add_subdirectory(Matrix)
#endif()

# Function Layer - Layer 3 (depends on Foundation + Infrastructure)
#message(STATUS "Building GISMO Function Layer...")

# All other modules commented out until implementation
# TODO: Implement modules one by one following the Core module pattern
