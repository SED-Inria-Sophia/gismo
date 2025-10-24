project(JIT
VERSION
  ${${CMAKE_PROJECT_NAME}_VERSION}
LANGUAGES
  CXX)

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
## Build rules
## #################################################################

target_sources(${PROJECT_NAME}
  PRIVATE
    JITCompiler.cpp
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        JIT  # Main module header
        JITCompiler.h
        JITCompiler.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_PROJECT_NAME}::IO
    ${CMAKE_PROJECT_NAME}::Common
)

# Platform-specific libraries for dynamic loading
if(WIN32)
  # Windows uses built-in LoadLibrary/GetProcAddress
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

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