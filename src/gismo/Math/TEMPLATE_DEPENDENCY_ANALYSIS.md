# Math Module Template Dependency Analysis

## Current Template Dependency Issues Identified

### 1. **Template Parameter Missing Issues**

**Problem**: Multiple files use `gsMatrix<T>` and `gsVector<T>` but these templates require more parameters.

**Current Definition**:
```cpp
// Matrix.h line 36
template<class T, int _Rows=Dynamic, int _Cols=Dynamic, int _Options=0>
class gsMatrix : public gsEigen::Matrix<T,_Rows, _Cols, _Options>

// Vector.h line 34
template<class T, int _Rows=Dynamic, int _Options=0>
class gsVector : public gsMatrix<T, _Rows, 1, _Options>
```

**Problematic Usage Locations**:
- `Matrix.h` lines: 263, 275, 289, 395, 426 - uses `gsMatrix<T>` (missing _Rows, _Cols, _Options)
- `PointGrid.h` lines: 53, 58, 62, 73, 84, 87, 100, 110, 112 - same issue
- `AsMatrix.h` lines: 68, 70, 309, 311 - same issue

### 2. **Circular Include Dependencies**

**Dependency Chain Analysis**:
```
Math (main header)
  ↓
LinearAlgebra.h (line 17)
  ↓
Matrix.h (line 215) + Vector.h (line 216) + SparseMatrix.h (line 218)
  ↓
Matrix.h uses gsVector<index_t> (lines 380-381)
Vector.h depends on gsMatrix (line 35: extends gsMatrix)
  ↓
Circular: Matrix → Vector → Matrix
```

**Forward Declaration Issues**:
```cpp
// LinearAlgebra.h lines 136-137
template<class T, int _Rows, int _Cols, int _Options> class gsMatrix;
template<class T, int _Rows, int _Options> class gsVector;

// But actual usage expects default parameters:
gsMatrix<T> // Should be gsMatrix<T,Dynamic,Dynamic,0>
gsVector<T> // Should be gsVector<T,Dynamic,0>
```

### 3. **Eigen Plugin System Issues**

**Plugin Definition Context Problem**:
```cpp
// LinearAlgebra.h lines 32-34
#define EIGEN_MATRIXBASE_PLUGIN <gismo/Math/MatrixAddons.h>
#define EIGEN_PLAINOBJECTBASE_PLUGIN <gismo/Math/PlainObjectBaseAddons.h>
#include <gismo/Math/EigenDeclarations.h>
```

**Issues Identified**:
1. **MatrixAddons.h** content assumes it's inside an Eigen Matrix class but uses:
   - `internal::` (should be `gsEigen::internal::`)
   - `Derived` (should be available in plugin context)
   - `Index` (should be `gsEigen::Index`)

2. **Plugin Inclusion Order**: Plugins are included in global namespace before Eigen classes are defined

### 4. **ChangeDim Template Utility Issues**

**Current Implementation** (LinearAlgebra.h lines 140-148):
```cpp
template <int Dim, int Change>
struct ChangeDim
{
    enum { D = Change+Dim<0 ? 0 : Dim + Change };
};
template <int Change>
struct ChangeDim<Dynamic, Change>
{
    enum { D = Dynamic };
};
```

**Usage in Matrix.h** (lines 107-115):
```cpp
// These fail because they use only 3 template parameters instead of 4
typedef gsMatrix< T, ChangeDim<_Rows, -1>::D, ChangeDim<_Cols, -1>::D>
    FirstMinorMatrixType;

typedef gsMatrix< T, ChangeDim<_Rows, -1>::D, _Cols>
    RowMinorMatrixType;

typedef gsMatrix< T, _Rows, ChangeDim<_Cols, -1>::D>
    ColMinorMatrixType;
```

### 5. **Sparse Matrix Template Issues**

**SparseMatrix Definition** (SparseMatrix.h line 172):
```cpp
template<typename T, int _Options, typename _Index>
class gsSparseMatrix : public gsEigen::SparseMatrix<T,_Options,_Index>
```

**Problematic Usage** (SparseMatrix.h line 429, SparseSolver.h line 92):
```cpp
gsSparseMatrix<>::iterator mIt;           // Missing all template parameters
typedef gsSparseMatrix<T> MatrixT;        // Missing _Options and _Index
```

### 6. **AsConstVector Template Issues**

**Definition** (AsMatrix.h line 284):
```cpp
template<class T, int _Rows>
class gsAsConstVector : public gsAsConstMatrix<T,_Rows,1>
```

**Problematic Usage** (SparseMatrix.h lines 391, 398):
```cpp
gsAsConstVector<_Index> nonZerosPerCol()  // Missing _Rows parameter
```

## **Template Dependency Graph**

```
Core Templates:
  gsMatrix<T, _Rows=Dynamic, _Cols=Dynamic, _Options=0>
    ↑
  gsVector<T, _Rows=Dynamic, _Options=0> (depends on gsMatrix)
    ↑
  gsPoint<T, d> (depends on gsVector)

Support Templates:
  gsAsMatrix<T, _Rows, _Cols>
  gsAsConstMatrix<T, _Rows, _Cols>
  gsAsVector<T, _Rows>
  gsAsConstVector<T, _Rows>

Sparse Templates:
  gsSparseMatrix<T, _Options, _Index>
  gsSparseVector<T, _Index>

Utility Templates:
  ChangeDim<Dim, Change>
  ChangeDim<Dynamic, Change> (specialization)

Block Templates:
  BlockDiag<MatrixType, NumBlocks>
  BlockTranspose<MatrixType, NumBlocks>
  gsMatrixBlockView<Base>
```

## **Include Order Dependencies**

**Current Order** (problematic):
```cpp
Math
├── LinearAlgebra.h
│   ├── Constants.h ✓
│   ├── EigenDeclarations.h ✓
│   ├── <Eigen/Core> ✓
│   ├── MatrixBlockView.h ✓
│   ├── Matrix.h ❌ (uses gsVector before definition)
│   ├── Vector.h ❌ (circular dependency with Matrix)
│   ├── AsMatrix.h ✓
│   ├── SparseMatrix.h ❌ (uses gsAsConstVector incorrectly)
│   └── ... other headers
└── Constants.h ✓
```

**Required Order** (fixed):
```cpp
1. Forward.h          # All forward declarations
2. EigenIntegration.h # Eigen setup + controlled plugin inclusion
3. Types.h            # Type aliases and utilities
4. Matrix.h           # Core matrix (uses forward declarations)
5. Vector.h           # Vector (extends Matrix, now safe)
6. AsMatrix.h         # Matrix views/wrappers
7. SparseMatrix.h     # Sparse operations (uses proper types)
8. Operations.h       # Mathematical operations
9. Utilities.h        # Higher-level utilities
```

## **Root Causes Summary**

1. **Missing Default Parameters**: Many usages assume `gsMatrix<T>` is valid but need explicit parameters
2. **Circular Dependencies**: Matrix and Vector classes reference each other
3. **Plugin Context Issues**: Eigen plugins included in wrong context
4. **Template Specialization Problems**: ChangeDim and other utilities don't handle template parameters correctly
5. **Inconsistent Parameter Counts**: Different template classes expect different numbers of parameters

## **Priority Issues to Address**

### **HIGH Priority** (Compilation Blockers):
1. Fix all `gsMatrix<T>` → `gsMatrix<T,Dynamic,Dynamic>` usage
2. Resolve Matrix ↔ Vector circular dependency
3. Fix Eigen plugin inclusion context

### **MEDIUM Priority** (Functionality Issues):
1. Fix ChangeDim template utility usage in minor matrix operations
2. Fix sparse matrix template parameter issues
3. Resolve AsConstVector parameter issues

### **LOW Priority** (Optimization):
1. Optimize include order for compilation speed
2. Add type aliases for common matrix types
3. Create explicit template instantiations

## **Next Steps**

Based on this analysis, the next task should focus on:
1. Creating proper forward declarations to break circular dependencies
2. Fixing the plugin system integration
3. Systematically updating all template parameter usage sites

This analysis forms the foundation for implementing the template dependency resolution plan.