# Math Module CMakeLists.txt
# Linear algebra and mathematical utilities

project(Math
VERSION
  ${${CMAKE_PROJECT_NAME}_VERSION}
LANGUAGES
  CXX)

## #################################################################
## Dependencies - Eigen3
## #################################################################

if(NOT TARGET Eigen3::Eigen)
  find_package(Eigen3 REQUIRED)
endif()

## #################################################################
## Create target
## #################################################################

add_library(${PROJECT_NAME} INTERFACE)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    VERSION   ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

## #################################################################
## Build rules
## #################################################################

target_sources(${PROJECT_NAME}
  INTERFACE
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Math
        LinearAlgebra.h
        Constants.h
        Matrix.h
        Vector.h
        SparseMatrix.h
        MatrixView.h
        PointGrid.h
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

## #################################################################
## Dependencies
## #################################################################

target_link_libraries(${PROJECT_NAME}
  INTERFACE
    ${CMAKE_PROJECT_NAME}::Common
    Eigen3::Eigen
)

## ###################################################################
## Install rules
## ###################################################################

install(
  TARGETS
    ${PROJECT_NAME}
  EXPORT
    ${CMAKE_PROJECT_NAME}${PROJECT_NAME}-targets
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