/** @file Functions.h

    @brief Mathematical functions for use in G+Smo.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Bressan, A. Mantzaflaris
*/

#pragma once

#include "../Core/Core"
#include <cmath>
#include <complex>

#ifdef gsCoDiPack_ENABLED
#include <gsCoDiPack/gsCoDiPack.h>
#endif

namespace gismo {

namespace math {

// Import standard math functions into gismo::math namespace
using std::abs;
using std::acos;
using std::asin;
using std::atan2;
using std::atan;
using std::ceil;
using std::cos;
using std::cosh;
using std::exp;
using std::floor;
using std::frexp;
using std::ldexp;
using std::log10;
using std::log;
using std::max;
using std::min;
using std::pow;
using std::sin;
using std::sinh;
using std::sqrt;
using std::tan;
using std::tanh;
using std::real;
using std::imag;
using std::conj;

#ifdef gsCoDiPack_ENABLED
// Import CoDiPack functions when available
using codi::abs;
using codi::acos;
using codi::asin;
using codi::atan2;
using codi::atan;
using codi::ceil;
using codi::cos;
using codi::cosh;
using codi::exp;
using codi::floor;
using codi::log10;
using codi::log;
using codi::max;
using codi::min;
using codi::pow;
using codi::sin;
using codi::sinh;
using codi::sqrt;
using codi::tan;
using codi::tanh;
using codi::isnan;
using codi::isfinite;
using codi::isinf;
#endif

#ifdef gsUniversal_ENABLED
// Import Universal number functions when available
using sw::universal::abs;
using sw::universal::acos;
using sw::universal::asin;
using sw::universal::atan2;
using sw::universal::atan;
using sw::universal::ceil;
using sw::universal::cos;
using sw::universal::cosh;
using sw::universal::exp;
using sw::universal::floor;
using sw::universal::log10;
using sw::universal::log;
using sw::universal::max;
using sw::universal::min;
using sw::universal::pow;
using sw::universal::sin;
using sw::universal::sinh;
using sw::universal::sqrt;
using sw::universal::tan;
using sw::universal::tanh;

// Dummy implementations for Universal numbers
template<size_t nbits, size_t es>
inline sw::universal::posit<nbits,es> frexp(const sw::universal::posit<nbits,es> & a, int* b) {return  a;}

template<size_t nbits, size_t es>
inline sw::universal::posit<nbits,es> ldexp(const sw::universal::posit<nbits,es> & a, int b ) {return  a;}

using sw::universal::isnan;
using sw::universal::isfinite;
using sw::universal::isinf;
using sw::universal::real;
using sw::universal::imag;
using sw::universal::conj;
#endif

} // namespace math

} // namespace gismo