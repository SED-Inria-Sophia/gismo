/** @file MatrixView.h

    @brief Provides matrix view and block operations.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include <Matrix.h>

namespace gismo
{

/** \brief Represents a block-view of the given matrix
 *
 * The blocks are references to the matrix segments. Each block
 * can be seen as a standalone matrix.
 *
 * \tparam T coefficient type
 * \ingroup Matrix
 */
template <typename MatrixType>
class gsMatrixBlockView
{
public:
    typedef gsEigen::Block<MatrixType> block_t;
    typedef gsEigen::Block<MatrixType> * block_ptr_t;
    typedef gsMatrix<index_t, gsEigen::Dynamic, 1, 0> Vector_t;

public:
    gsMatrixBlockView() : m_rowSize(0), m_colSize(0) { }

    /** \brief Creates a block-view of the given matrix
     *
     * \param[in] matrix   the matrix to be "partitioned" into blocks
     * \param[in] rowSizes the sizes of the row blocks
     * \param[in] colSizes the sizes of the column blocks
     */
    template <typename MatrixPtr>
    gsMatrixBlockView(MatrixPtr matrix,
                      const Vector_t& rowSizes,
                      const Vector_t& colSizes)
    : m_matrix(matrix), m_rowSizes(rowSizes), m_colSizes(colSizes)
    {
        m_rowSize = rowSizes.size();
        m_colSize = colSizes.size();
        m_blocks.resize(m_rowSize * m_colSize);
        // Implementation would set up block references
    }

    /** \brief Returns the (i,j)-th block */
    block_t& operator()(index_t i, index_t j)
    {
        return m_blocks[i * m_colSize + j];
    }

    /** \brief Returns the (i,j)-th block */
    const block_t& operator()(index_t i, index_t j) const
    {
        return m_blocks[i * m_colSize + j];
    }

private:
    MatrixType* m_matrix;
    Vector_t m_rowSizes;
    Vector_t m_colSizes;
    index_t m_rowSize;
    index_t m_colSize;
    std::vector<block_t> m_blocks;
};

} // namespace gismo