project(IO
VERSION
  ${${CMAKE_PROJECT_NAME}_VERSION}
LANGUAGES
  CXX)

## #################################################################
## External dependencies
## #################################################################

# Use modern CMake modules to find dependencies
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find RapidXML for XML functionality
find_package(RapidXML REQUIRED)

# Find TCLAP for command line parsing
find_package(TCLAP REQUIRED)

# Find zlib for compression support
find_package(ZLIB REQUIRED)

message(STATUS "IO module: All dependencies found")
if(RAPIDXML_INCLUDE_DIRS)
    message(STATUS "  RapidXML: ${RAPIDXML_INCLUDE_DIRS}")
elseif(RapidXML_INCLUDE_DIR)
    message(STATUS "  RapidXML: ${RapidXML_INCLUDE_DIR}")
endif()
message(STATUS "  TCLAP: ${TCLAP_INCLUDE_DIRS}")

## #################################################################
## Create target
## #################################################################

add_library(${PROJECT_NAME} SHARED)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME ${CMAKE_PROJECT_NAME}${PROJECT_NAME}
    VERSION   ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


## #################################################################
## Collect sources from all functionality areas
## #################################################################

# Core I/O functionality
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        FileManager.h
        CommandLine.h
        OptionList.h
        Csv.h
  PRIVATE
    FileManager.cpp
    CommandLine.cpp
    OptionList.cpp
)

# XML functionality
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Xml.h
        Xml.hpp
        # TODO: Uncomment when Core modules are migrated
        # XmlUtils.h
        # XmlUtils.hpp
        XmlGenericUtils.hpp
        XmlComposedBasis.hpp
        XmlComposedFunction.hpp
        XmlComposedGeometry.hpp
        XmlConstantFunction.hpp
        XmlFunctionExpr.hpp
  PRIVATE
    Xml.cpp
    XmlInstance.cpp
    # TODO: Uncomment when Core modules are migrated
    # XmlUtils.cpp
)

# High-level file I/O
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        FileData.h
        FileData.hpp
        ReadFile.h
  PRIVATE
    FileData_.cpp
    ReadFile_.cpp
)

# Paraview visualization
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Paraview/Paraview
        Paraview/ParaviewCollection.h
        Paraview/ParaviewDataSet.h
        Paraview/ParaviewUtils.h
        Paraview/ParaviewUtils.hpp
        Paraview/WriteParaview.h
        Paraview/WriteParaview.hpp
#   PRIVATE
#     Paraview/ParaviewCollection.cpp
#     Paraview/ParaviewDataSet.cpp
#     Paraview/ParaviewUtils.cpp
#     Paraview/ParaviewUtils_.cpp
#     Paraview/WriteParaview.cpp
#     Paraview/WriteParaview_.cpp
)

# Compression functionality
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        gzstreamer/gzstream.h
  PRIVATE
    gzstreamer/gzstream.cpp
)

# Main convenience header
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        IO
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_PROJECT_NAME}::Common
    ${CMAKE_PROJECT_NAME}::Math
    ${CMAKE_PROJECT_NAME}::Core
    RapidXML::RapidXML
    TCLAP::TCLAP
  PRIVATE
    ZLIB::ZLIB  # For gzstreamer compression support
)

## ###################################################################
## Install rules
## ###################################################################

install(
  TARGETS
    ${PROJECT_NAME}
  EXPORT
    ${CMAKE_PROJECT_NAME}${PROJECT_NAME}-targets
  LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  FILE_SET HEADERS
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/${PROJECT_NAME}
  INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
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

######################################################################
### CMakeLists.txt ends here