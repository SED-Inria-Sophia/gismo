# - Find TCLAP
# Find the TCLAP (Templatized Command Line Argument Parser) includes
# This module defines:
#   TCLAP_FOUND         - True if TCLAP was found.
#   TCLAP_INCLUDE_DIRS  - Include directories for TCLAP.
#   TCLAP::TCLAP        - Imported target for TCLAP (if found).

find_path(TCLAP_INCLUDE_DIR
    NAMES tclap/CmdLine.h
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
        ${CMAKE_SOURCE_DIR}/external/tclap/include
    DOC "TCLAP include directory"
)

# Handle the REQUIRED and QUIETLY arguments using CMake's standard function.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TCLAP
    REQUIRED_VARS TCLAP_INCLUDE_DIR
    FAIL_MESSAGE "Could NOT find TCLAP (missing: TCLAP_INCLUDE_DIR)"
)

# If the package is found, set up include directories and create imported target.
if(TCLAP_FOUND)
    set(TCLAP_INCLUDE_DIRS ${TCLAP_INCLUDE_DIR})

    # Create imported target
    if(NOT TARGET TCLAP::TCLAP)
        add_library(TCLAP::TCLAP INTERFACE IMPORTED)
        set_target_properties(TCLAP::TCLAP PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${TCLAP_INCLUDE_DIR}"
        )
    endif()

    message(STATUS "Found TCLAP: Include dir: ${TCLAP_INCLUDE_DIR}")
endif()

# Set variables for backward compatibility
if(TCLAP_FOUND)
    set(TCLAP_LIBRARIES "")  # Header-only library
    set(TCLAP_INCLUDE_DIRS ${TCLAP_INCLUDE_DIR})
endif()