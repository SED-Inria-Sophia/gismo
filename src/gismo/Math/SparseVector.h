/** @file gsSparseVector.h

    @brief Provides declaration of SparseVector class (wrapping Eigen)

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/


# pragma once

// Assumes that gsEigen library has been already included


namespace gismo
{

/** @brief
    Sparse vector class, based on gsEigen::SparseVector.

    \tparam T coefficient type
    \tparam _Option zero is ColMajor order.
    \tparam _Index index type

    \ingroup Matrix
*/

template<typename T, int _Options, typename _Index>
class gsSparseVectorImpl : public gsEigen::SparseVector<T,_Options,_Index>
{
public:

    typedef gsEigen::SparseVector<T,_Options,_Index> Base;

    // Self type
    typedef gsSparseVectorImpl<T, _Options, _Index> Self;

    typedef typename gsEigen::SparseVector<T,_Options,_Index>::InnerIterator InnerIterator;

    class iterator : public InnerIterator
    {
    public:
        iterator() = default;
        iterator(const gsSparseVectorImpl & sv) : InnerIterator(sv) { }

        inline T& operator[](size_t i)
        { return const_cast<T&>(*(this->m_values+i)); }
    };

    // Type pointing to a block of the sparse vector
    typedef typename gsEigen::Block<Base> Block;

    // Type pointing to a block view of the sparse vector
    typedef gsMatrixBlockView<Base> BlockView;

    /// Shared pointer for gsSparseVectorImpl
    typedef memory::shared_ptr< Self > Ptr;

    /// Unique pointer for gsSparseVectorImpl
    typedef memory::unique_ptr< Self > uPtr;

public:
    gsSparseVectorImpl() : Base() { }
    gsSparseVectorImpl(_Index rows) : Base(rows) { }

    /// This constructor allows constructing a gsSparseVectorImpl from
    /// gsEigen expressions
    template<typename OtherDerived>
    gsSparseVectorImpl(const gsEigen::EigenBase<OtherDerived>& other)  : Base(other) { }

    /// This constructor allows constructing a gsSparseVectorImpl from
    /// another sparse expression
    template<typename OtherDerived>
    gsSparseVectorImpl(const gsEigen::MatrixBase<OtherDerived>& other)  : Base(other) { }

    /// This constructor allows constructing a gsSparseVectorImpl from
    /// another sparse expression
    template<typename OtherDerived>
    gsSparseVectorImpl(const gsEigen::SparseMatrixBase<OtherDerived>& other)  : Base(other) { }

    /// This constructor allows constructing a gsSparseVectorImpl from
    /// another sparse expression
    template<typename OtherDerived>
    gsSparseVectorImpl(const gsEigen::ReturnByValue<OtherDerived>& other)  : Base(other) { }

    ~gsSparseVectorImpl() { }

#if !EIGEN_HAS_RVALUE_REFERENCES
    // Using the assignment operators of gsEigen
    // Note: using Base::operator=; is ambiguous in MSVC
#ifdef _MSC_VER
    template <class gsEigenExpr>
    gsSparseVectorImpl& operator= (const gsEigenExpr & other)
    {
        this->Base::operator=(other);
        return *this;
    }
#else
    using Base::operator=;
#endif

#else

    // Avoid default keyword for MSVC<2013
    // https://msdn.microsoft.com/en-us/library/hh567368.aspx
    gsSparseVectorImpl(const gsSparseVectorImpl& other) : Base(other)
    { Base::operator=(other); }

    gsSparseVectorImpl& operator= (const gsSparseVectorImpl & other)
    { Base::operator=(other); return *this; }

    gsSparseVectorImpl(gsSparseVectorImpl&& other)
    { operator=(std::forward<gsSparseVectorImpl>(other)); }

    gsSparseVectorImpl & operator=(gsSparseVectorImpl&& other)
    {
        this->swap(other);
        other.clear();
        return *this;
    }

#endif

    void clear()
    {
        this->resize(0);
        this->data().squeeze();
    }

    iterator begin() const { return iterator(*this); }

    inline T   at (_Index i ) const { return this->coeff(i); }
    inline T & at (_Index i ) { return this->coeffRef(i); }

    inline T    operator () (_Index i) const { return this->coeff(i); }
    inline T  & operator () (_Index i) { return this->coeffRef(i); }

    inline T    operator [] (_Index i) const { return this->coeff(i); }
    inline T  & operator [] (_Index i) { return this->coeffRef(i); }

    /// Clone function. Used to make a copy of the matrix
    gsSparseVectorImpl * clone() const
    { return new gsSparseVectorImpl(*this); }

}; // class gsSparseVectorImpl




} // namespace gismo
