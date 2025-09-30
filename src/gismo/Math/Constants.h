/** @file Constants.h

    @brief Mathematical constants for use in G+Smo.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Bressan, A. Mantzaflaris
*/

#pragma once

#include <gismo/Common/Types.h>

// #include <cmath>  // Temporarily disabled to debug compilation issues
#include <limits>

namespace gismo {

/** @namespace gismo::math

    @brief
    This namespace contains common mathematical constants.

    \ingroup Math
*/
namespace math {

// Mathematical constants
static const real_t E       = 2.718281828459045235360;  ///< e
static const real_t LOG2E   = 1.442695040888963407360;  ///< log2(e)
static const real_t LOG10E  = 0.434294481903251827651;  ///< log10(e)
static const real_t LN2     = 0.693147180559945309417;  ///< ln(2)
static const real_t LN10    = 2.302585092994045684018;  ///< ln(10)
static const real_t PI      = 3.141592653589793238463;  ///< pi
static const real_t PI_2    = 1.570796326794896619231;  ///< pi/2
static const real_t PI_4    = 0.785398163397448309616;  ///< pi/4
static const real_t PI_180  = 0.017453292519943295769;  ///< pi/180
static const real_t _1_PI   = 0.318309886183790671538;  ///< 1/pi
static const real_t _2_PI   = 0.636619772367581343076;  ///< 2/pi
static const real_t _180_PI = 57.295779513082320876798; ///< 180/pi

} // namespace math

} // namespace gismo