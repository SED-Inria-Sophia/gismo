/** @file Vector.h

    @brief Vector class definitions for geometric operations.

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
template<typename T, int Dim> class Point;

/**
 * @brief Vector class representing a vector in N-dimensional space
 * @tparam T Scalar type (typically real_t)
 * @tparam Dim Dimension of the vector (1, 2, 3, etc.)
 */
template<typename T, int Dim>
class Vector
{
public:
    /// Scalar type
    typedef T Scalar;

    /// Dimension
    static constexpr int Dimension = Dim;

    /// Container type
    typedef std::array<T, Dim> Container;

private:
    Container m_components;

public:
    /// Default constructor (zero vector)
    Vector() { m_components.fill(T(0)); }

    /// Constructor from initializer list
    Vector(std::initializer_list<T> components)
    {
        GISMO_ASSERT(components.size() <= Dim, "Too many components");
        m_components.fill(T(0));
        std::copy(components.begin(), components.end(), m_components.begin());
    }

    /// Copy constructor
    Vector(const Vector& other) = default;

    /// Assignment operator
    Vector& operator=(const Vector& other) = default;

    /// Access component by index
    T& operator[](int i) {
        GISMO_ASSERT(i >= 0 && i < Dim, "Index out of bounds");
        return m_components[i];
    }

    /// Access component by index (const)
    const T& operator[](int i) const {
        GISMO_ASSERT(i >= 0 && i < Dim, "Index out of bounds");
        return m_components[i];
    }

    /// Get component data
    T* data() { return m_components.data(); }
    const T* data() const { return m_components.data(); }

    /// Get dimension
    static constexpr int dim() { return Dim; }

    /// Length (magnitude) of vector
    T length() const
    {
        return math::sqrt(squaredLength());
    }

    /// Squared length of vector (faster)
    T squaredLength() const
    {
        T sum = T(0);
        for (int i = 0; i < Dim; ++i) {
            sum += m_components[i] * m_components[i];
        }
        return sum;
    }

    /// Normalize vector (return normalized copy)
    Vector normalized() const
    {
        T len = length();
        GISMO_ASSERT(!math::isZero(len), "Cannot normalize zero vector");

        Vector result;
        for (int i = 0; i < Dim; ++i) {
            result.m_components[i] = m_components[i] / len;
        }
        return result;
    }

    /// Normalize vector in-place
    Vector& normalize()
    {
        T len = length();
        GISMO_ASSERT(!math::isZero(len), "Cannot normalize zero vector");

        for (int i = 0; i < Dim; ++i) {
            m_components[i] /= len;
        }
        return *this;
    }

    /// Dot product with another vector
    T dot(const Vector& other) const
    {
        T result = T(0);
        for (int i = 0; i < Dim; ++i) {
            result += m_components[i] * other.m_components[i];
        }
        return result;
    }

    /// Cross product (only for 3D vectors)
    template<int D = Dim>
    typename std::enable_if<D == 3, Vector>::type
    cross(const Vector& other) const
    {
        return Vector{
            m_components[1] * other.m_components[2] - m_components[2] * other.m_components[1],
            m_components[2] * other.m_components[0] - m_components[0] * other.m_components[2],
            m_components[0] * other.m_components[1] - m_components[1] * other.m_components[0]
        };
    }

    /// Vector addition
    Vector operator+(const Vector& other) const
    {
        Vector result;
        for (int i = 0; i < Dim; ++i) {
            result.m_components[i] = m_components[i] + other.m_components[i];
        }
        return result;
    }

    /// Vector subtraction
    Vector operator-(const Vector& other) const
    {
        Vector result;
        for (int i = 0; i < Dim; ++i) {
            result.m_components[i] = m_components[i] - other.m_components[i];
        }
        return result;
    }

    /// Scalar multiplication
    Vector operator*(T scalar) const
    {
        Vector result;
        for (int i = 0; i < Dim; ++i) {
            result.m_components[i] = m_components[i] * scalar;
        }
        return result;
    }

    /// Scalar division
    Vector operator/(T scalar) const
    {
        GISMO_ASSERT(!math::isZero(scalar), "Division by zero");

        Vector result;
        for (int i = 0; i < Dim; ++i) {
            result.m_components[i] = m_components[i] / scalar;
        }
        return result;
    }

    /// Unary minus
    Vector operator-() const
    {
        Vector result;
        for (int i = 0; i < Dim; ++i) {
            result.m_components[i] = -m_components[i];
        }
        return result;
    }

    /// Equality comparison
    bool operator==(const Vector& other) const
    {
        for (int i = 0; i < Dim; ++i) {
            if (!math::isEqual(m_components[i], other.m_components[i])) {
                return false;
            }
        }
        return true;
    }

    /// Inequality comparison
    bool operator!=(const Vector& other) const
    {
        return !(*this == other);
    }
};

/// Scalar multiplication (scalar * vector)
template<typename T, int Dim>
Vector<T, Dim> operator*(T scalar, const Vector<T, Dim>& vec)
{
    return vec * scalar;
}

/// Common typedefs
typedef Vector<real_t, 1> Vector1d;
typedef Vector<real_t, 2> Vector2d;
typedef Vector<real_t, 3> Vector3d;

/// Template typedefs
template<typename T> using Vector1 = Vector<T, 1>;
template<typename T> using Vector2 = Vector<T, 2>;
template<typename T> using Vector3 = Vector<T, 3>;

} // namespace geometry

} // namespace gismo