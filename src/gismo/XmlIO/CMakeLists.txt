project(XmlIO
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
  PUBLIC
    FILE_SET HEADERS
      BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
      FILES
        XmlIO                           # Main module header
        XmlSerializer.h                 # SerializationInterface implementation
        XmlFactoryRegistrar.h           # Auto-registration with Core
        # Migrated XML core system (from gsIO) - clean naming convention
        Core/Xml.h                      # Main XML template system
        Core/XmlUtils.h                 # XML serialization utilities
        Core/FileManager.h              # File management utilities
  PRIVATE
    XmlSerializer.cpp                   # XML serialization implementation
    XmlFactoryRegistrar.cpp             # Factory registration logic
    # Migrated XML core implementation files - clean naming convention
    Core/Xml.cpp                        # XML implementation
    Core/Xml_.cpp                       # XML template instantiations
    Core/XmlInstance_.cpp               # XML instance implementations
    Core/FileManager.cpp                # File management implementation
)

# XML specialization headers (implementation detail - not part of public interface)
target_sources(${PROJECT_NAME}
  PRIVATE
    Serialization/ComposedBasis.xml.hpp
    Serialization/ComposedGeometry.xml.hpp
    Serialization/ComposedFunction.xml.hpp
    Serialization/ConstantFunction.xml.hpp
    Serialization/FunctionExpr.xml.hpp
    Serialization/JITCompiler.xml.hpp
    Serialization/MultiBasis.assembler.hpp
    # XML template implementations (from gsIO) - clean naming convention
    Core/Xml.hpp                        # XML template implementations
    Core/XmlUtils.hpp                   # XML utility implementations
    Core/XmlGenericUtils.hpp            # Generic XML utilities
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    # Note: XmlIO now contains all necessary XML files - no external dependencies
)

## #################################################################
## Dependencies
## #################################################################

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_PROJECT_NAME}::Core      # For interfaces and Core types
  # Note: XmlIO is now completely self-contained with all XML functionality
)

# Enable XML serialization features
target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    GISMO_WITH_XML_SERIALIZATION
  PRIVATE
    GISMO_XMLIO_MODULE
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