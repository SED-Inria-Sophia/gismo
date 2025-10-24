# GISMO IO Module

## Overview

The IO module provides input/output functionality for GISMO applications including file I/O, command-line parsing, XML processing, and Paraview visualization export.

## Core Components

- **FileData**: High-level GISMO file reading/writing
- **CommandLine**: Command-line argument parsing
- **OptionList**: Configuration and parameter management
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
├── OptionList.h/.cpp        # Configuration management
├── FileManager.h/.cpp       # Basic file operations
├── Csv.h                   # CSV export (header-only)
│
├── FileData.h/.hpp         # High-level GISMO file I/O
├── FileData_.cpp           # FileData implementations
├── ReadFile.h              # Convenient file reading
├── ReadFile_.cpp           # ReadFile implementations
│
├── Xml.h/.hpp/.cpp         # XML parsing and serialization
├── XmlUtils.h/.hpp/.cpp    # XML utility functions
├── XmlOptionList.h/.cpp    # XML serialization for OptionList
├── XmlInstance.cpp         # XML template instantiations
├── Xml*.hpp                # XML specializations for Core classes
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