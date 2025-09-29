# Math Module CMakeLists.txt
# Mathematical utilities, constants, and functions

project(Math VERSION ${gismo_VERSION} LANGUAGES CXX)

# Create interface library for header-only math components
add_library(${PROJECT_NAME} INTERFACE)

# Add namespace alias
add_library(gismo::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Define math headers
target_sources(${PROJECT_NAME}
    INTERFACE
        FILE_SET HEADERS
            BASE_DIRS
                ${CMAKE_CURRENT_SOURCE_DIR}/..  # Points to src/gismo/
            FILES
                # Main module header (Generic C++ pattern)
                Math/Math
                # Mathematical components
                Math/Constants.h
                Math/Functions.h
                Math/Utilities.h
)

# Include directories for build and install
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>  # Build: src/gismo/
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>   # Install: include/
)

# Dependencies - Math depends on Core (Layer 1 → Layer 1)
target_link_libraries(${PROJECT_NAME}
    INTERFACE
        gismo::Core
)

# Compiler requirements
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

# Add compile definitions
target_compile_definitions(${PROJECT_NAME}
    INTERFACE
        GISMO_WITH_MATH
)

# Installation
install(
    TARGETS ${PROJECT_NAME}
    EXPORT gismo-targets
    FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gismo
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export for build tree
export(
    TARGETS ${PROJECT_NAME}
    NAMESPACE gismo::
    APPEND FILE ${CMAKE_BINARY_DIR}/GismoMathTargets.cmake
)