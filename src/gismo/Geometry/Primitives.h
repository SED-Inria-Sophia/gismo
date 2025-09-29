/** @file Primitives.h

    @brief Basic geometric primitive definitions.

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
 * @brief Bounding box class
 * @tparam T Scalar type
 * @tparam Dim Dimension
 */
template<typename T, int Dim>
class BoundingBox
{
private:
    Point<T, Dim> m_min;
    Point<T, Dim> m_max;
    bool m_empty;

public:
    /// Constructor for empty box
    BoundingBox() : m_empty(true) {}

    /// Constructor with min and max points
    BoundingBox(const Point<T, Dim>& min, const Point<T, Dim>& max)
        : m_min(min), m_max(max), m_empty(false)
    {
        // Ensure min <= max
        for (int i = 0; i < Dim; ++i) {
            if (m_min[i] > m_max[i]) {
                std::swap(m_min[i], m_max[i]);
            }
        }
    }

    /// Check if box is empty
    bool empty() const { return m_empty; }

    /// Get minimum point
    const Point<T, Dim>& min() const {
        GISMO_ASSERT(!m_empty, "Box is empty");
        return m_min;
    }

    /// Get maximum point
    const Point<T, Dim>& max() const {
        GISMO_ASSERT(!m_empty, "Box is empty");
        return m_max;
    }

    /// Add a point to the box (expand if necessary)
    void addPoint(const Point<T, Dim>& point)
    {
        if (m_empty) {
            m_min = m_max = point;
            m_empty = false;
        } else {
            for (int i = 0; i < Dim; ++i) {
                m_min[i] = math::min(m_min[i], point[i]);
                m_max[i] = math::max(m_max[i], point[i]);
            }
        }
    }

    /// Check if point is inside box
    bool contains(const Point<T, Dim>& point) const
    {
        if (m_empty) return false;

        for (int i = 0; i < Dim; ++i) {
            if (point[i] < m_min[i] || point[i] > m_max[i]) {
                return false;
            }
        }
        return true;
    }

    /// Get center of box
    Point<T, Dim> center() const
    {
        GISMO_ASSERT(!m_empty, "Box is empty");

        Point<T, Dim> result;
        for (int i = 0; i < Dim; ++i) {
            result[i] = (m_min[i] + m_max[i]) / T(2);
        }
        return result;
    }

    /// Get size (diagonal) of box
    Vector<T, Dim> size() const
    {
        GISMO_ASSERT(!m_empty, "Box is empty");

        Vector<T, Dim> result;
        for (int i = 0; i < Dim; ++i) {
            result[i] = m_max[i] - m_min[i];
        }
        return result;
    }
};

/**
 * @brief Line segment class
 * @tparam T Scalar type
 * @tparam Dim Dimension
 */
template<typename T, int Dim>
class LineSegment
{
private:
    Point<T, Dim> m_start;
    Point<T, Dim> m_end;

public:
    /// Constructor
    LineSegment(const Point<T, Dim>& start, const Point<T, Dim>& end)
        : m_start(start), m_end(end) {}

    /// Get start point
    const Point<T, Dim>& start() const { return m_start; }

    /// Get end point
    const Point<T, Dim>& end() const { return m_end; }

    /// Get direction vector
    Vector<T, Dim> direction() const
    {
        return m_end - m_start;
    }

    /// Get length
    T length() const
    {
        return m_start.distanceTo(m_end);
    }

    /// Get point at parameter t (0 = start, 1 = end)
    Point<T, Dim> pointAt(T t) const
    {
        Point<T, Dim> result;
        for (int i = 0; i < Dim; ++i) {
            result[i] = m_start[i] + t * (m_end[i] - m_start[i]);
        }
        return result;
    }

    /// Get closest point on segment to given point
    Point<T, Dim> closestPoint(const Point<T, Dim>& point) const
    {
        Vector<T, Dim> dir = direction();
        T len_sq = dir.squaredLength();

        if (math::isZero(len_sq)) {
            return m_start; // Degenerate case
        }

        Vector<T, Dim> to_point;
        for (int i = 0; i < Dim; ++i) {
            to_point[i] = point[i] - m_start[i];
        }

        T t = to_point.dot(dir) / len_sq;
        t = math::clamp(t, T(0), T(1));

        return pointAt(t);
    }
};

/// Common typedefs
template<typename T> using BoundingBox2d = BoundingBox<T, 2>;
template<typename T> using BoundingBox3d = BoundingBox<T, 3>;

template<typename T> using LineSegment2d = LineSegment<T, 2>;
template<typename T> using LineSegment3d = LineSegment<T, 3>;

} // namespace geometry

} // namespace gismo