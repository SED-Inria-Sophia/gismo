# GISMO JIT Module

## Overview

The JIT (Just-In-Time) module provides runtime code generation and compilation capabilities for GISMO, enabling performance-critical applications to generate and compile optimized code at runtime.

## Core Components

- **gsJITCompiler**: Main JIT compilation engine
- **gsJITCompilerConfig**: Compiler configuration and auto-detection
- **gsDynamicLibrary**: Cross-platform dynamic library management
- **XML Support**: Configuration serialization and deserialization

## Source Tree

```
src/gismo/JIT/
├── JIT                      # Main convenience header
├── CMakeLists.txt          # Modern CMake configuration
├── README.md              # This file
│
├── JITCompiler.h          # Main JIT compiler classes
├── JITCompiler.cpp        # JIT compiler implementations
└── JITCompiler.hpp        # XML serialization specializations
```

## External Dependencies

The JIT module requires system-level dependencies that must be available at runtime:

### System Compiler
- **Purpose**: Compile generated code into dynamic libraries
- **Supported**: GCC, Clang, MSVC, Intel C++ Compiler, NVCC (CUDA)
- **Required**: At least one C++ compiler must be installed and accessible via PATH
- **Detection**: Automatic compiler detection and configuration

### Platform Dynamic Loading APIs
- **Windows**: `LoadLibrary`, `GetProcAddress`, `FreeLibrary` (kernel32.dll)
- **Linux/Unix**: `dlopen`, `dlsym`, `dlclose` (libdl)
- **macOS**: `dlopen`, `dlsym`, `dlclose` (built-in system APIs)
- **Purpose**: Load and execute dynamically compiled code at runtime

### System Dependencies
- **Temporary Directory**: Used for intermediate compilation files
- **File System**: Read/write access for source and library files
- **Process Creation**: Ability to spawn compiler processes

**Note**: Unlike other GISMO modules, JIT depends on external system tools that cannot be bundled. Users must have a working C++ compiler installation.

## Platform Support
- **Windows**: MSVC, Intel C++ Compiler, MinGW-w64/Clang
- **Linux**: GCC, Clang, Intel C++ Compiler
- **macOS**: Apple Clang, GCC via Homebrew
- **CUDA**: NVCC for GPU kernel compilation (when available)

## Quick Usage

### Basic JIT Compilation
```cpp
#include <gismo/JIT/JIT>

// Create JIT compiler with auto-detected configuration
gsJITCompiler jit;

// Add source code
jit << "EXPORT double compute(double x) { return x * x + 1.0; }";

// Compile and get function
auto lib = jit.build();
auto compute = lib.getSymbol<double(double)>("compute");
double result = compute(3.14);
```

### Compiler Configuration
```cpp
#include <gismo/JIT/JIT>

// Use specific compiler
gsJITCompilerConfig config = gsJITCompilerConfig::gcc();
config.setFlags("-O3 -march=native");

gsJITCompiler jit(config);
// ... compile with optimizations
```

### XML Configuration
```cpp
#include <gismo/JIT/JIT>

// Save/load compiler configuration
gsJITCompilerConfig config;
config.save("compiler.xml");

gsJITCompilerConfig loaded;
loaded.load("compiler.xml");
```

## Usage

Simply include the main JIT header to access all functionality:
```cpp
#include <gismo/JIT/JIT>
```

For specific components, use individual headers:
```cpp
#include <gismo/JIT/JITCompiler.h>
```