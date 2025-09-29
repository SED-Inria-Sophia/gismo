/** @file Point.h

    @brief Point class definitions for geometric operations.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include "../Core/Core"
#include "../Math/Math"
#include <array>
#include <initializer_list>

namespace gismo {

namespace geometry {

/// Forward declaration
template<typename T, int Dim> class Vector;

/**
 * @brief Point class representing a point in N-dimensional space
 * @tparam T Scalar type (typically real_t)
 * @tparam Dim Dimension of the point (1, 2, 3, etc.)
 */
template<typename T, int Dim>
class Point
{
public:
    /// Scalar type
    typedef T Scalar;

    /// Dimension
    static constexpr int Dimension = Dim;

    /// Container type
    typedef std::array<T, Dim> Container;

private:
    Container m_coords;

public:
    /// Default constructor (zero point)
    Point() { m_coords.fill(T(0)); }

    /// Constructor from initializer list
    Point(std::initializer_list<T> coords)
    {
        GISMO_ASSERT(coords.size() <= Dim, "Too many coordinates");
        m_coords.fill(T(0));
        std::copy(coords.begin(), coords.end(), m_coords.begin());
    }

    /// Copy constructor
    Point(const Point& other) = default;

    /// Assignment operator
    Point& operator=(const Point& other) = default;

    /// Access coordinate by index
    T& operator[](int i) {
        GISMO_ASSERT(i >= 0 && i < Dim, "Index out of bounds");
        return m_coords[i];
    }

    /// Access coordinate by index (const)
    const T& operator[](int i) const {
        GISMO_ASSERT(i >= 0 && i < Dim, "Index out of bounds");
        return m_coords[i];
    }

    /// Get coordinate data
    T* data() { return m_coords.data(); }
    const T* data() const { return m_coords.data(); }

    /// Get dimension
    static constexpr int dim() { return Dim; }

    /// Distance to another point
    T distanceTo(const Point& other) const
    {
        T sum = T(0);
        for (int i = 0; i < Dim; ++i) {
            T diff = m_coords[i] - other.m_coords[i];
            sum += diff * diff;
        }
        return math::sqrt(sum);
    }

    /// Squared distance to another point (faster)
    T squaredDistanceTo(const Point& other) const
    {
        T sum = T(0);
        for (int i = 0; i < Dim; ++i) {
            T diff = m_coords[i] - other.m_coords[i];
            sum += diff * diff;
        }
        return sum;
    }

    /// Vector from this point to another
    Vector<T, Dim> vectorTo(const Point& other) const;

    /// Addition with vector
    Point operator+(const Vector<T, Dim>& vec) const;

    /// Subtraction of vector
    Point operator-(const Vector<T, Dim>& vec) const;

    /// Vector between two points
    Vector<T, Dim> operator-(const Point& other) const;

    /// Equality comparison
    bool operator==(const Point& other) const
    {
        for (int i = 0; i < Dim; ++i) {
            if (!math::isEqual(m_coords[i], other.m_coords[i])) {
                return false;
            }
        }
        return true;
    }

    /// Inequality comparison
    bool operator!=(const Point& other) const
    {
        return !(*this == other);
    }
};

/// Common typedefs
typedef Point<real_t, 1> Point1d;
typedef Point<real_t, 2> Point2d;
typedef Point<real_t, 3> Point3d;

/// Template typedefs
template<typename T> using Point1 = Point<T, 1>;
template<typename T> using Point2 = Point<T, 2>;
template<typename T> using Point3 = Point<T, 3>;

} // namespace geometry

} // namespace gismo