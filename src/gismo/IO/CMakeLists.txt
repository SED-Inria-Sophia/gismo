project(IO
VERSION
  ${${CMAKE_PROJECT_NAME}_VERSION}
LANGUAGES
  CXX)

## #################################################################
## External dependencies
## #################################################################

# Find RapidXML for XML functionality
find_path(RAPIDXML_INCLUDE_DIR
    NAMES rapidxml.hpp
    PATHS ${CMAKE_SOURCE_DIR}/external/rapidxml
    DOC "RapidXML include directory"
)

# Find TCLAP for command line parsing
find_path(TCLAP_INCLUDE_DIR
    NAMES tclap/CmdLine.h
    PATHS ${CMAKE_SOURCE_DIR}/external/tclap/include
    DOC "TCLAP include directory"
)

# Check if all required dependencies are available
if(NOT RAPIDXML_INCLUDE_DIR OR NOT TCLAP_INCLUDE_DIR)
    message(STATUS "IO module: Missing dependencies")
    if(NOT RAPIDXML_INCLUDE_DIR)
        message(STATUS "  Missing: RapidXML (required for XML parsing)")
    endif()
    if(NOT TCLAP_INCLUDE_DIR)
        message(STATUS "  Missing: TCLAP (required for command line parsing)")
    endif()
    message(STATUS "  IO module will not be built")
    return()
endif()

# Create modern IMPORTED targets for external dependencies
if(NOT TARGET RapidXML::RapidXML)
    add_library(RapidXML::RapidXML INTERFACE IMPORTED)
    target_include_directories(RapidXML::RapidXML INTERFACE
        ${RAPIDXML_INCLUDE_DIR}
    )
endif()

if(NOT TARGET TCLAP::TCLAP)
    add_library(TCLAP::TCLAP INTERFACE IMPORTED)
    target_include_directories(TCLAP::TCLAP INTERFACE
        ${TCLAP_INCLUDE_DIR}
    )
endif()

message(STATUS "IO module: All dependencies found")
message(STATUS "  RapidXML: ${RAPIDXML_INCLUDE_DIR}")
message(STATUS "  TCLAP: ${TCLAP_INCLUDE_DIR}")

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
        XmlUtils.h
        XmlUtils.hpp
        XmlOptionList.h
        XmlGenericUtils.hpp
        XmlComposedBasis.hpp
        XmlComposedFunction.hpp
        XmlComposedGeometry.hpp
        XmlConstantFunction.hpp
        XmlFunctionExpr.hpp
        XmlMultiBasis.hpp
  PRIVATE
    Xml.cpp
    XmlInstance.cpp
    XmlOptionList.cpp
    XmlUtils.cpp
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
  PRIVATE
    Paraview/ParaviewCollection.cpp
    Paraview/ParaviewDataSet.cpp
    Paraview/ParaviewUtils.cpp
    Paraview/ParaviewUtils_.cpp
    Paraview/WriteParaview.cpp
    Paraview/WriteParaview_.cpp
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