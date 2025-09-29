/** @file Constants.h

    @brief Mathematical constants for use in G+Smo.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Bressan, A. Mantzaflaris
*/

#pragma once

#include "../Core/Core"
#include <limits>

namespace gismo {

/** @namespace gismo::math

    @brief
    This namespace contains common mathematical constants and functions.

    \ingroup Math
*/
namespace math {

/// Numeric limits for real_t type
typedef std::numeric_limits<real_t> limits;

/// Mathematical constants
template<typename T>
struct constants
{
    /// Pi constant
    static constexpr T pi = T(3.14159265358979323846264338327950288419716939937510);

    /// Euler's number
    static constexpr T e = T(2.71828182845904523536028747135266249775724709369996);

    /// Square root of 2
    static constexpr T sqrt2 = T(1.41421356237309504880168872420969807856967187537694);

    /// Golden ratio
    static constexpr T phi = T(1.61803398874989484820458683436563811772030917980576);

    /// Natural logarithm of 2
    static constexpr T ln2 = T(0.693147180559945309417232121458176568075500134360255);

    /// Natural logarithm of 10
    static constexpr T ln10 = T(2.30258509299404568401799145468436420760110148862877);
};

/// Convenience aliases for common types
constexpr real_t pi = constants<real_t>::pi;
constexpr real_t e = constants<real_t>::e;
constexpr real_t sqrt2 = constants<real_t>::sqrt2;
constexpr real_t phi = constants<real_t>::phi;
constexpr real_t ln2 = constants<real_t>::ln2;
constexpr real_t ln10 = constants<real_t>::ln10;

} // namespace math

} // namespace gismo