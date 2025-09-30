# Common Module Documentation

This document describes the Common module, which serves as the foundation layer of the new modular G+Smo architecture. The Common module provides essential utilities with zero external dependencies, following the Generic C++ Build Pattern.

## Overview

The Common module was created by extracting and cleaning up foundation-level code from the original `gsCore` module. The goal was to create a minimal, dependency-free foundation layer that other modules can build upon.

All files follow the new naming conventions:
- PascalCase headers (e.g., `Debug.h` instead of `gsDebug.h`)
- No `gs` prefix on types and functions
- Clean, modern C++ code

## File-by-File Migration Summary

### Assert.h
**Original**: Part of `gsDebug.h` in `src/gsCore/gsDebug.h`
**Purpose**: Assertion macros for runtime checks
**Changes**:
- **Extracted** from `gsDebug.h` for better organization and modularity
- **Simplified** to just include `Debug.h` (assertions are defined there)
- **Rationale**: Separates assertion functionality from general debugging utilities for cleaner dependencies

### BoundedPriorityQueue.h
**Original**: `gsBoundedPriorityQueue.h` in `src/gsUtils/`
**Purpose**: Priority queue with fixed maximum size
**Changes**:
- **Moved** from `gsUtils` to `Common` as it's a fundamental data structure
- **Renamed** from `gsBoundedPriorityQueue` to follow new naming conventions (no `gs` prefix)
- **Cleaned up** implementation, removed unnecessary dependencies on other gsUtils functions
- **Rationale**: Priority queues are fundamental data structures that should be available in the foundation layer

### Combinatorics.h
**Original**: `gsCombinatorics.h` in `src/gsUtils/`
**Purpose**: Mathematical combinatorial functions (factorial, binomial coefficients)
**Changes**:
- **Moved** from `gsUtils` to `Common` as fundamental math utilities
- **Renamed** functions: `gsFactorial()` → `factorial()`, `gsBinomial()` → `binomial()`
- **Simplified** implementation using precomputed lookup table for factorial
- **Removed** dependencies on other gsUtils functions
- **Rationale**: Combinatorial functions are basic mathematical utilities needed across the library

### Config.h
**Original**: Generated from `gsConfig.h.in` in `src/gsCore/`
**Purpose**: Build configuration and version information
**Changes**:
- **Removed** CMake-generated variables that caused conflicts (`@gismo_VERSION@` etc.)
- **Hardcoded** version to "1.0.0" for the new architecture
- **Simplified** coefficient type to `double` (removed CMake variable)
- **Removed** data directory and search path configurations
- **Removed** extension includes (`gsConfigExt.h`, MPI, PARDISO flags)
- **Rationale**: Foundation layer should not depend on build system complexity; higher layers can add configuration as needed

### Debug.h
**Original**: `gsDebug.h` in `src/gsCore/gsDebug.h` (324 lines)
**Purpose**: Debugging and messaging system
**Changes**:
- **Major cleanup**: Reduced from 324 lines to 130 lines (60% reduction)
- **Removed** MSVC-specific memory debugging code (crt debug headers, popup disabling)
- **Removed** extensive compiler warning pragmas (~100 lines for MSVC, Intel, Clang, GCC)
- **Removed** complex static assertion system with template metaprogramming fallbacks
- **Removed** deprecated macro system (`GISMO_DEPRECATED`)
- **Kept** only essential logging macros (`gsInfo`, `gsWarn`, `gsDebug`, `gsDebugVar`)
- **Kept** core assertion macros (`GISMO_ASSERT`, `GISMO_ENSURE`, `GISMO_ERROR`)
- **Kept** utility macros (`GISMO_UNUSED`, `GISMO_HPP_HEADER`)
- **Kept** math functions (`gsIsnumber`, `gsIsnan`, `gsIsfinite`)
- **Rationale**: Foundation layer should be minimal and dependency-free; platform-specific code and complex workarounds introduce hidden dependencies

### EigenDeclarations.h
**Original**: `gsEigenDeclarations.h` in `src/gsMatrix/`
**Purpose**: Forward declarations for Eigen types
**Changes**:
- **Moved** from `gsMatrix` to `Common` as it's needed by multiple modules
- **Simplified** to only forward declarations (no implementations)
- **Removed** dependencies on matrix operations and other gsMatrix functionality
- **Rationale**: Eigen forward declarations are needed early in the include chain but don't require full matrix functionality

### Export.h
**Original**: Generated from `gsExport.h.in` in `src/gsCore/`
**Purpose**: Symbol export/import macros for shared libraries
**Changes**:
- **Minimal changes**: mainly namespace updates and file path references
- **Removed** any platform-specific workarounds that were present in the original
- **Rationale**: Export macros are fundamental to library structure and belong in the foundation layer

### ForwardDeclarations.h
**Original**: `gsForwardDeclarations.h` in `src/gsCore/`
**Purpose**: Forward declarations of key G+Smo classes
**Changes**:
- **Updated** to use new class names without `gs` prefix
- **Removed** forward declarations for classes that moved to other modules (not needed in foundation)
- **Kept** only the essential forward declarations needed by Common and its dependents
- **Rationale**: Forward declarations reduce compilation dependencies but should only include what's actually needed

### MatrixAddons.h
**Original**: `gsMatrixAddons.h` in `src/gsMatrix/` (92 lines with implementations)
**Purpose**: Extensions to Eigen MatrixBase class
**Changes**:
- **Simplified** from full implementations to only forward declarations
- **Removed** complex template implementations (Gauss elimination, block operations, etc.)
- **Made** header-only for better compilation performance
- **Rationale**: Full matrix operations belong in higher-level modules; foundation only needs declarations to avoid circular dependencies

### Memory.h
**Original**: `gsMemory.h` in `src/gsCore/` (421 lines)
**Purpose**: Smart pointer adaptors and memory utilities
**Changes**:
- **Major simplification**: Reduced from 421 lines to 54 lines (87% reduction)
- **Removed** legacy C++03 compatibility code (boost/tr1 includes)
- **Removed** MinGW-specific memory headers
- **Removed** complex memory management utilities and adaptors
- **Kept** only modern C++11 smart pointer type aliases (`shared_ptr`, `unique_ptr`)
- **Rationale**: With C++17 requirement, legacy compatibility code is unnecessary; foundation only needs basic smart pointer support

### PlainObjectBaseAddons.h
**Original**: `gsPlainObjectBaseAddons.h` in `src/gsMatrix/`
**Purpose**: Extensions to Eigen PlainObjectBase
**Changes**:
- **Similar to MatrixAddons.h**: simplified to forward declarations only
- **Removed** implementation details that could create dependencies
- **Rationale**: Plain object extensions belong in higher-level math modules, not foundation

### SortedVector.h
**Original**: `gsSortedVector.h` in `src/gsUtils/`
**Purpose**: Self-sorting vector container
**Changes**:
- **Moved** from `gsUtils` to `Common` as fundamental data structure
- **Renamed** from `gsSortedVector` to follow new conventions
- **Cleaned up** implementation, removed dependencies on other utilities
- **Rationale**: Sorted containers are fundamental data structures used across the library

### Stopwatch.h
**Original**: `gsStopwatch.h` in `src/gsUtils/`
**Purpose**: Timing and profiling utilities
**Changes**:
- **Moved** from `gsUtils` to `Common` as it's used across the library
- **Simplified** timing implementation using standard C++ chrono
- **Removed** dependencies on other utility functions
- **Rationale**: Timing utilities are needed in many parts of the library and belong in foundation

### Types.h
**Original**: Part of `gsConfig.h` and scattered definitions in `src/gsCore/`
**Purpose**: Fundamental type aliases (`real_t`, `index_t`, etc.)
**Changes**:
- **Consolidated** type definitions from multiple files into one place
- **Updated** to use modern C++ type aliases (`using` instead of `typedef`)
- **Removed** CMake-generated type definitions, hardcoded to standard types
- **Rationale**: Type aliases are fundamental and should be defined early; CMake complexity belongs in higher layers

### Utils.h
**Original**: `gsUtils.h` in `src/gsUtils/` (333 lines)
**Purpose**: String utilities and helper functions
**Changes**:
- **Extracted** only the essential utility functions needed by foundation layer
- **Moved** string manipulation functions: `starts_with()`, `ends_with()`, `to_string()`
- **Removed** dependencies on gsCore headers (gsExport, gsDebug, gsMemory)
- **Removed** OpenMP dependencies and complex template metaprogramming
- **Removed** GCC-specific demangling code
- **Rationale**: Only basic string utilities are needed in foundation; complex utilities belong in higher layers

### Common (Main Header)
**Original**: N/A - this is a new convenience header
**Purpose**: Single include point for all Common module functionality
**Changes**:
- **New file** created for the modular architecture
- **Includes** all Common headers in logical order (foundation first, then utilities)
- **Follows** Generic C++ Build Pattern for module headers
- **Rationale**: Provides convenient single-include access while maintaining modular structure

## Migration Rationale

### Zero Dependencies Principle
The Common module was designed with a strict "zero external dependencies" rule:
- No dependencies on other G+Smo modules
- No platform-specific code that could break cross-compilation
- No complex template metaprogramming that could slow compilation
- Only standard C++ libraries and Eigen (for math types)

### Clean Naming Convention
All code was updated to follow the new naming conventions:
- Headers: PascalCase (e.g., `Debug.h`, not `gsDebug.h`)
- Functions: Remove `gs` prefix (e.g., `factorial()`, not `gsFactorial()`)
- Types: Remove `gs` prefix (e.g., `real_t`, not `gsReal_t`)

### Code Quality Improvements
- Removed legacy code for old compilers
- Simplified complex implementations
- Better error messages and documentation
- Consistent code formatting

## Testing

The Common module includes comprehensive unit tests in `tst/gismo/Common/Common_test.cpp` that validate:
- All header includes work correctly
- Type definitions are properly set up
- Memory utilities function correctly
- Mathematical functions produce correct results
- Container classes work as expected

Run tests with: `ctest -R Common_test`

## Dependencies

**None** - This is the foundation layer. Other modules depend on Common, but Common depends on nothing except:
- Standard C++ libraries
- Eigen3 (for mathematical types only)

## Future Extensions

As the modular architecture matures, additional foundation utilities may be added to Common, but the zero-dependencies principle will be maintained.