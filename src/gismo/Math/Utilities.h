/** @file Utilities.h

    @brief Mathematical utility functions for use in G+Smo.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Bressan, A. Mantzaflaris
*/

#pragma once

#include <gismo/Math/Functions.h>
#include <gismo/Math/Constants.h>

namespace gismo {

namespace math {

/// Power of 2 function
template <typename T>
inline T exp2(const T a) {
    return 1U << a;
}

/// Rounding function
template <typename T>
T round(T a) {
    return math::floor(a + 0.5);
}

/// For numeric types, this function returns the next representable
/// value after x in the direction of y
template <typename T>
inline T nextafter(T x, T y) {
    // Implementation depends on the type
    if (x < y) {
        return x + std::numeric_limits<T>::epsilon();
    } else if (x > y) {
        return x - std::numeric_limits<T>::epsilon();
    } else {
        return x;
    }
}

/// Check if a value is approximately zero
template <typename T>
inline bool isZero(T x, T tolerance = std::numeric_limits<T>::epsilon()) {
    return math::abs(x) <= tolerance;
}

/// Check if two values are approximately equal
template <typename T>
inline bool isEqual(T a, T b, T tolerance = std::numeric_limits<T>::epsilon()) {
    return math::abs(a - b) <= tolerance;
}

/// Clamp a value between min and max
template <typename T>
inline T clamp(T value, T minVal, T maxVal) {
    return math::min(math::max(value, minVal), maxVal);
}

/// Linear interpolation between two values
template <typename T>
inline T lerp(T a, T b, T t) {
    return a + t * (b - a);
}

/// Sign function (-1, 0, or 1)
template <typename T>
inline int sign(T x) {
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

/// Square function
template <typename T>
inline T square(T x) {
    return x * x;
}

/// Cube function
template <typename T>
inline T cube(T x) {
    return x * x * x;
}

} // namespace math

} // namespace gismo