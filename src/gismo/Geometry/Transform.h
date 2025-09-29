/** @file Transform.h

    @brief Transformation classes for geometric operations.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include "Point.h"
#include "Vector.h"

namespace gismo {

namespace geometry {

/**
 * @brief Base transformation interface
 * @tparam T Scalar type
 * @tparam Dim Dimension
 */
template<typename T, int Dim>
class Transform
{
public:
    /// Apply transformation to a point
    virtual Point<T, Dim> apply(const Point<T, Dim>& point) const = 0;

    /// Apply transformation to a vector
    virtual Vector<T, Dim> apply(const Vector<T, Dim>& vector) const = 0;

    /// Get inverse transformation
    virtual std::unique_ptr<Transform<T, Dim>> inverse() const = 0;

    /// Virtual destructor
    virtual ~Transform() = default;
};

/**
 * @brief Translation transformation
 * @tparam T Scalar type
 * @tparam Dim Dimension
 */
template<typename T, int Dim>
class Translation : public Transform<T, Dim>
{
private:
    Vector<T, Dim> m_offset;

public:
    /// Constructor
    explicit Translation(const Vector<T, Dim>& offset) : m_offset(offset) {}

    /// Apply translation to point
    Point<T, Dim> apply(const Point<T, Dim>& point) const override
    {
        Point<T, Dim> result;
        for (int i = 0; i < Dim; ++i) {
            result[i] = point[i] + m_offset[i];
        }
        return result;
    }

    /// Apply translation to vector (vectors are not affected by translation)
    Vector<T, Dim> apply(const Vector<T, Dim>& vector) const override
    {
        return vector; // Translation doesn't affect vectors
    }

    /// Get inverse translation
    std::unique_ptr<Transform<T, Dim>> inverse() const override
    {
        return std::make_unique<Translation<T, Dim>>(-m_offset);
    }

    /// Get translation offset
    const Vector<T, Dim>& offset() const { return m_offset; }
};

/**
 * @brief Scaling transformation
 * @tparam T Scalar type
 * @tparam Dim Dimension
 */
template<typename T, int Dim>
class Scaling : public Transform<T, Dim>
{
private:
    T m_factor;

public:
    /// Constructor
    explicit Scaling(T factor) : m_factor(factor)
    {
        GISMO_ASSERT(!math::isZero(factor), "Scaling factor cannot be zero");
    }

    /// Apply scaling to point
    Point<T, Dim> apply(const Point<T, Dim>& point) const override
    {
        Point<T, Dim> result;
        for (int i = 0; i < Dim; ++i) {
            result[i] = point[i] * m_factor;
        }
        return result;
    }

    /// Apply scaling to vector
    Vector<T, Dim> apply(const Vector<T, Dim>& vector) const override
    {
        Vector<T, Dim> result;
        for (int i = 0; i < Dim; ++i) {
            result[i] = vector[i] * m_factor;
        }
        return result;
    }

    /// Get inverse scaling
    std::unique_ptr<Transform<T, Dim>> inverse() const override
    {
        return std::make_unique<Scaling<T, Dim>>(T(1) / m_factor);
    }

    /// Get scaling factor
    T factor() const { return m_factor; }
};

/// Common typedefs
template<typename T> using Transform2d = Transform<T, 2>;
template<typename T> using Transform3d = Transform<T, 3>;

template<typename T> using Translation2d = Translation<T, 2>;
template<typename T> using Translation3d = Translation<T, 3>;

template<typename T> using Scaling2d = Scaling<T, 2>;
template<typename T> using Scaling3d = Scaling<T, 3>;

} // namespace geometry

} // namespace gismo