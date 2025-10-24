project(Core
VERSION
  ${${CMAKE_PROJECT_NAME}_VERSION}
LANGUAGES
  CXX)

## #################################################################
## Create unified Core target
## #################################################################

add_library(${PROJECT_NAME} SHARED)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME ${CMAKE_PROJECT_NAME}${PROJECT_NAME}
    VERSION   ${${PROJECT_NAME}_VERSION}
    SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

add_library(${CMAKE_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

## #################################################################
## Collect sources from all subfolders
## #################################################################

# Function layer sources
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Function/FunctionSet.h
        Function/Function.h
        Function/FuncData.h
        Function/FuncCoordinate.h
        Function/FunctionSet.hpp
        Function/Function.hpp
  PRIVATE
    Function/FunctionSet_.cpp
    Function/Function_.cpp
)

# Topology layer sources (has implementations)
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Topology/Boundary.h
        Topology/BoxTopology.h
  PRIVATE
    Topology/Boundary.cpp
    Topology/BoxTopology.cpp
)

# Geometry layer sources (unified - includes geometric shapes)
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Geometry/Geometry.h
        Geometry/Geometry.hpp
        Geometry/ComposedGeometry.h
        Geometry/ComposedGeometry.hpp
        Geometry/Curve.h
        Geometry/Surface.h
        Geometry/Volume.h
        Geometry/Bulk.h
        Geometry/GenericGeometry.h
        Geometry/GeometrySlice.h
        Geometry/Curve.hpp
        Geometry/Surface.hpp
        Geometry/Volume.hpp
        Geometry/Bulk.hpp
  PRIVATE
    Geometry/Geometry_.cpp
    Geometry/ComposedGeometry_.cpp
)

# Basis layer sources
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Basis/Basis.h
        Basis/BasisFun.h
        Basis/BasisRefs.h
        Basis/ConstantBasis.h
        Basis/RationalBasis.h
        Basis/ComposedBasis.h
        Basis/Basis.hpp
        Basis/RationalBasis.hpp
        Basis/ComposedBasis.hpp
  PRIVATE
    Basis/Basis_.cpp
    Basis/BasisFun_.cpp
    Basis/ComposedBasis_.cpp
)

# SpecializedFunction layer sources
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        SpecializedFunction/AffineFunction.h
        SpecializedFunction/FunctionExpr.h
        SpecializedFunction/ComposedFunction.h
        SpecializedFunction/ConstantFunction.h
        SpecializedFunction/PiecewiseFunction.h
        SpecializedFunction/PatchwiseFunction.h
        SpecializedFunction/GeometryTransform.h
        SpecializedFunction/AffineFunction.hpp
        SpecializedFunction/FunctionExpr.hpp
        SpecializedFunction/ComposedFunction.hpp
        SpecializedFunction/ConstantFunction.hpp
  PRIVATE
    SpecializedFunction/AffineFunction_.cpp
    SpecializedFunction/ComposedFunction_.cpp
    SpecializedFunction/ConstantFunction_.cpp
)

# DOFMapping layer sources (has implementations)
# TODO: Re-enable when dependencies are resolved
# target_sources(${PROJECT_NAME}
#   PUBLIC
#     FILE_SET HEADERS
#       BASE_DIRS
#         ${CMAKE_CURRENT_SOURCE_DIR}
#       FILES
#         DOFMapping/DofMapper.h
#         DOFMapping/DofMapper.hpp
#   PRIVATE
#     DOFMapping/DofMapper.cpp
#     DOFMapping/DofMapper_.cpp
# )

# MultiPatch layer sources (header-only - all templates)
# TODO: Re-enable when dependencies are resolved
# target_sources(${PROJECT_NAME}
#   PUBLIC
#     FILE_SET HEADERS
#       BASE_DIRS
#         ${CMAKE_CURRENT_SOURCE_DIR}
#       FILES
#         MultiPatch/MultiPatch.h
#         MultiPatch/MultiBasis.h
#         MultiPatch/MultiPatch.hpp
#         MultiPatch/MultiBasis.hpp
# )

# Field layer sources
# TODO: Re-enable when dependencies are resolved
# target_sources(${PROJECT_NAME}
#   PUBLIC
#     FILE_SET HEADERS
#       BASE_DIRS
#         ${CMAKE_CURRENT_SOURCE_DIR}
#       FILES
#         Field/Field.h
#         Field/FieldCreator.h
#         Field/Field.hpp
#   PRIVATE
#     Field/Field_.cpp
# )

# Mesh layer sources (has implementations)
# Dependencies resolved using gsDomainBase interface and reinterpret_cast
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Mesh/Mesh.h
        Mesh/MeshElement.h
        Mesh/MeshElement.hpp
        Mesh/Vertex.h
        Mesh/Edge.h
        Mesh/Face.h
        Mesh/Cell.h
        Mesh/BoundingBox.h
        Mesh/HalfEdgeMesh.h
        Mesh/Mesh.hpp
        Mesh/HalfEdgeMesh.hpp
  PRIVATE
    Mesh/Mesh_.cpp
    Mesh/HalfEdgeMesh_.cpp
)

# Interface layer sources (abstractions for dependency injection)
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        # Interface files removed - not used by Core functionality
  PRIVATE
    # Interface implementations removed
)

# Utilities layer sources (has implementations)
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        Utilities/SysInfo.h
        Utilities/Base64.h
        Utilities/Base64.hpp
  PRIVATE
    Utilities/SysInfo.cpp
    Utilities/Base64.cpp
)

## #################################################################
## Dependencies
## #################################################################

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_PROJECT_NAME}::Common
    ${CMAKE_PROJECT_NAME}::Math
)

# Core is now completely independent - no XML dependencies
# XML functionality is provided by optional XmlIO module

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

## #################################################################
## Main Core convenience header
## #################################################################

# Create main Core header that includes all submodules
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Core.in
    ${CMAKE_CURRENT_BINARY_DIR}/Core
    @ONLY
)

target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        ${CMAKE_CURRENT_BINARY_DIR}/Core
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