# GISMO Component Orchestration CMakeLists.txt
# This file manages all GISMO components following modern CMake patterns

# Foundation layer - zero dependencies
add_subdirectory(Common)

# Mathematics layer - depends on Common
# add_subdirectory(Math)

# Example module - commented out as it's just an empty example
# add_subdirectory(Module1)
