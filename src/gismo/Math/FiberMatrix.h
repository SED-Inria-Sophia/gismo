/** @file FiberMatrix.h

    @brief A specialized sparse matrix class which stores separately
    each fiber.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include <gismo/Math/LinearAlgebra.h>

namespace gismo
{

/**
 * \brief A specialized sparse matrix class which stores each row
 *  as a separate sparse vector.
 *
 *  This allows efficient row resizing and insertion
 *  operations, particularly for knot insertion algorithms.
 */
template <class T, int Major = ColMajor> // RowMajor==0, ColMajor==1
class gsFiberMatrix
{
    static constexpr bool IsRowMajor = (Major==RowMajor);
public:
    typedef gsSparseVector<T> Fiber;
    typedef typename Fiber::InnerIterator iterator;

    struct RowBlockXpr
    {
        gsFiberMatrix & mat;
        index_t start, num;

        RowBlockXpr(const gsFiberMatrix& _mat, index_t _start, index_t _num)
        : mat(const_cast<gsFiberMatrix&>(_mat)), start(_start), num(_num)
        { }

        operator gsFiberMatrix () const
        { return gsFiberMatrix(*this); }

        void operator=(const gsFiberMatrix & other)
        {
            GISMO_ASSERT( num <= (index_t)mat.m_fibers.size() - start, "Invalid block-assignment size");
            GISMO_ASSERT( num == (index_t)other.m_fibers.size(), "Size of the other matrix does not fit");
            for (index_t i = 0; i != num; ++i)
            {
                delete mat.m_fibers[start + i];
                mat.m_fibers[start + i] = new Fiber(*other.m_fibers[i]);
            }
        }
    };

    gsFiberMatrix()
    { }

    gsFiberMatrix(index_t rows, index_t cols)
    : m_fibers(IsRowMajor?rows:cols)
    {
        for (size_t i = 0; i < m_fibers.size(); ++i)
            m_fibers[i] = new Fiber(cols);
    }

    gsFiberMatrix(const gsFiberMatrix& other)
    : m_fibers(other.outerSize())
    {
        for (size_t i = 0; i < m_fibers.size(); ++i)
            m_fibers[i] = new Fiber( *other.m_fibers[i] );
    }

    gsFiberMatrix(const RowBlockXpr& rowxpr)
    : m_fibers(rowxpr.num)
    {
        for (index_t i = 0; i < rowxpr.num; ++i)
            m_fibers[i] = new Fiber( *rowxpr.mat.m_fibers[rowxpr.start + i] );
    }

    ~gsFiberMatrix()
    {
        clear();
    }

    iterator begin(index_t j) const { return iterator(*m_fibers[j]); }

#if EIGEN_HAS_RVALUE_REFERENCES
    gsFiberMatrix(gsFiberMatrix&& other) : m_fibers(give(other.m_fibers)) {}

    /// Assignment operator
    gsFiberMatrix& operator= ( const gsFiberMatrix& other )
    {
        clear();
        m_fibers.resize(other.outerSize());
        for (size_t i = 0; i < m_fibers.size(); ++i)
            m_fibers[i] = new Fiber( *other.m_fibers[i] );
        return *this;
    }

    /// Move assignment operator
    gsFiberMatrix& operator= ( gsFiberMatrix&& other )
    {
        clear();
        m_fibers = give(other.m_fibers);
        return *this;
    }
#else
    gsFiberMatrix& operator= (gsFiberMatrix other)
    {
        this->swap( other );
        return *this;
    }
#endif

    gsFiberMatrix& operator= (const RowBlockXpr& rowxpr)
    {
        gsFiberMatrix temp(rowxpr);
        this->swap( temp );
        return *this;
    }

    /// Write Access
    T & operator()(const index_t i, const index_t j)
    {
        const index_t outer = (IsRowMajor ? i : j );
        const index_t inner = (IsRowMajor ? j : i );
        return (*m_fibers[outer])[inner]; // ouch
    }

    /// Read Access
    T operator()(const index_t i, const index_t j) const
    {
        const index_t outer = (IsRowMajor ? i : j );
        const index_t inner = (IsRowMajor ? j : i );
        return m_fibers[outer]->coeff(inner);
    }

    T coeff(const index_t i, const index_t j) const
    { return this->operator()(i,j); }

    void setZero()
    {
        for (size_t i = 0; i < m_fibers.size(); ++i)
            m_fibers[i]->setZero();
    }

    void setOnes()
    {
        for (size_t i = 0; i < m_fibers.size(); ++i)
            m_fibers[i]->fill(T(1));
    }

    void clear()
    {
        for (size_t i = 0; i < m_fibers.size(); ++i)
            delete m_fibers[i];
        m_fibers.clear();
    }

    /// Removes fiber \a i
    void removeFiber(index_t i)
    {
        GISMO_ASSERT(i < (index_t)m_fibers.size(), "Invalid fiber");
        delete m_fibers[i];
        m_fibers.erase(m_fibers.begin()+i);
    }

    /// Inserts a copy of fiber \a fiber at position \a i.
    void insertFiber(index_t i, const Fiber & fiber)
    {
        GISMO_ASSERT(i <= (index_t)m_fibers.size(), "Invalid fiber");
        m_fibers.insert(m_fibers.begin()+i, new Fiber(fiber));
    }

    /// Gives back fiber \a i
    Fiber * releaseFiber(index_t i)
    {
        GISMO_ASSERT(i < (index_t)m_fibers.size(), "Invalid fiber");
        Fiber * temp = m_fibers[i];
        m_fibers.erase(m_fibers.begin()+i);
        return temp;
    }

    /// Returns a row-block starting at row \a start composed of \a num rows.
    const RowBlockXpr rowBlock(index_t start, index_t num) const
    {
        return RowBlockXpr(*this, start, num);
    }

    RowBlockXpr rowBlock(index_t start, index_t num)
    {
        return RowBlockXpr(*this, start, num);
    }

    index_t outerSize() const
    { return m_fibers.size(); }

    index_t innerSize() const
    {
        return (outerSize() == 0 ? 0 :
                m_fibers[0]->size());
    }

    index_t size() const
    {
        if ( outerSize() == 0 )
            return 0;
        return outerSize() * innerSize();
    }

    index_t cols() const { return IsRowMajor ? innerSize() : outerSize(); }

    index_t rows() const { return IsRowMajor ? outerSize() : innerSize(); }

    RowBlockXpr middleRows(index_t start, index_t num)
    { return rowBlock(start, num); }

    Fiber & fiber(const index_t k) { return *m_fibers[k]; }

    const Fiber & fiber(const index_t k) const { return *m_fibers[k]; }

    void reserve(const index_t nnz)
    {
        for (size_t i = 0; i < m_fibers.size(); ++i)
            m_fibers[i]->reserve(nnz);
    }

    gsSparseMatrix<T> toSparse() const
    {
        gsSparseMatrix<T> result(rows(), cols());
        std::vector<gsEigen::Triplet<T> > tripletList;
        tripletList.reserve(20*m_fibers.size());

        for (index_t i = 0; i!=outerSize(); ++i)
        {
            for (typename Fiber::InnerIterator it(*m_fibers[i]); it; ++it)
            {
                if ( IsRowMajor )
                    tripletList.push_back(gsEigen::Triplet<T>(i, it.index(), it.value()));
                else
                    tripletList.push_back(gsEigen::Triplet<T>(it.index(), i, it.value()));
            }
        }
        result.setFromTriplets(tripletList.begin(), tripletList.end());
        return result;
    }

    void swap(gsFiberMatrix& other)
    {
        m_fibers.swap(other.m_fibers);
    }

    void resize(index_t rows, index_t cols)
    {
        GISMO_ASSERT(rows >=0 && cols >= 0, "Dimensions must be non-negative");

        clear();
        m_fibers.resize(IsRowMajor?rows:cols);
        for (size_t i = 0; i < m_fibers.size(); ++i)
            m_fibers[i] = new Fiber(IsRowMajor?cols:rows);
    }

    void conservativeResize(index_t rows, index_t cols)
    {
        GISMO_ASSERT(rows >=0 && cols >= 0, "Dimensions must be non-negative");

        const index_t old_osize = outerSize();
        const index_t old_isize = innerSize();
        const index_t new_osize = IsRowMajor?rows:cols;
        const index_t new_isize = IsRowMajor?cols:rows;

        m_fibers.resize(new_osize);

        for (index_t i = old_osize; i < new_osize; ++i)
            m_fibers[i] = new Fiber(new_isize);

        if (new_isize > old_isize) // Extend existing fibers
            for (index_t i = 0; i != math::min(new_osize, old_osize); ++i)
                m_fibers[i]->conservativeResize(new_isize);
        else if (new_isize < old_isize) // Shrink existing fibers
            for (index_t i = 0; i != math::min(new_osize, old_osize); ++i)
                m_fibers[i]->conservativeResize(new_isize);
    }

    void print(std::ostream & os) const
    {
        for ( size_t i = 0; i != m_fibers.size(); ++i)
        {
            os << *m_fibers[i] <<"\n";
        }
    }

private:
    std::vector<Fiber*> m_fibers;
};

/// Print operator
template<class T, int Major>
std::ostream &operator<<(std::ostream &os, const gsFiberMatrix<T,Major>& m)
{
    m.print(os); return os;
}

} // namespace gismo