/** @file PointGrid.h

    @brief Provides functions to generate structured point data

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): C. Hofreither, A. Mantzaflaris
*/

#pragma once

#include <Vector.h>
#include <vector>

namespace gismo
{

/* **************** Utility functions **************** */

template<typename T>
gsVector<unsigned> uniformSampleCount(const gsVector<T>& lower,
                                      const gsVector<T>& upper,
                                      int numPoints = 1000)
{
    const T vol = (upper - lower).prod();
    const T avgLength = std::pow(vol, 1.0 / lower.size());
    const unsigned ptsPerDim = std::max(2u, static_cast<unsigned>(std::pow(numPoints, 1.0 / lower.size()) + 0.5));

    return gsVector<unsigned>::Constant(lower.size(), ptsPerDim);
}

template<typename T>
void uniformIntervals(const gsVector<T>& lower, const gsVector<T>& upper,
                      std::vector< std::vector<T> >& intervals, int numIntervals = 1000)
{
    const index_t d = lower.size();
    intervals.resize(d);

    for (index_t i = 0; i < d; ++i)
    {
        const T h = (upper[i] - lower[i]) / (numIntervals - 1);
        intervals[i].resize(numIntervals);
        for (int j = 0; j < numIntervals; ++j)
        {
            intervals[i][j] = lower[i] + j * h;
        }
    }
}

/* **************** Uniform grids described by limits/corners **************** */

/** @brief Construct a Cartesian grid of uniform points in a hypercube, using
 * <em>np[i]</em> points in direction \em i.
 *
 * The hypercube is defined by its lower corner \f$a = (a_1,\ldots,a_d)\f$ and
 * the upper corner \f$b = (b_1,\ldots,b_d)\f$, i.e.,
 * the hypercube is \f$ \mathsf{X}_{i=1}^d [a_i,b_i] \f$.
 *
 * \param a gsVector of length \em d, lower corner of the hypercube: \f$a = (a_1,\ldots,a_d)\f$
 * \param b gsVector of length \em d, upper corner of the hypercube: \f$b = (b_1,\ldots,b_d)\f$
 * \param np gsVector of length \em d, indicating number of grid points in each coordinate direction
 *
 * \returns gsMatrix with size \f$ d \times \prod_{i=1}^d np[i]\f$.
 *
 * \ingroup Utils
 */
template<typename T>
gsMatrix<T> uniformGrid(const gsVector<T>& a, const gsVector<T>& b,
                        const gsVector<unsigned>& np)
{
    const index_t d = a.size();
    gsVector<unsigned> cumprod(d+1);
    cumprod[0] = 1;
    for (index_t i = 0; i < d; ++i)
        cumprod[i+1] = cumprod[i] * np[i];

    gsMatrix<T> result(d, cumprod[d]);

    gsVector<T> h(d);
    for (index_t i = 0; i < d; ++i)
        h[i] = (b[i] - a[i]) / (np[i] - 1);

    for (size_t p = 0; p < cumprod[d]; ++p)
    {
        size_t rem = p;
        for (index_t i = 0; i < d; ++i)
        {
            const size_t coord = rem / cumprod[i];
            result(i, p) = a[i] + coord * h[i];
            rem %= cumprod[i];
        }
    }

    return result;
}

} // namespace gismo