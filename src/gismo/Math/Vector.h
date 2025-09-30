/** @file Vector.h

    @brief Provides vector operations and utilities.

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

/** @brief
    A vector with arbitrary coefficient type and fixed or dynamic size.

    This is a specialization of gsMatrix for column vectors.

    \tparam T coefficient type
    \tparam _Rows number of rows: an integer or \c Dynamic
    \tparam _Options further options; see Eigen documentation

    \ingroup Matrix
*/
template<class T, int _Rows = gsEigen::Dynamic, int _Options = 0>
class gsVector : public gsMatrix<T, _Rows, 1, _Options>
{
public:
    typedef gsMatrix<T, _Rows, 1, _Options> Base;
    typedef gsVector<T, _Rows, _Options> Self;
    typedef T Scalar_t;

    gsVector() : Base() { }

    gsVector(gsEigen::Index size) : Base(size, 1) { }

    template<typename OtherDerived>
    gsVector(const gsEigen::MatrixBase<OtherDerived>& other) : Base(other) { }

    // Vector-specific operations
    gsEigen::Index size() const { return this->rows(); }

    Scalar_t& operator[](gsEigen::Index i) { return this->coeffRef(i, 0); }
    const Scalar_t& operator[](gsEigen::Index i) const { return this->coeff(i, 0); }

    Scalar_t& operator()(gsEigen::Index i) { return this->coeffRef(i, 0); }
    const Scalar_t& operator()(gsEigen::Index i) const { return this->coeff(i, 0); }
};

} // namespace gismo