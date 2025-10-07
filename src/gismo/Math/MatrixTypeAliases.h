#pragma once

/** @file MatrixTypeAliases.h

    @brief Backward-compatible type aliases supporting both user-friendly and full template syntax

    This file provides a comprehensive aliasing strategy that supports:
    - User-friendly syntax: gsMatrix<T> (with sensible defaults)
    - Full compatibility: gsMatrix<T, Rows, Cols, Options> (existing codebase)
    - Template deduction: Works seamlessly with both patterns

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Template Resolution Strategy Implementation - Full Compatibility Version
*/

#include <gismo/Common/Common>

// Need Eigen constants - included after Common which sets up gsEigen namespace
#include <Eigen/Core>

namespace gismo {

// =================== FORWARD DECLARATIONS ===================
// These will point to the actual implementation classes

template<typename T, int _Rows, int _Cols, int _Options> class gsMatrixImpl;
template<typename T, int _Rows, int _Options> class gsVectorImpl;
template<typename T, int _Options, typename _Index> class gsSparseMatrixImpl;
template<typename T, int _Options, typename _Index> class gsSparseVectorImpl;


// =================== BACKWARD COMPATIBLE PRIMARY ALIASES ===================
// These support BOTH single-parameter AND multi-parameter usage

/**
 * @brief Primary matrix alias - fully backward compatible
 *
 * Supports both:
 * - gsMatrix<double>                             (user-friendly, defaults applied)
 * - gsMatrix<double, 3, 3, ColMajor>            (full specification, existing code)
 * - gsMatrix<double, Dynamic, Dynamic, ColMajor> (explicit dynamics, existing code)
 */
template<typename T,
         int _Rows = gsEigen::Dynamic,
         int _Cols = gsEigen::Dynamic,
         int _Options = gsEigen::ColMajor>
using gsMatrix = gsMatrixImpl<T, _Rows, _Cols, _Options>;

/**
 * @brief Primary vector alias - fully backward compatible
 *
 * Supports both:
 * - gsVector<double>                    (user-friendly, dynamic size)
 * - gsVector<double, 3>                (fixed size, existing code)
 * - gsVector<double, Dynamic, ColMajor> (full specification, existing code)
 */
template<typename T,
         int _Rows = gsEigen::Dynamic,
         int _Options = gsEigen::ColMajor>
using gsVector = gsVectorImpl<T, _Rows, _Options>;

/**
 * @brief Primary sparse matrix alias - fully backward compatible
 *
 * Supports both:
 * - gsSparseMatrix<double>               (user-friendly, ColMajor default)
 * - gsSparseMatrix<double, RowMajor>     (full specification, existing code)
 */
template<typename T,
         int _Options = gsEigen::ColMajor>
using gsSparseMatrix = gsSparseMatrixImpl<T, _Options, index_t>;

/**
 * @brief Primary sparse vector alias - fully backward compatible
 *
 * Supports both:
 * - gsSparseVector<double>               (user-friendly, ColMajor default)
 * - gsSparseVector<double, RowMajor>     (full specification, existing code)
 */
template<typename T,
         int _Options = gsEigen::ColMajor>
using gsSparseVector = gsSparseVectorImpl<T, _Options, index_t>;



// =================== END OF TYPE ALIASES ===================
// All aliases above provide user-friendly syntax while maintaining
// backward compatibility with existing GISMO code

} // namespace gismo

// =================== COMPATIBILITY VALIDATION ===================
/*

This design ensures full backward compatibility:

1. Existing code continues to work:
   - gsMatrix<double, 3, 3, ColMajor> mat;        ✓ Works
   - gsVector<double, Dynamic, ColMajor> vec;     ✓ Works
   - gsSparseMatrix<double, RowMajor> sparse;     ✓ Works

2. New user-friendly syntax works:
   - gsMatrix<double> mat;                        ✓ Works (defaults to Dynamic, Dynamic, ColMajor)
   - gsVector<double> vec;                        ✓ Works (defaults to Dynamic, ColMajor)
   - gsSparseMatrix<double> sparse;               ✓ Works (defaults to ColMajor)

3. Template argument deduction works:
   - Function parameters can use either syntax
   - Return types can use either syntax
   - Template specializations work correctly

4. All existing template metraprogramming continues to work:
   - Eigen::NumTraits specializations
   - Matrix operations and arithmetic
   - Template-based algorithms

*/