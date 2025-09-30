/** @file SparseMatrix.h

    @brief Provides declaration of the gsSparseMatrix class.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include <gismo/Math/LinearAlgebra.h>
#include <vector>

namespace gismo
{

/**
   @brief Class that provides a container for triplets (i,j,value) to be
   filled in a sparse matrix.

   Constructing a sparse matrix from triplets is much faster than
   inserting directly.  Use gsSparseMatrix().setFrom(gsSparseEntries)
   to pass the triplets to the matrix.

   \tparam T coefficient type
   \ingroup Matrix
*/
template<typename T>
class gsSparseEntries : public std::vector<gsEigen::Triplet<T,index_t> >
{
public:
    typedef gsEigen::Triplet<T,index_t> Triplet;
    typedef std::vector<gsEigen::Triplet<T,index_t> > Base;

    typedef typename Base::iterator iterator;

    inline void add( int i, int j, T value )
    { this->push_back( Triplet(i,j,value) ); }

    inline void reserve(const size_t sz)
    { Base::reserve(sz); }

    inline void clear()
    { Base::clear(); }
};

/**
   @brief Sparse matrix class.

   This class provides an interface to Eigen::SparseMatrix from the Eigen
   linear algebra library.

   \tparam T coefficient type
   \tparam _Options further options; see Eigen documentation

   \ingroup Matrix
*/
template<typename T, int _Options = 0>
class gsSparseMatrix : public gsEigen::SparseMatrix<T, _Options>
{
public:
    typedef gsEigen::SparseMatrix<T, _Options> Base;
    typedef gsSparseMatrix<T, _Options> Self;
    typedef T Scalar_t;

    gsSparseMatrix() : Base() { }

    gsSparseMatrix(gsEigen::Index rows, gsEigen::Index cols) : Base(rows, cols) { }

    template<typename OtherDerived>
    gsSparseMatrix(const gsEigen::SparseMatrixBase<OtherDerived>& other) : Base(other) { }

    // Utility functions
    gsEigen::Index rows() const { return Base::rows(); }
    gsEigen::Index cols() const { return Base::cols(); }
    gsEigen::Index nonZeros() const { return Base::nonZeros(); }

    // Triplet-based construction
    void setFrom(const gsSparseEntries<T>& entries)
    {
        this->setFromTriplets(entries.begin(), entries.end());
    }
};

// Common sparse matrix type alias
typedef gsSparseMatrix<real_t> gsSparseMatrixd;

} // namespace gismo