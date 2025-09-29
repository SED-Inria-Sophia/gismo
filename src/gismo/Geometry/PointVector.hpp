/** @file PointVector.hpp

    @brief Implementation of Point-Vector interactions (included after both are defined).

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

// This file implements the Point methods that depend on Vector
// It should be included after both Point.h and Vector.h

namespace gismo {
namespace geometry {

template<typename T, int Dim>
Vector<T, Dim> Point<T, Dim>::vectorTo(const Point& other) const
{
    Vector<T, Dim> result;
    for (int i = 0; i < Dim; ++i) {
        result[i] = other.m_coords[i] - m_coords[i];
    }
    return result;
}

template<typename T, int Dim>
Point<T, Dim> Point<T, Dim>::operator+(const Vector<T, Dim>& vec) const
{
    Point result;
    for (int i = 0; i < Dim; ++i) {
        result.m_coords[i] = m_coords[i] + vec[i];
    }
    return result;
}

template<typename T, int Dim>
Point<T, Dim> Point<T, Dim>::operator-(const Vector<T, Dim>& vec) const
{
    Point result;
    for (int i = 0; i < Dim; ++i) {
        result.m_coords[i] = m_coords[i] - vec[i];
    }
    return result;
}

template<typename T, int Dim>
Vector<T, Dim> Point<T, Dim>::operator-(const Point& other) const
{
    Vector<T, Dim> result;
    for (int i = 0; i < Dim; ++i) {
        result[i] = m_coords[i] - other.m_coords[i];
    }
    return result;
}

} // namespace geometry
} // namespace gismo