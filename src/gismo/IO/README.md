# GISMO IO Module

## Overview

The IO module provides input/output functionality for GISMO applications including file I/O, command-line parsing, XML processing, and Paraview visualization export.

## Core Components

- **FileData**: High-level GISMO file reading/writing
- **CommandLine**: Command-line argument parsing
- **OptionList**: Configuration and parameter management (includes integrated XML serialization)
- **Xml**: XML parsing and serialization
- **Paraview**: Visualization file export
- **FileManager**: Basic file operations

## Source Tree

```
src/gismo/IO/
├── IO                        # Main convenience header
├── CMakeLists.txt           # Modern CMake configuration
├── README.md               # This file
│
├── CommandLine.h/.cpp       # Command-line argument parsing
├── OptionList.h/.cpp        # Configuration management with XML serialization
├── FileManager.h/.cpp       # Basic file operations
├── Csv.h                   # CSV export (header-only)
│
├── gzstreamer/             # Compressed stream I/O
│   ├── gzstreamer.h       # Gzip stream interface
│   └── gzstreamer.cpp     # Gzip stream implementation
│
├── FileData.h/.hpp         # High-level GISMO file I/O
├── FileData_.cpp           # FileData implementations
├── ReadFile.h              # Convenient file reading
├── ReadFile_.cpp           # ReadFile implementations
│
├── Xml.h/.hpp/.cpp         # XML parsing and serialization
├── XmlTypes.h              # XML type definitions
├── XmlRegistry.h/.cpp      # Modern XML registry system
│
├── XmlFunction.h           # Function XML serialization
├── XmlFunctionExpr.h       # Function expression XML
├── XmlConstantFunction.h   # Constant function XML
├── XmlComposedFunction.h   # Composed function XML
├── XmlFunction_.cpp        # Function XML instantiations
│
├── XmlGeometry.h           # Geometry XML serialization
├── XmlComposedGeometry.h   # Composed geometry XML
├── XmlCurve.h              # Curve XML serialization
├── XmlSurface.h            # Surface XML serialization
├── XmlGeometry_.cpp        # Geometry XML instantiations
│
├── XmlBasis.h              # Basis XML serialization
├── XmlComposedBasis.h      # Composed basis XML
├── XmlBasis_.cpp           # Basis XML instantiations
│
├── XmlMatrix.h             # Matrix XML serialization
├── XmlSparseMatrix.h       # Sparse matrix XML
├── XmlMatrix_.cpp          # Matrix XML instantiations
│
├── XmlInstance.cpp         # XML template instantiations
├── Xml*.hpp                # Legacy XML specializations (deprecated)
│
└── Paraview/              # Paraview visualization export
    ├── Paraview           # Paraview convenience header
    ├── WriteParaview.h/.hpp/.cpp      # Main Paraview writer
    ├── ParaviewCollection.h/.cpp      # Collection (.pvd) files
    ├── ParaviewDataSet.h/.cpp         # Dataset (.vts) files
    ├── ParaviewUtils.h/.hpp/.cpp      # Paraview utilities
    └── ParaviewUtils_.cpp             # ParaviewUtils implementations
```

## External Dependencies

The IO module requires two external libraries that are automatically managed by the build system:

### RapidXML
- **Purpose**: XML parsing and generation
- **Used by**: `Xml.h`, `FileData.h`, and all XML functionality
- **Location**: `external/rapidxml/`
- **License**: Boost Software License or MIT License
- **Why**: Fast, header-only XML parser optimized for performance

### TCLAP (Templatized Command Line Argument Parser)
- **Purpose**: Command-line argument parsing
- **Used by**: `CommandLine.h` and `gsCmdLine` class
- **Location**: `external/tclap/include/`
- **License**: MIT License
- **Why**: Simple, type-safe command-line parsing with automatic help generation

### Automatic Dependency Management
```cmake
# Dependencies are automatically found and configured
find_path(RAPIDXML_INCLUDE_DIR NAMES rapidxml.hpp ...)
find_path(TCLAP_INCLUDE_DIR NAMES tclap/CmdLine.h ...)

# Modern INTERFACE targets created automatically
target_link_libraries(gismo::IO PUBLIC RapidXML::RapidXML TCLAP::TCLAP)
```

**Note**: These dependencies are bundled with GISMO and require no separate installation. The build system automatically creates modern CMake INTERFACE targets for clean dependency management.

---

## Modern XML Architecture

The GISMO IO module has been modernized with a new XML serialization architecture that provides type-safe, extensible, and dependency-reduced XML handling.

### Key Design Principles

1. **Type Safety**: All XML operations are type-checked at compile time
2. **Dependency Reduction**: Circular dependencies between modules eliminated
3. **Extensibility**: Easy to add new types without modifying existing code
4. **Performance**: Registry-based dispatch with minimal runtime overhead
5. **Backward Compatibility**: Existing code continues to work unchanged

### Core Components

#### XmlRegistry: The Foundation
The `XmlRegistry` is a singleton that manages all XML type registrations and provides polymorphic dispatch:

```cpp
#include <gismo/IO/XmlRegistry.h>

// Register a new type (done automatically for GISMO types)
XmlRegistry::registerType<MyClass>(
    "MyType",           // XML type attribute
    MyXmlClass::get,    // Deserializer function
    MyXmlClass::put,    // Serializer function
    "MyTag"             // XML tag name
);

// Polymorphic serialization
gsXmlNode* node = XmlRegistry::putPolymorphic(myObject, xmlDoc);
MyClass* obj = XmlRegistry::getByTypeAttribute<MyClass>(node, "MyType");
```

#### XmlType Classes: Type-Specific Logic
Each GISMO type has a corresponding `XmlType<T>` class that handles serialization:

```cpp
template<class T>
class XmlMatrix {
public:
    static std::string tag() { return "Matrix"; }
    static std::string type() { return "Matrix"; }

    static gsMatrix<T>* get(gsXmlNode* node) {
        // Custom deserialization logic
    }

    static gsXmlNode* put(const gsMatrix<T>& obj, gsXmlTree& data) {
        // Custom serialization logic
    }

    static void registerType() {
        XmlRegistry::registerType<gsMatrix<T>>(type(), get, put, tag());
    }
};
```

#### gsXml Wrapper: Backward Compatibility
The familiar `gsXml<T>` interface is preserved but now dispatches to the registry:

```cpp
// This still works exactly as before
gsMatrix<>* matrix = gsXml<gsMatrix<>>::get(node);
gsXmlNode* node = gsXml<gsMatrix<>>::put(*matrix, doc);
```

### Supported Types

The modern XML system currently supports:

#### Functions
- `gsFunction<T>` - Base function interface
- `gsFunctionExpr<T>` - Expression-based functions
- `gsConstantFunction<T>` - Constant functions
- `gsComposedFunction<T>` - Composed functions

#### Geometry
- `gsGeometry<T>` - Base geometry interface
- `gsComposedGeometry<T>` - Composed geometry
- `gsCurve<T>` - Curve abstractions
- `gsSurface<T>` - Surface abstractions

#### Basis Functions
- `gsBasis<T>` - Base basis interface
- `gsComposedBasis<T>` - Composed basis functions

#### Mathematics
- `gsMatrix<T>` - Dense matrices (real_t, index_t)
- `gsSparseMatrix<T>` - Sparse matrices (real_t, index_t, bool)

### Adding New Types

To add XML support for a new type, follow these steps:

#### 1. Create XmlType Class
```cpp
// In XmlMyType.h
#include <gismo/IO/XmlRegistry.h>

template<class T>
class XmlMyType {
public:
    static std::string tag() { return "MyTag"; }
    static std::string type() { return "MyType"; }

    static MyType<T>* get(gsXmlNode* node) {
        // Parse XML attributes and content
        // Return new MyType instance
    }

    static gsXmlNode* put(const MyType<T>& obj, gsXmlTree& data) {
        // Create XML node with appropriate attributes
        // Return the node
    }

    static void registerType() {
        XmlRegistry::registerType<MyType<T>>(type(), get, put, tag());
    }
};
```

#### 2. Create gsXml Wrapper
```cpp
// Still in XmlMyType.h
template<class T>
class gsXml< MyType<T> > {
private:
    gsXml() { }
    typedef MyType<T> Object;

public:
    GSXML_COMMON_FUNCTIONS(Object);  // Standard interface

    static std::string tag() { return XmlMyType<T>::tag(); }
    static std::string type() { return XmlMyType<T>::type(); }

    static Object* get(gsXmlNode* node) {
        return XmlRegistry::get<Object>(node);
    }

    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        return XmlRegistry::put(obj, data);
    }
};
```

#### 3. Create Instantiation File
```cpp
// In XmlMyType_.cpp
#include <gismo/Common/TemplateTools.h>
#include "XmlMyType.h"

namespace gismo {
namespace internal {

// Explicit template instantiations
TEMPLATE_INST class XmlMyType<real_t>;
TEMPLATE_INST class gsXml< MyType<real_t> >;

} // namespace internal
} // namespace gismo

// Registration at static initialization
namespace gismo {
namespace internal {
namespace {
    struct MyTypeXmlRegistrations {
        MyTypeXmlRegistrations() {
            XmlMyType<real_t>::registerType();
        }
    };
    static MyTypeXmlRegistrations mytype_registrations;
}
} // namespace internal
} // namespace gismo
```

#### 4. Update CMakeLists.txt
```cmake
target_sources(IO
  PUBLIC
    FILE_SET HEADERS
      FILES
        XmlMyType.h
  PRIVATE
    XmlMyType_.cpp
)
```

### Advanced Features

#### Custom get_into Methods
For types that need custom deserialization into existing objects:

```cpp
// Don't use GSXML_GET_INTO macro, implement custom method
static void get_into(gsXmlNode* node, Object& obj) {
    // Custom logic for populating existing object
    // e.g., matrix resizing, sparse matrix optimization
}
```

#### Polymorphic Composition
For types that contain other polymorphic types:

```cpp
static gsXmlNode* put(const gsComposedFunction<T>& obj, gsXmlTree& data) {
    gsXmlNode* node = makeNode("Function", data);

    // Handle composition polymorphically
    if (const gsGeometry<T>* geo = dynamic_cast<const gsGeometry<T>*>(&obj.composition()))
        compData = XmlRegistry::putPolymorphic(*geo, data);
    else if (const gsFunction<T>* fun = dynamic_cast<const gsFunction<T>*>(&obj.composition()))
        compData = XmlRegistry::putPolymorphic(*fun, data);

    return node;
}
```

#### Type-Specific Registry Access
For types that need explicit template dispatch:

```cpp
// Use explicit template type instead of runtime dispatch
gsXmlNode* basis_content = XmlRegistry::put<BasisType>(obj.basis(), data);
// vs.
gsXmlNode* comp_content = XmlRegistry::putPolymorphic(obj.composition(), data);
```

### Migration from Legacy System

The old `XmlUtils.hpp` system is deprecated but still supported. To migrate:

1. **No immediate action required** - existing code continues to work
2. **New types** should use the modern XmlRegistry system
3. **Performance benefits** - registry dispatch is faster than old template recursion
4. **Better error messages** - type-safe registration catches errors at startup

### Architecture Benefits

#### Eliminated Circular Dependencies
**Before**: Core ↔ IO circular dependency through template instantiation
**After**: Core → IO dependency only, with registration in IO module

#### Improved Build Times
- Template instantiations concentrated in `_cpp` files
- Reduced header dependencies
- Parallel compilation of instantiation units

#### Runtime Performance
- Registry lookup is O(1) hash table access
- No template instantiation overhead at runtime
- Reduced binary size through explicit instantiation

---
## Quick Usage

### Reading GISMO Files
```cpp
#include <gismo/IO/IO>

gsFileData<> file("input.xml");
if (file.has<gsGeometry<>>()) {
    auto geometry = file.getFirst<gsGeometry<>>();
}
```

### Command-Line Applications
```cpp
#include <gismo/IO/IO>

int main(int argc, char** argv) {
    gsCmdLine cmd("My Application");
    cmd.addString("i", "input", "Input file", "input.xml");
    cmd.addInt("r", "refine", "Refinement levels", 3);

    try { cmd.getValues(argc, argv); }
    catch (...) { return EXIT_FAILURE; }

    std::string input = cmd.getString("input");
    int refine = cmd.getInt("refine");
}
```

### Configuration Management
```cpp
#include <gismo/IO/IO>

gsOptionList options;
options.addInt("iterations", "Max iterations", 100);
options.addReal("tolerance", "Tolerance", 1e-8);

int maxIter = options.getInt("iterations");
```

### Paraview Export
```cpp
#include <gismo/IO/IO>

gsWriteParaview<> writer("output", pts, data);
writer.plot();  // Creates output.vts
```

## Usage

Simply include the main IO header to access all functionality:
```cpp
#include <gismo/IO/IO>
```

For specific components, use individual headers:
```cpp
#include <gismo/IO/CommandLine.h>
#include <gismo/IO/FileData.h>
#include <gismo/IO/Paraview/WriteParaview.h>
```