# Circular Dependencies Analysis - Math Module

## **Circular Dependency Chains Identified**

### **1. Matrix ↔ Vector Circular Dependency**

**Dependency Chain:**
```
Matrix.h (line 380-381) → uses gsVector<index_t>
    ↓
Vector.h (line 35) → extends gsMatrix<T, _Rows, 1, _Options>
    ↓
Matrix.h → Back to Matrix class definition
```

**Root Cause:**
- `Matrix.h` defines `gsMatrix` class
- `Matrix.h` also uses `gsVector` in method signatures (blockView methods)
- `Vector.h` extends `gsMatrix`
- Creates circular dependency: Matrix needs Vector, Vector needs Matrix

**Specific Problem Code:**
```cpp
// Matrix.h lines 380-381
BlockView blockView(const gsVector<index_t> & rowSizes,
                    const gsVector<index_t> & colSizes)

// Vector.h line 35
template<class T, int _Rows=Dynamic, int _Options=0>
class gsVector : public gsMatrix<T, _Rows, 1, _Options>
```

### **2. LinearAlgebra ↔ Plugin Headers Circular Dependency**

**Dependency Chain:**
```
LinearAlgebra.h (lines 32-34) → defines Eigen plugins
    ↓
#include <Eigen/Core> (line 36) → triggers plugin inclusion
    ↓
MatrixAddons.h + PlainObjectBaseAddons.h → plugin content included
    ↓
Plugin headers reference types not yet defined → potential issues
```

**Root Cause:**
- Eigen plugin system requires plugin headers to be included during Eigen class definition
- Plugin headers contain references that may not be available at inclusion time
- Timing issue: plugins included before full Math module context is established

### **3. AsMatrix ↔ Matrix Implicit Dependency**

**Dependency Chain:**
```
AsMatrix.h → provides matrix view classes
    ↓
Matrix.h (includes AsMatrix.h via LinearAlgebra.h)
    ↓
AsMatrix.h uses gsMatrix types → potential circular reference
```

**Root Cause:**
- AsMatrix provides views/wrappers for Matrix types
- Matrix includes AsMatrix functionality
- Not a direct circular dependency but potential for issues

### **4. Forward Declaration ↔ Implementation Mismatch**

**Dependency Chain:**
```
LinearAlgebra.h (lines 136-137) → forward declares templates
    ↓
template<class T, int _Rows, int _Cols, int _Options> class gsMatrix;
template<class T, int _Rows, int _Options> class gsVector;
    ↓
Matrix.h/Vector.h → provide actual implementations with default parameters
    ↓
Usage sites → expect different template parameters than declared
```

**Root Cause:**
- Forward declarations don't match actual template signatures
- Default parameters only available in implementation, not forward declaration
- Creates mismatch between expected and actual template usage

## **Include Dependency Chain Analysis**

### **Current Include Flow:**
```
Math (main header)
├── LinearAlgebra.h
│   ├── Constants.h ✓ (no deps)
│   ├── EigenDeclarations.h ✓ (no deps)
│   ├── Plugin setup + <Eigen/Core> ⚠️ (plugin timing issue)
│   ├── MatrixBlockView.h ✓ (depends on Eigen, no circular)
│   ├── Matrix.h ❌ CIRCULAR WITH Vector.h
│   ├── Vector.h ❌ CIRCULAR WITH Matrix.h
│   ├── AsMatrix.h ⚠️ (uses Matrix types, potential circular)
│   ├── SparseMatrix.h ❌ (template parameter issues)
│   ├── SparseVector.h ⚠️ (depends on SparseMatrix)
│   ├── SparseSolver.h ❌ (template parameter issues)
│   └── Point.h ⚠️ (extends Vector)
└── Constants.h ✓ (included twice, but safe)
```

### **Problematic Include Pairs:**

1. **Matrix.h ↔ Vector.h**: Direct circular dependency
2. **LinearAlgebra.h → Plugin Headers**: Timing/context issues
3. **SparseMatrix.h → AsConstVector**: Template parameter mismatch
4. **Point.h → Vector.h → Matrix.h**: Dependency chain issues

## **Breaking Circular Dependencies - Strategy**

### **1. Matrix ↔ Vector Resolution**

**Current Problem:**
```cpp
// Matrix.h needs Vector for method signatures
BlockView blockView(const gsVector<index_t> & rowSizes, ...);

// Vector.h extends Matrix
class gsVector : public gsMatrix<T, _Rows, 1, _Options>
```

**Solution Strategy:**
```cpp
// Option A: Forward Declaration in Matrix.h
// Matrix.h
template<class T, int _Rows, int _Options> class gsVector; // Forward declare
class gsMatrix {
    // Use forward declaration in method signatures
    BlockView blockView(const gsVector<index_t> & rowSizes, ...);
};

// Option B: Move problematic methods to separate header
// Matrix.h - core matrix definition only
// MatrixOperations.h - methods that use Vector (included after Vector.h)

// Option C: Use template parameter instead of concrete Vector type
template<typename VectorType>
BlockView blockView(const VectorType & rowSizes, const VectorType & colSizes);
```

### **2. Plugin System Dependency Resolution**

**Current Problem:**
- Plugins included during Eigen/Core inclusion
- Plugin content references may not be available

**Solution Strategy:**
```cpp
// Create controlled plugin inclusion
// EigenIntegration.h
#ifndef GISMO_EIGEN_INTEGRATION_H
#define GISMO_EIGEN_INTEGRATION_H

// Step 1: Setup namespace and types
#include <gismo/Math/EigenDeclarations.h>

// Step 2: Define plugins in controlled manner
#define EIGEN_MATRIXBASE_PLUGIN <gismo/Math/internal/MatrixAddonsImpl.h>
#define EIGEN_PLAINOBJECTBASE_PLUGIN <gismo/Math/internal/PlainObjectAddonsImpl.h>

// Step 3: Include Eigen with plugins active
#include <Eigen/Core>

// Step 4: Cleanup plugin definitions
#undef EIGEN_MATRIXBASE_PLUGIN
#undef EIGEN_PLAINOBJECTBASE_PLUGIN

#endif
```

### **3. Template Parameter Mismatch Resolution**

**Current Problem:**
- Forward declarations have different parameters than usage
- `gsMatrix<T>` used but requires 4 parameters

**Solution Strategy:**
```cpp
// Create consistent template aliases
template<typename T> using gsMatrixD = gsMatrix<T, Dynamic, Dynamic, 0>;
template<typename T> using gsVectorD = gsVector<T, Dynamic, 0>;

// Update all usage sites:
// OLD: gsMatrix<T>
// NEW: gsMatrixD<T> or gsMatrix<T, Dynamic, Dynamic>
```

## **Priority Resolution Order**

### **Phase 1: Critical Circular Dependencies**
1. **Matrix ↔ Vector**: Use forward declarations + method reorganization
2. **Template Parameter Consistency**: Fix all `gsMatrix<T>` usage

### **Phase 2: Plugin System Issues**
3. **Eigen Plugin Integration**: Create controlled inclusion mechanism
4. **Plugin Content Fixes**: Fix namespace and type references in plugins

### **Phase 3: Include Organization**
5. **Include Order Optimization**: Reorganize headers by dependency level
6. **Forward Declaration Centralization**: Create comprehensive Forward.h

## **Detailed Resolution Plan**

### **Step 1: Fix Matrix ↔ Vector Circular Dependency**

**Immediate Action**: Move Vector-dependent methods out of Matrix.h
```cpp
// Matrix.h - core definition only (no Vector references)
template<class T, int _Rows=Dynamic, int _Cols=Dynamic, int _Options=0>
class gsMatrix : public gsEigen::Matrix<T,_Rows, _Cols, _Options> {
    // Core matrix functionality only
    // NO blockView methods here
};

// MatrixExtensions.h - methods requiring Vector (include after Vector.h)
template<class T, int _Rows, int _Cols, int _Options>
typename gsMatrix<T,_Rows,_Cols,_Options>::BlockView
gsMatrix<T,_Rows,_Cols,_Options>::blockView(
    const gsVector<index_t> & rowSizes,
    const gsVector<index_t> & colSizes) {
    return BlockView(*this, rowSizes, colSizes);
}
```

### **Step 2: Fix Template Parameter Usage**

**Search and Replace Pattern**:
```cpp
// Find: gsMatrix<T>
// Replace: gsMatrix<T, Dynamic, Dynamic>

// Find: gsVector<T>
// Replace: gsVector<T, Dynamic>

// Find: gsSparseMatrix<T>
// Replace: gsSparseMatrix<T, 0, index_t>
```

This systematic approach addresses all identified circular dependencies and provides concrete steps for resolution.