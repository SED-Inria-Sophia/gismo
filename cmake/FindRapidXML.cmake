# - Find RapidXML
# Find the RapidXML includes
# This module defines:
#   RAPIDXML_FOUND         - True if RapidXML was found.
#   RAPIDXML_INCLUDE_DIRS  - Include directories for RapidXML.

find_path(RapidXML_INCLUDE_DIR
    NAMES
        rapidxml/rapidxml.hpp
        rapidxml.hpp
    HINTS
        "${CMAKE_INSTALL_PREFIX}/include"
        "${CMAKE_PREFIX_PATH}/include"
        "$ENV{CONDA_PREFIX}/include"
        "$ENV{PREFIX}/include"
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
        ${CMAKE_SOURCE_DIR}/external/rapidxml
    DOC "RapidXML include directory"
)

# Handle the REQUIRED and QUIETLY arguments using CMake's standard function.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RapidXML
    REQUIRED_VARS RapidXML_INCLUDE_DIR
    FAIL_MESSAGE "Could NOT find RapidXML (missing: ${RapidXML_INCLUDE_DIR})"
)

# If the package is found, set up include directories and create imported target.
if(RapidXML_FOUND)
    set(RAPIDXML_INCLUDE_DIRS ${RapidXML_INCLUDE_DIR})

    # Create imported target
    if(NOT TARGET RapidXML::RapidXML)
        add_library(RapidXML::RapidXML INTERFACE IMPORTED)
        set_target_properties(RapidXML::RapidXML PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${RapidXML_INCLUDE_DIR}"
        )
    endif()

    message(STATUS "Found RapidXML: Include dir: ${RapidXML_INCLUDE_DIR}")
endif()

# Set variables for backward compatibility
if(RapidXML_FOUND)
    set(RAPIDXML_LIBRARIES "")  # Header-only library
    set(RAPIDXML_INCLUDE_DIRS ${RapidXML_INCLUDE_DIR})
endif()

