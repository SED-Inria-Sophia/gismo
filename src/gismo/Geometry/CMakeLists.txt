# Geometry Module CMakeLists.txt
# Geometric primitives and transformations

project(Geometry VERSION ${gismo_VERSION} LANGUAGES CXX)

# Create interface library for header-only geometry components
add_library(${PROJECT_NAME} INTERFACE)

# Add namespace alias
add_library(gismo::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Define geometry headers
target_sources(${PROJECT_NAME}
    INTERFACE
        FILE_SET HEADERS
            BASE_DIRS
                ${CMAKE_CURRENT_SOURCE_DIR}/..  # Points to src/gismo/
            FILES
                # Main module header (Generic C++ pattern)
                Geometry/Geometry
                # Geometric components
                Geometry/Point.h
                Geometry/Vector.h
                Geometry/PointVector.hpp
                Geometry/Transform.h
                Geometry/Primitives.h
)

# Include directories for build and install
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>  # Build: src/gismo/
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>   # Install: include/
)

# Dependencies - Geometry depends on Core and Math (Layer 2 → Layer 1)
target_link_libraries(${PROJECT_NAME}
    INTERFACE
        gismo::Core
        gismo::Math
)

# Compiler requirements
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

# Add compile definitions
target_compile_definitions(${PROJECT_NAME}
    INTERFACE
        GISMO_WITH_GEOMETRY
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
    APPEND FILE ${CMAKE_BINARY_DIR}/GismoGeometryTargets.cmake
)