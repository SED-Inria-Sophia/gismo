# Math Module Documentation

This document describes the Math module, which provides linear algebra and mathematical utilities for the new modular G+Smo architecture. The Math module builds upon the Common module and provides the core mathematical infrastructure needed by other modules.

## Overview

The Math module was created by extracting and modernizing linear algebra code from the original `gsCore` and `gsMatrix` modules. The goal was to break the circular dependency between `gsCore` ↔ `gsMatrix` by creating a clean, modern linear algebra foundation.

All files follow the new naming conventions:
- PascalCase headers (e.g., `Matrix.h` instead of `gsMatrix.h`)
- No `gs` prefix on types and functions
- Modern C++17 code with proper template defaults
- Clean, dependency-managed architecture

## File-by-File Migration Summary

### Constants.h
**Original**: Part of `gsMath.h` in `src/gsCore/gsMath.h`
**Purpose**: Mathematical constants and basic math utilities
**Changes**:
- **Extracted** from `gsMath.h` to create a dedicated constants header
- **Renamed** from `gsMath.h` content to focus specifically on constants
- **Added** modern C++ constant definitions using `constexpr`
- **Removed** dependencies on other gsCore functionality
- **Rationale**: Mathematical constants are fundamental and should be separated from general math utilities for better organization

### LinearAlgebra.h
**Original**: `gsLinearAlgebra.h` in `src/gsCore/` (215 lines)
**Purpose**: Eigen library setup and linear algebra infrastructure
**Changes**:
- **Major cleanup**: Reduced from 215 lines to ~50 lines (75% reduction)
- **Removed** complex plugin system setup (EIGEN_MATRIXBASE_PLUGIN, etc.)
- **Removed** platform-specific workarounds and legacy code
- **Kept** essential Eigen includes and namespace setup
- **Added** `#define gsEigen Eigen` for backward compatibility during transition
- **Simplified** to only what's needed for basic linear algebra operations
- **Rationale**: Foundation math layer should be minimal; complex plugin systems belong in higher-level modules

### Math (Main Header)
**Original**: N/A - this is a new convenience header
**Purpose**: Single include point for all Math module functionality
**Changes**:
- **New file** created for the modular architecture
- **Includes** all Math headers in logical order (core first, then utilities)
- **Follows** Generic C++ Build Pattern for module headers
- **Rationale**: Provides convenient single-include access while maintaining modular structure

### Matrix.h
**Original**: `gsMatrix.h` in `src/gsMatrix/` (702 lines)
**Purpose**: Dense matrix class with Eigen integration
**Changes**:
- **Major modernization**: Updated template parameters to have defaults
- **Changed** from `template<class T, int _Rows, int _Cols, int _Options>` to `template<class T, int _Rows = Dynamic, int _Cols = Dynamic, int _Options = 0>`
- **Added** convenience typedefs: `gsMatrixd`, `gsVectord`, `gsRowVectord`, etc.
- **Removed** complex inheritance patterns and legacy code
- **Simplified** to focus on core matrix operations
- **Rationale**: Template defaults make the class easier to use while maintaining full flexibility

### MatrixView.h
**Original**: `gsMatrixBlockView.h` in `src/gsMatrix/`
**Purpose**: Block view operations on matrices
**Changes**:
- **Renamed** from `gsMatrixBlockView.h` to `MatrixView.h` for consistency
- **Updated** typedef from `gsEigen::Matrix<index_t,...>` to `gsMatrix<index_t, Dynamic, 1, 0>`
- **Simplified** block view implementation
- **Removed** dependencies on complex matrix operations
- **Rationale**: Block views are fundamental matrix operations that belong in the math foundation

### PointGrid.h
**Original**: `gsPointGrid.h` in `src/gsUtils/`
**Purpose**: Structured point generation utilities
**Changes**:
- **Moved** from `gsUtils` to `Math` as it's mathematically oriented
- **Updated** to use new `gsVector` and `gsMatrix` classes with template defaults
- **Renamed** functions to follow new naming conventions
- **Removed** dependencies on other utility functions
- **Rationale**: Point generation is a mathematical operation that fits better in the Math module

### SparseMatrix.h
**Original**: `gsSparseMatrix.h` in `src/gsMatrix/`
**Purpose**: Sparse matrix class with Eigen integration
**Changes**:
- **Modernized** to use new naming conventions
- **Updated** template parameters and inheritance
- **Simplified** sparse matrix operations
- **Removed** legacy code and complex dependencies
- **Rationale**: Sparse matrices are fundamental linear algebra constructs needed by the math foundation

### Vector.h
**Original**: `gsVector.h` in `src/gsMatrix/` (351 lines)
**Purpose**: Column vector class with Eigen integration
**Changes**:
- **Major modernization**: Updated template parameters to have defaults
- **Changed** from `template<class T, int _Rows, int _Options>` to `template<class T, int _Rows = Dynamic, int _Options = 0>`
- **Simplified** vector-specific operations
- **Removed** complex inheritance patterns and legacy code
- **Added** proper size() method and operator[]
- **Rationale**: Template defaults make vectors much easier to use while maintaining type safety

## Migration Rationale

### Breaking Circular Dependencies
The primary goal of the Math module was to break the circular dependency between `gsCore` and `gsMatrix`:
- **Original**: `gsCore` depended on `gsMatrix` for linear algebra types
- **Original**: `gsMatrix` depended on `gsCore` for basic utilities and configuration
- **New**: `Math` depends only on `Common` (foundation layer)
- **New**: Higher-level modules can depend on `Math` without circular issues

### Template Parameter Defaults
All matrix and vector classes now have sensible template defaults:
- `int _Rows = Dynamic` - Most matrices/vectors have dynamic size
- `int _Cols = Dynamic` - For matrices
- `int _Options = 0` - ColMajor storage (Eigen default)

This makes the classes much easier to use:
```cpp
// Old way - required all template parameters
gsMatrix<double, Dynamic, Dynamic, ColMajor> mat;

// New way - defaults make it simple
gsMatrixd mat;  // gsMatrix<double, Dynamic, Dynamic, 0>
```

### Clean Naming Convention
All code was updated to follow the new naming conventions:
- Headers: PascalCase (e.g., `Matrix.h`, not `gsMatrix.h`)
- Functions: Remove `gs` prefix where appropriate
- Types: Remove `gs` prefix (e.g., `gsMatrix` becomes the class name)
- Convenience typedefs retain `gs` prefix for backward compatibility during transition

### Code Quality Improvements
- Removed legacy C++03 compatibility code
- Simplified complex template metaprogramming
- Better error messages and documentation
- Consistent code formatting
- Modern C++17 features where appropriate

## Testing

The Math module includes comprehensive unit tests in `tst/gismo/Math/Math_test.cpp` that validate:
- All header includes work correctly
- Matrix and vector operations function properly
- Linear algebra utilities work as expected
- Point grid generation produces correct results
- Sparse matrix operations are functional

Run tests with: `ctest -R Math_test`

## Dependencies

**Common** - Foundation layer providing basic utilities and types

The Math module depends only on:
- `gismo::Common` - Foundation utilities and types
- `Eigen3::Eigen` - Linear algebra library
- Standard C++ libraries

## Future Extensions

As the modular architecture matures, additional mathematical utilities may be added to the Math module:
- More sophisticated linear algebra operations
- Additional matrix decompositions
- Extended sparse matrix support
- Geometric transformations and utilities

The Math module will maintain its position as the core linear algebra foundation, with higher-level mathematical modules building upon it.</content>
<parameter name="filePath">/Users/tkloczko/Development/gismo/gismo/src/gismo/Math/README.md