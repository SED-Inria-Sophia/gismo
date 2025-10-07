/** @file RowSelection.h

    @brief RowSelection extension for Eigen matrix objects

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Created for GISMO Math module architecture refactoring
*/

#pragma once

namespace gsEigen {

/**
  * \class RowSelection
  *
  * \brief Expression for selecting specific rows from a matrix or vector
  *
  * \param MatrixType the type of the object we are selecting from
  * \param IndicesType the type of the indices (e.g., VectorXi, std::vector<int>)
  *
  */
namespace internal {
template<typename MatrixType, typename IndicesType>
struct traits<RowSelection<MatrixType,IndicesType> >
 : traits<MatrixType>
{
  typedef typename MatrixType::Scalar Scalar;
  typedef typename traits<MatrixType>::StorageKind StorageKind;
  typedef typename traits<MatrixType>::XprKind XprKind;
  typedef typename ref_selector<MatrixType>::type MatrixTypeNested;
  typedef typename remove_reference<MatrixTypeNested>::type _MatrixTypeNested;
  typedef typename ref_selector<IndicesType>::type IndicesTypeNested;
  typedef typename remove_reference<IndicesTypeNested>::type _IndicesTypeNested;

  enum {
    RowsAtCompileTime = Dynamic,  // Number of selected rows is dynamic
    ColsAtCompileTime = MatrixType::ColsAtCompileTime,
    MaxRowsAtCompileTime = Dynamic,
    MaxColsAtCompileTime = MatrixType::MaxColsAtCompileTime,
    IsRowMajor = (MatrixType::Flags & RowMajorBit) ? 1 : 0,
    Flags = IsRowMajor ? RowMajorBit : 0
  };
};
}

template<typename MatrixType, typename IndicesType>
class RowSelection
  : public internal::dense_xpr_base< RowSelection<MatrixType,IndicesType> >::type
{
    typedef typename internal::traits<RowSelection>::MatrixTypeNested MatrixTypeNested;
    typedef typename internal::traits<RowSelection>::_MatrixTypeNested _MatrixTypeNested;
    typedef typename internal::traits<RowSelection>::IndicesTypeNested IndicesTypeNested;
    typedef typename internal::traits<RowSelection>::_IndicesTypeNested _IndicesTypeNested;

  public:
    typedef typename internal::dense_xpr_base<RowSelection>::type Base;
    EIGEN_DENSE_PUBLIC_INTERFACE(RowSelection)
    typedef typename internal::remove_all<MatrixType>::type NestedExpression;

    template<typename OriginalMatrixType, typename OriginalIndicesType>
    EIGEN_DEVICE_FUNC
    inline RowSelection(const OriginalMatrixType& matrix, const OriginalIndicesType& indices)
      : m_matrix(matrix), m_indices(indices)
    {
      EIGEN_STATIC_ASSERT((internal::is_same<typename internal::remove_const<MatrixType>::type,OriginalMatrixType>::value),
                          THE_MATRIX_OR_EXPRESSION_THAT_YOU_PASSED_DOES_NOT_HAVE_THE_EXPECTED_TYPE)
      EIGEN_STATIC_ASSERT((internal::is_same<typename internal::remove_const<IndicesType>::type,OriginalIndicesType>::value),
                          THE_INDICES_THAT_YOU_PASSED_DO_NOT_HAVE_THE_EXPECTED_TYPE)
    }

    EIGEN_DEVICE_FUNC
    inline Index rows() const { return m_indices.size(); }

    EIGEN_DEVICE_FUNC
    inline Index cols() const { return m_matrix.cols(); }

    EIGEN_DEVICE_FUNC
    inline Scalar coeff(Index rowId, Index colId) const
    {
      return m_matrix.coeff(m_indices.coeff(rowId), colId);
    }

    template<int LoadMode>
    inline PacketScalar packet(Index rowId, Index colId) const
    {
      return m_matrix.template packet<LoadMode>(m_indices.coeff(rowId), colId);
    }

    EIGEN_DEVICE_FUNC
    const _MatrixTypeNested& nestedExpression() const
    {
      return m_matrix;
    }

    EIGEN_DEVICE_FUNC
    const _IndicesTypeNested& indices() const
    {
      return m_indices;
    }

  protected:
    MatrixTypeNested m_matrix;
    IndicesTypeNested m_indices;
};

namespace internal {
template<typename MatrixType, typename IndicesType>
struct unary_evaluator<RowSelection<MatrixType, IndicesType> >
  : evaluator_base<RowSelection<MatrixType, IndicesType> >
{
  typedef RowSelection<MatrixType, IndicesType> XprType;
  typedef typename XprType::CoeffReturnType CoeffReturnType;
  typedef typename internal::nested_eval<MatrixType, 1>::type MatrixTypeNested;
  typedef typename internal::remove_all<MatrixTypeNested>::type MatrixTypeNestedCleaned;
  typedef typename internal::nested_eval<IndicesType, 1>::type IndicesTypeNested;
  typedef typename internal::remove_all<IndicesTypeNested>::type IndicesTypeNestedCleaned;

  enum {
    CoeffReadCost = evaluator<MatrixTypeNestedCleaned>::CoeffReadCost + evaluator<IndicesTypeNestedCleaned>::CoeffReadCost,
    LinearAccessMask = 0, // Row selection breaks linear access
    Flags = evaluator<MatrixTypeNestedCleaned>::Flags & (HereditaryBits & ~LinearAccessBit),
    Alignment = evaluator<MatrixTypeNestedCleaned>::Alignment
  };

  EIGEN_DEVICE_FUNC explicit unary_evaluator(const XprType& xpr)
    : m_matrix(xpr.nestedExpression()),
      m_indices(xpr.indices()),
      m_matrixImpl(m_matrix),
      m_indicesImpl(m_indices)
  {}

  EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE
  CoeffReturnType coeff(Index rowId, Index colId) const
  {
    return m_matrixImpl.coeff(m_indicesImpl.coeff(rowId), colId);
  }

  template<int LoadMode, typename PacketType>
  EIGEN_STRONG_INLINE
  PacketType packet(Index rowId, Index colId) const
  {
    return m_matrixImpl.template packet<LoadMode>(m_indicesImpl.coeff(rowId), colId);
  }

protected:
  MatrixTypeNested m_matrix;
  IndicesTypeNested m_indices;
  evaluator<MatrixTypeNestedCleaned> m_matrixImpl;
  evaluator<IndicesTypeNestedCleaned> m_indicesImpl;
};

} // namespace internal

/** \memberof gsEigen::MatrixBase
  * \return an expression that selects specific rows from the matrix
  *
  * \param indices Vector of row indices to select (0-based)
  *
  * Example usage:
  * \code
  * MatrixXd mat(4,3);
  * mat << 1, 2, 3,
  *        4, 5, 6,
  *        7, 8, 9,
  *        10,11,12;
  *
  * VectorXi rowIndices(2);
  * rowIndices << 0, 2;  // Select rows 0 and 2
  *
  * auto selectedRows = mat.selectRows(rowIndices);
  * // Result: [1, 2, 3]
  * //         [7, 8, 9]
  * \endcode
  *
  * \sa class RowSelection
  */
template<typename Derived>
template<typename IndicesType>
const RowSelection<Derived,IndicesType>
MatrixBase<Derived>::selectRows(const IndicesType& indices) const
{
  return RowSelection<Derived,IndicesType>(derived(), indices);
}

} // namespace gsEigen