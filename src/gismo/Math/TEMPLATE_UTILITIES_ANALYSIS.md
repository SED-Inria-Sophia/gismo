# Template Utilities Analysis

## **Template Utilities Overview**

This document analyzes template utility classes used throughout the Math module, focusing on their dependencies, template parameter requirements, and integration with Matrix/Vector operations.

## **1. ChangeDim Template Utility**

### **Purpose**
Compile-time template utility for changing dimensions of matrix types while preserving other template parameters.

### **Implementation** (`LinearAlgebra.h` lines 141-150):
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

### **Usage Patterns**

#### **In Matrix.h**:
```cpp
// Line 107: Remove columns (-1 columns)
typedef gsMatrix< T, ChangeDim<_Rows, -1>::D, ChangeDim<_Cols, -1>::D>
    AllMinorMatrixType;

// Line 111: Remove columns only
typedef gsMatrix< T, ChangeDim<_Rows, -1>::D, _Cols>
    ColMinorMatrixType;

// Line 115: Remove rows only
typedef gsMatrix< T, _Rows, ChangeDim<_Cols, -1>::D>
    RowMinorMatrixType;
```

#### **In Vector.h**:
```cpp
// Line 76: Create projection matrix type
typedef gsMatrix< T, ChangeDim<_Rows, -1>::D, ColMajor> Projection_t;
```

### **Template Parameter Dependencies**:
- **Input**: `int Dim` (original dimension), `int Change` (dimension change)
- **Output**: `D` enum value (computed new dimension)
- **Special Cases**: Dynamic dimension handling via specialization

### **Issues Identified**:
1. ✅ **Logic Correct**: Handles negative changes and Dynamic dimensions
2. ✅ **Specialization Present**: Dynamic case properly handled
3. ❌ **Usage Context**: Used in typedef chains that create complex dependencies

## **2. MatrixBlockView Template Class**

### **Purpose**
Provides block-structured view of matrices with references to matrix segments.

### **Template Declaration** (`MatrixBlockView.h` line 31):
```cpp
template <typename MatrixType, bool isConst = false>
class gsMatrixBlockView
```

### **Key Type Definitions**:
```cpp
typedef gsEigen::Block<MatrixType>   block_t;
typedef gsEigen::Block<MatrixType> * block_ptr_t;
typedef gsEigen::Matrix<index_t,Eigen::Dynamic, 1, gsEigen::ColMajor> Vector_t;
```

### **Usage in Matrix/Vector Classes**:

#### **Matrix.h Usage**:
```cpp
// Line 54: Block view typedef
typedef gsMatrixBlockView<Base> BlockView;

// Line 380: Block view method
BlockView blockView(const gsVector<index_t> & rowSizes,
                   const gsVector<index_t> & colSizes);
```

#### **Vector.h Usage**:
```cpp
// Line 57: Block view typedef
typedef gsMatrixBlockView<Base> BlockView;

// Line 189: Block view method
BlockView blockView(const gsVector<index_t> & rowSizes);
```

#### **SparseVector.h Usage**:
```cpp
// Line 56: Block view typedef
typedef gsMatrixBlockView<Base> BlockView;
```

### **Template Dependencies**:
- **Primary**: `MatrixType` (the matrix/vector being viewed)
- **Eigen Types**: `gsEigen::Block<MatrixType>`, `gsEigen::Matrix`
- **Method Parameters**: `gsVector<index_t>` (creates circular dependency!)

### **Issues Identified**:
1. ❌ **Circular Dependency**: Methods use `gsVector<index_t>` parameters
2. ✅ **Template Structure**: Properly parameterized template class
3. ✅ **Eigen Integration**: Correct use of Eigen block types

## **3. BlockDiag Template Class**

### **Purpose**
Eigen expression template for block diagonal replication of matrices.

### **Template Declaration** (`BlockDiag.h` line 57):
```cpp
template<typename MatrixType,int NumBlocks> class BlockDiag
  : public internal::dense_xpr_base< BlockDiag<MatrixType,NumBlocks> >::type
```

### **Template Traits** (`BlockDiag.h` lines 25-49):
```cpp
template<typename MatrixType,int NumBlocks>
struct traits<BlockDiag<MatrixType,NumBlocks> >
 : traits<MatrixType>
{
  typedef typename MatrixType::Scalar Scalar;
  typedef typename traits<MatrixType>::StorageKind StorageKind;
  typedef typename traits<MatrixType>::XprKind XprKind;
  typedef typename ref_selector<MatrixType>::type MatrixTypeNested;

  enum {
    RowsAtCompileTime = NumBlocks==Dynamic || int(MatrixType::RowsAtCompileTime)==Dynamic
                      ? Dynamic
                      : NumBlocks * MatrixType::RowsAtCompileTime,
    ColsAtCompileTime = NumBlocks==Dynamic || int(MatrixType::ColsAtCompileTime)==Dynamic
                      ? Dynamic
                      : NumBlocks * MatrixType::ColsAtCompileTime,
    // ... other traits
  };
};
```

### **Usage in Plugin System**:

#### **EigenDeclarations.h Forward Declaration**:
```cpp
template<typename MatrixType,int RowFactor> class BlockDiag;
```

#### **MatrixAddons.h Plugin Usage**:
```cpp
typedef BlockDiag<Derived,Dynamic> BlockDiagReturnType;
inline const BlockDiagReturnType blockDiag(Index rowFactor) const;
```

### **Template Dependencies**:
- **Primary**: `MatrixType` (matrix being block-diagonalized)
- **Secondary**: `int NumBlocks` (number of diagonal blocks)
- **Eigen Integration**: Extensive use of Eigen traits system
- **Plugin Context**: Used as return type in Eigen plugins

### **Issues Identified**:
1. ✅ **Template Structure**: Properly designed Eigen expression template
2. ❌ **Plugin Integration**: Namespace issues in plugin context (Index vs gsEigen::Index)
3. ✅ **Traits System**: Correct implementation of Eigen traits

## **4. BlockTranspose Template Class**

### **Purpose**
Similar to BlockDiag but for block transpose operations.

### **Template Declaration** (similar structure to BlockDiag):
```cpp
template<typename MatrixType,int NumBlocks> class BlockTranspose
```

### **Usage Pattern**:
```cpp
// EigenDeclarations.h
template<typename MatrixType,int RowFactor> class BlockTranspose;

// MatrixAddons.h
typedef BlockTranspose<Derived,Dynamic> BlockTransposeReturnType;
inline const BlockTransposeReturnType blockTranspose(Index rowFactor) const;
```

### **Issues Identified**:
1. ✅ **Consistent with BlockDiag**: Same design pattern
2. ❌ **Same Plugin Issues**: Namespace problems in plugin context

## **5. Missing RowSelection Template**

### **Declared But Not Implemented**:

#### **EigenDeclarations.h Declaration**:
```cpp
template<typename XprType, typename IndicesType> class RowSelection;
```

#### **MatrixAddons.h Usage**:
```cpp
template<typename IndicesType>
const RowSelection<Derived,IndicesType> selectRows(const IndicesType & ind) const;
```

#### **LinearAlgebra.h Comment**:
```cpp
//#include <gismo/Math/RowSelection.h>  // COMMENTED OUT - FILE MISSING!
```

### **Status**:
- **MISSING IMPLEMENTATION** ❌
- Forward declared but no actual implementation exists
- Referenced in plugin but will cause linker errors
- Commented out inclusion suggests known issue

## **Template Utilities Dependency Analysis**

### **Dependency Graph**:
```
ChangeDim (utility)
├── Used by: Matrix.h (minor matrix typedefs)
├── Used by: Vector.h (projection matrix typedef)
└── Dependencies: None (pure compile-time utility) ✅

MatrixBlockView<MatrixType>
├── Used by: Matrix.h, Vector.h, SparseVector.h
├── Method parameters: gsVector<index_t> (CIRCULAR DEPENDENCY) ❌
└── Dependencies: gsEigen::Block, gsEigen::Matrix ✅

BlockDiag<MatrixType,NumBlocks>
├── Used by: MatrixAddons.h plugin
├── Plugin context issues: Index namespace ❌
└── Dependencies: Eigen expression template system ✅

BlockTranspose<MatrixType,NumBlocks>
├── Used by: MatrixAddons.h plugin
├── Plugin context issues: Index namespace ❌
└── Dependencies: Eigen expression template system ✅

RowSelection<XprType,IndicesType>
├── Used by: MatrixAddons.h plugin
├── Status: MISSING IMPLEMENTATION ❌
└── Dependencies: Unknown (not implemented)
```

### **Critical Issues Summary**:

1. **Circular Dependency**: MatrixBlockView methods use `gsVector<index_t>` parameters
2. **Plugin Namespace Issues**: BlockDiag/BlockTranspose use unqualified `Index` in plugin context
3. **Missing Implementation**: RowSelection template declared but not implemented
4. **Template Parameter Complexity**: ChangeDim usage creates complex nested template expressions

## **Resolution Strategies**

### **1. Fix MatrixBlockView Circular Dependency**:
```cpp
// Current (CIRCULAR):
BlockView blockView(const gsVector<index_t> & rowSizes);

// Solution: Use template parameter instead
template<typename VectorType>
BlockView blockView(const VectorType & rowSizes);

// Or use Eigen vector directly:
BlockView blockView(const gsEigen::VectorXi & rowSizes);
```

### **2. Fix Plugin Namespace Issues**:
```cpp
// Current (BROKEN):
inline const BlockDiagReturnType blockDiag(Index rowFactor) const;

// Solution: Qualify namespace
inline const BlockDiagReturnType blockDiag(gsEigen::Index rowFactor) const;
```

### **3. Handle Missing RowSelection**:
```cpp
// Option 1: Implement RowSelection class
// Option 2: Remove from plugin and EigenDeclarations
// Option 3: Comment out plugin method until implemented
```

### **4. Simplify ChangeDim Usage**:
```cpp
// Current complex usage is actually correct
// ChangeDim is a pure compile-time utility with no runtime dependencies
// No changes needed ✅
```

## **Implementation Priority**

### **HIGH Priority** (Blocking compilation):
1. ✅ **ChangeDim**: No issues, works correctly
2. ❌ **MatrixBlockView circular dependency**: Fix blockView method parameters
3. ❌ **Plugin namespace issues**: Fix Index qualification in MatrixAddons.h
4. ❌ **Missing RowSelection**: Either implement or remove references

### **MEDIUM Priority** (Improvement):
1. **Template documentation**: Add comprehensive template parameter documentation
2. **Usage examples**: Create examples showing proper template utility usage

### **LOW Priority** (Future enhancement):
1. **Additional template utilities**: Consider additional compile-time utilities
2. **Template metaprogramming**: More sophisticated template techniques

## **Expected Outcomes**

After resolving template utility issues:
- ✅ No circular dependencies in template utility usage
- ✅ All Eigen plugin methods work correctly with proper namespace resolution
- ✅ Complete implementation of all declared template classes
- ✅ Clear documentation of template parameter requirements
- ✅ Foundation for adding new template utilities in the future

This analysis provides the foundation for implementing Phase 1 Task 4 template utility fixes.