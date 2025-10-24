# GISMO Component Orchestration CMakeLists.txt
# This file manages all GISMO components following modern CMake patterns

# Foundation layer - zero dependencies
add_subdirectory(Common)

# Mathematics layer - depends on Common
add_subdirectory(Math)

# Core module - unified core functionality
add_subdirectory(Core)

# IO module - unified input/output functionality
add_subdirectory(IO)

# JIT module - just-in-time compilation (depends on Core, IO)
add_subdirectory(JIT)

# Example module - commented out as it's just an empty example
# add_subdirectory(Module1)
