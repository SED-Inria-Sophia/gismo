/** @file Combinatorics.h

    @brief Provides combinatorial utilities.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Bressan, A. Mantzaflaris
*/

#pragma once

#include <vector>

namespace gismo
{

/** \brief Returns the factorial of \a n i.e. \a n!
 * Remember that factorial grow too fast and only n! with n<=13 can be
 * stored in a 32bit that is an unsigned.
 * \ingroup combinatorics
 */
inline unsigned factorial( unsigned n)
{
    static const unsigned precomputed[]= {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};
    if (n >= sizeof(precomputed)/sizeof(unsigned)) return 0; // overflow
    return precomputed[n];
}

/**
   \brief Computes the binomial expansion coefficient binomial(n,r)
*/
inline unsigned binomial(unsigned n, unsigned r)
{
    if (r > n) return 0;
    if (r > n-r) r = n-r; // symmetry
    unsigned result = 1;
    for(unsigned i = 1; i <= r; ++i)
    {
        result *= (n - r + i);
        result /= i;
    }
    return result;
}

} // namespace gismo