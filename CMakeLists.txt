# GISMO - Geometry + Simulation Modules
# Modern CMake Architecture Implementation

cmake_minimum_required(VERSION 3.20.0)

if(CMAKE_VERSION VERSION_LESS "3.19")
  cmake_minimum_required(VERSION 2.8.12)
else()
  cmake_minimum_required(VERSION 3.1...3.10)
endif()

if(POLICY CMP0048)# CMake 3.0
cmake_policy(SET CMP0011 NEW)
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0048 NEW)
endif()

# Project declaration with version
project(gismo
    VERSION 24.12.0
    LANGUAGES CXX
    DESCRIPTION "Geometry + Simulation Modules"
    HOMEPAGE_URL "http://gismo.github.io/"
)

# Modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directory standardization
include(GNUInstallDirs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Build type defaults
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Installation prefix handling
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH
        "Installation prefix" FORCE)
endif()

# RPATH configuration for runtime library location
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Enable compile commands export for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Modern CMake features
include(GenerateExportHeader)

# Include GISMO-specific options and feature detection
include(cmake/GismoOptions.cmake)

set(CMAKE_BUILD_TYPE_INIT Release)
if(NOT CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "" FORCE)
endif()
if(NOT CMAKE_BUILD_TYPE)
   # Set default build type to Release
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "RelWithDebInfo" "MinSizeRel")
endif()

set(gismo_VERSION_MAJOR 25) #year
set(gismo_VERSION_MINOR 07) #month
set(gismo_VERSION_PATCH 0 ) #patch
set(gismo_VERSION
  "${gismo_VERSION_MAJOR}.${gismo_VERSION_MINOR}.${gismo_VERSION_PATCH}" CACHE INTERNAL "gismo version number")
set(GISMO_VERSION "${gismo_VERSION}" CACHE INTERNAL "G+Smo version number")

if(CMAKE_QUIET)
  function(message)
    list(GET ARGV 0 MessageType)
    if(MessageType STREQUAL FATAL_ERROR OR
	MessageType STREQUAL SEND_ERROR OR
	MessageType STREQUAL WARNING OR
	MessageType STREQUAL AUTHOR_WARNING)
      list(REMOVE_AT ARGV 0)
      _message(${MessageType} "${ARGV}")
    endif()
  endfunction()
  unset(CMAKE_QUIET CACHE)
endif()

if(POLICY CMP0048)# CMake 3.0
  project(gismo LANGUAGES C CXX  VERSION ${gismo_VERSION})
else()
  project(gismo C CXX)
endif()

if (GISMO_BUILD_PCH)
  enable_language(CXXPCH)
endif()

if(NOT GISMO_INSOURCE_BUILD AND "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
   message(FATAL_ERROR "In-source G+Smo builds are not permitted. \nHow to fix this:\n1. Type \nmake cleancmake\nto remove garbage CMake files.\n2. Please make a separate folder for building, otherwise typing \nmake\nthat will create a ./build folder and will compile in that folder. ")
endif()

## #################################################################
## Cmake Inclusions and configuration options
## #################################################################

#Fix for Eigen directory
if (NOT WIN32 AND NOT EXISTS "${PROJECT_BINARY_DIR}/Eigen")
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/external/gsEigen ${PROJECT_BINARY_DIR}/Eigen)
endif()

set(CMAKE_INSTALL_MESSAGE NEVER)

#Use solution folders for Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Remove NDEBUG flag from RelWithDebInfo builds
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()

include( gismoUse )
include( gsConfig )

message( "   GGGGGGGGG      GGGG      GGGGGGGGG  GGGGGG   GGGGGG  GGGGGGGGGG" )
message( "  GGGG            GGGG     GGGG        GGGGGG  GGGGGG  GGGG   GGGG" )
message( " GGGG         GGGGGGGGGGGG GGGGGGGGG   G GGGG  G GGGG GGGG    GGGG" )
message( " GGGG GGGGGG GGGGGGGGGGGGG GGGGGGGGGG GG GGGG GG GGGG GGGG   GGGGG" )
message( "GGGGG  GGGGG GGGGGGGGGGGG  GGGGGGGGG  GG GGGGGG GGGG  GGGG   GGGG " )
message( "GGGG   GGGG      GGGG           GGGG  GG  GGGG  GGGG  GGGG   GGGG " )
message( " GGGGGGGGGG      GGGG     GGGGGGGGG  GG   GGG   GGGG  GGGGGGGGGG  " )
message( "                          version ${${PROJECT_NAME}_VERSION}" )

## #################################################################
## Project options
## #################################################################

include( gsOptions )
include(CTest)
# Note: clean cache to reset to OFF
set(BUILD_TESTING "Enable testing" ${GISMO_BUILD_UNITTESTS})
file(WRITE "${PROJECT_BINARY_DIR}/CTestCustom.cmake" "set(CTEST_SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR})\nset(CTEST_BINARY_DIRECTORY   ${PROJECT_BINARY_DIR})\nset(CTEST_SITE         ${SITE})\nset(CTEST_BUILD_NAME   ${BUILDNAME})\n")

include(submodules.txt)
set(GISMO_OPTIONAL ${SUBMODULES_TXT} ${GISMO_OPTIONAL} CACHE STRING "List of optional G+Smo modules" FORCE)
if (CMAKE_VERSION VERSION_GREATER 3.11.5)
  list(TRANSFORM GISMO_OPTIONAL STRIP)
endif()
set(GISMO_OPTIONAL ${GISMO_OPTIONAL}
  CACHE STRING "List of optional G+Smo modules" FORCE)
#message(STATUS "Submodules ${GISMO_OPTIONAL}")

## #################################################################
## Preprocessor definitions
## #################################################################

# Data directory
include ( gsDataDir )

## #################################################################
## JIT compiler
## #################################################################

include(gsJITConfigXml)
gsJITConfigXml(${CMAKE_CURRENT_SOURCE_DIR}/cmake/jit.xml.in
  ${CMAKE_BINARY_DIR}/config/jit.xml)

## #################################################################
## Collect headers and sources
## #################################################################

# Clean previously set variables in the cache
unset(GISMO_EXTRA_INSTANCE       CACHE)
unset(GISMO_EXTERNALS            CACHE)
unset(${PROJECT_NAME}_HEADERS    CACHE)
unset(${PROJECT_NAME}_SOURCES    CACHE)
unset(${PROJECT_NAME}_EXTENSIONS CACHE)
unset(${PROJECT_NAME}_MODULES    CACHE)
unset(${PROJECT_NAME}_optionals  CACHE)
unset(${PROJECT_NAME}_LINKER     CACHE)
unset(gismo_TMPL_HEADERS         CACHE)
unset(gismo_UNITTESTS            CACHE)
set(${PROJECT_NAME}_optionals ""
    CACHE INTERNAL "${PROJECT_NAME} list of optionals")

set (GISMO_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/external"
     "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/optional"
     CACHE INTERNAL "${PROJECT_NAME} include directories" )

# Offer the user the choice of overriding the installation directories
set(LIB_INSTALL_DIR     lib     CACHE STRING "Installation directory for libraries")
set(BIN_INSTALL_DIR     bin     CACHE STRING "Installation directory for executables")
set(INCLUDE_INSTALL_DIR include CACHE STRING "Installation directory for header files")

## #################################################################
## Extensions
## #################################################################

#functions to fetch(download) head revision from git or Subversion
include(gsFetch)

if (EIGEN_USE_MKL_ALL)
  # See http://eigen.tuxfamily.org/dox/TopicUsingIntelMKL.html
  find_package(MKL REQUIRED)
  set (GISMO_INCLUDE_DIRS ${GISMO_INCLUDE_DIRS} ${MKL_INCLUDE_DIR} ${MKL_INCLUDE}
    CACHE INTERNAL "${PROJECT_NAME} include directories")
endif()

if (";${GISMO_OPTIONAL};" MATCHES ";gsUniversal;")
  gismo_fetch_module(gsUniversal)
endif()

if (";${GISMO_OPTIONAL};" MATCHES ";gsCoDiPack;")
  gismo_fetch_module(gsCoDiPack)
endif()

#first time
include_directories(${GISMO_INCLUDE_DIRS})

# Add externals directory
add_subdirectory(external)

if(GISMO_WITH_MPI)
  find_package(MPI REQUIRED)
  set (GISMO_INCLUDE_DIRS ${GISMO_INCLUDE_DIRS} ${MPI_INCLUDE_PATH}
  CACHE INTERNAL "${PROJECT_NAME} include directories)" FORCE)
  set(gismo_LINKER ${gismo_LINKER} ${MPI_CXX_LIBRARIES}
  CACHE INTERNAL "${PROJECT_NAME} extra linker objects" FORCE)
endif(GISMO_WITH_MPI)

if(${GISMO_COEFF_TYPE} STREQUAL "mpq_class")
  include(external/gsGmp.cmake)
endif()

if(${GISMO_COEFF_TYPE} STREQUAL "mpfr::mpreal")
  include(external/gsGmp.cmake)
  include(external/gsMpfr.cmake)
endif()

if(GISMO_WITH_SUPERLU)
  find_package(SuperLU REQUIRED)
  set (GISMO_INCLUDE_DIRS ${GISMO_INCLUDE_DIRS} ${SUPERLU_INCLUDES}
  CACHE INTERNAL "${PROJECT_NAME} include directories")
  set(gismo_LINKER ${gismo_LINKER} ${SUPERLU_LIBRARIES}
  CACHE INTERNAL "${PROJECT_NAME} extra linker objects")
  #include_directories(SYSTEM ${SUPERLU_INCLUDES})
endif(GISMO_WITH_SUPERLU)

if(GISMO_WITH_TAUCS)
  find_package(Taucs REQUIRED)
  set (GISMO_INCLUDE_DIRS ${GISMO_INCLUDE_DIRS} ${TAUCS_INCLUDES}
  CACHE INTERNAL "${PROJECT_NAME} include directories")
  set(gismo_LINKER ${gismo_LINKER} ${TAUCS_LIBRARIES}
  CACHE INTERNAL "${PROJECT_NAME} extra linker objects")
  #include_directories(SYSTEM ${SUPERLU_INCLUDES})
endif(GISMO_WITH_TAUCS)

if(GISMO_WITH_UMFPACK)
  find_package(Umfpack REQUIRED)
  set (GISMO_INCLUDE_DIRS ${GISMO_INCLUDE_DIRS} ${UMFPACK_INCLUDES}
  CACHE INTERNAL "${PROJECT_NAME} include directories")
  set(gismo_LINKER ${gismo_LINKER} ${UMFPACK_LIBRARIES}
  CACHE INTERNAL "${PROJECT_NAME} extra linker objects")
  #include_directories(SYSTEM ${UMFPACK_INCLUDES})
endif(GISMO_WITH_UMFPACK)

#second time
include_directories(${GISMO_INCLUDE_DIRS})

# external inclusion paths
#get_property(EXTERNAL_INCLUDE_DIRS GLOBAL PROPERTY EXTERNAL_INCLUDE_DIRS)
#include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/external)

## #################################################################
## G+Smo sources
## #################################################################

# Add main sources directory
add_subdirectory(src)

# Optional directories based on build options
if(GISMO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GISMO_BUILD_UNITTESTS)
    enable_testing()
    add_subdirectory(unittests)
endif()

# Handle optional external modules
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/optional" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/optional")
    message(STATUS "Found optional modules directory")
    # Optional modules are handled separately and can be included as needed
endif()

# Export configuration
include(cmake/gismoExport.cmake)

# Export package for use from build tree
export(PACKAGE ${PROJECT_NAME})

# Status summary
message(STATUS "")
message(STATUS "GISMO Configuration Summary:")
message(STATUS "  Version: ${gismo_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Components:")
message(STATUS "  Core: ${GISMO_WITH_CORE}")
message(STATUS "  Math: ${GISMO_WITH_MATH}")
message(STATUS "  Geometry: ${GISMO_WITH_GEOMETRY}")
message(STATUS "  Matrix: ${GISMO_WITH_MATRIX}")
message(STATUS "  Basis: ${GISMO_WITH_BASIS}")
message(STATUS "  Function: ${GISMO_WITH_FUNCTION}")
message(STATUS "  IO: ${GISMO_WITH_IO}")
message(STATUS "  Assembler: ${GISMO_WITH_ASSEMBLER}")
message(STATUS "")
message(STATUS "Optional modules:")
message(STATUS "  IETI: ${GISMO_WITH_IETI}")
message(STATUS "  Parallel: ${GISMO_WITH_PARALLEL}")
message(STATUS "  Compiler: ${GISMO_WITH_COMPILER}")
message(STATUS "")
