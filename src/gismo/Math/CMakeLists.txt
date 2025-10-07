# Math Module CMakeLists.txt
# Mathematical operations, linear algebra, and Eigen integration

project(Math
VERSION
  ${${CMAKE_PROJECT_NAME}_VERSION}
LANGUAGES
  CXX)

## #################################################################
## Create target
## #################################################################

add_library(${PROJECT_NAME} INTERFACE)

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
  INTERFACE
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Math
        LinearAlgebra.h
        Constants.h
        EigenDeclarations.h
        MatrixAddons.h
        PlainObjectBaseAddons.h
        Adjugate.h
        BlockDiag.h
        BlockTranspose.h
        Matrix.h
        Vector.h
        SparseMatrix.h
        SparseVector.h
        SparseSolver.h
        MatrixBlockView.h
        AsMatrix.h
        FiberMatrix.h
        Point.h
        Combinatorics.h
        PointGrid.h
        MatrixTypeAliases.h
        VecAsSymmMatrix.h
        RowSelection.h
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

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