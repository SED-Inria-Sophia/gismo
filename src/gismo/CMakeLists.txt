# GISMO Component Orchestration CMakeLists.txt
# This file manages all GISMO components following modern CMake patterns

# Foundation layer - zero dependencies
add_subdirectory(Common)

# Mathematics layer - depends on Common
add_subdirectory(Math)

# Core module - unified core functionality
add_subdirectory(Core)

# XmlIO module - optional XML serialization (depends on Core)
if(GISMO_WITH_XML_SERIALIZATION)
    add_subdirectory(XmlIO)
endif()

# Example module - commented out as it's just an empty example
# add_subdirectory(Module1)
