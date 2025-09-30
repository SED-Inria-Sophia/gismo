/** @file Matrix.h

    @brief Provides declaration of Matrix class.

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

/** @brief
    A matrix with arbitrary coefficient type and fixed or dynamic size.

    This class provides an interface to Eigen::Matrix from the Eigen
    linear algebra library. Most operations from Eigen are supported
    on a gsMatrix.

    See therefore also the Eigen documentation for dense matrices,
    http://eigen.tuxfamily.org/dox/group__QuickRefPage.html

    \tparam T coefficient type
    \tparam _Rows number of rows: an integer or \c Dynamic
    \tparam _Cols number of rows: an integer or \c Dynamic
    \tparam _Options further options; see Eigen documentation

    \ingroup Matrix
*/
template<class T, int _Rows = gsEigen::Dynamic, int _Cols = gsEigen::Dynamic, int _Options = 0>
class gsMatrix : public gsEigen::Matrix<T,_Rows, _Cols, _Options>
{
public:
    // Base is the dense matrix class of Eigen
    typedef gsEigen::Matrix<T,_Rows, _Cols, _Options> Base;

    // Self type
    typedef gsMatrix<T,_Rows, _Cols, _Options> Self;

    // The type of the coefficients of the matrix
    typedef T Scalar_t;

    // Constructors
    gsMatrix() : Base() { }

    gsMatrix(gsEigen::Index rows, gsEigen::Index cols) : Base(rows, cols) { }

    gsMatrix(gsEigen::Index size) : Base(size) { }

    template<typename OtherDerived>
    gsMatrix(const gsEigen::MatrixBase<OtherDerived>& other) : Base(other) { }

    template<typename OtherDerived>
    gsMatrix(const gsEigen::EigenBase<OtherDerived>& other) : Base(other) { }

    // Assignment operators
    template<typename OtherDerived>
    gsMatrix& operator=(const gsEigen::MatrixBase<OtherDerived>& other)
    {
        this->Base::operator=(other);
        return *this;
    }

    // Utility functions
    gsEigen::Index rows() const { return Base::rows(); }
    gsEigen::Index cols() const { return Base::cols(); }
    gsEigen::Index size() const { return Base::size(); }

    // Data access
    Scalar_t* data() { return Base::data(); }
    const Scalar_t* data() const { return Base::data(); }
};

// Common matrix type aliases
typedef gsMatrix<real_t, gsEigen::Dynamic, gsEigen::Dynamic, 0> gsMatrixd;
typedef gsMatrix<real_t, gsEigen::Dynamic, 1, 0> gsVectord;
typedef gsMatrix<real_t, 1, gsEigen::Dynamic, 0> gsRowVectord;
typedef gsMatrix<real_t, 2, 1, 0> gsVector2d;
typedef gsMatrix<real_t, 3, 1, 0> gsVector3d;
typedef gsMatrix<real_t, 4, 1, 0> gsVector4d;
typedef gsMatrix<real_t, 2, 2, 0> gsMatrix22d;
typedef gsMatrix<real_t, 3, 3, 0> gsMatrix33d;
typedef gsMatrix<real_t, 4, 4, 0> gsMatrix44d;

} // namespace gismo