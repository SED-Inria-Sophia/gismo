/** @file gsPointGrid.h

    @brief Provides functions to generate structured point data

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): C. Hofreither, A. Mantzaflaris
*/

#pragma once

#include <gismo/Math/LinearAlgebra.h>
#include <gismo/Math/GridIterator.h>

namespace gismo {


/* **************** Utility functions **************** */

template<typename T>  // gsMatrix<T> ab
gsVector<unsigned> uniformSampleCount (const gsVector<T>& lower,
                                       const gsVector<T>& upper,
                                       int numPoints = 1000)
{
    const index_t d = lower.rows();
    assert( d == upper.rows() );

    // TO do : phys. volume criterion
    gsVector<T> span = upper - lower;
    const T volume = span.prod();
    const T h = math::pow(volume / (T)(numPoints), (T)(1) / (T)(d));

    gsVector<unsigned> np(d);

    for (index_t i = 0; i < d; ++i)
    {
        np[i] = cast<T,unsigned>(math::ceil( span[i] / h ) );
        GISMO_ASSERT( np[i] > 0, "Something went wrong, number of points is zero..");
    }

    return np;
}

template<typename T>
void uniformIntervals(const gsVector<T>& lower, const gsVector<T>& upper,
                      std::vector< std::vector<T> >& intervals, int numIntervals = 1000)
{
    const int d = lower.rows();
    assert( d == upper.rows() );

    gsVector<unsigned> np = uniformSampleCount( lower, upper, numIntervals );

    // resize to dimension d without copying old contents, if any
    intervals.clear();
    intervals.resize(d);

    for (int i = 0; i < d; ++i)
    {
        int numInt = np[i] - 1;
        if (numInt <= 1)
            numInt = 1;

        const T h = (T)(1) / (T)(numInt);

        intervals[i].resize(numInt + 1);
        for (int j = 0; j <= numInt; ++j)
            intervals[i][j] = (T)(j) * h;
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
template<class T>  // gsMatrix<T> ab
gsMatrix<T> gsPointGrid( gsVector<T> const & a, gsVector<T> const & b,
                         gsVector<unsigned> const & np )
{
    gsMatrix<T> res(a.size(), np.prod() );
    gsGridIterator<T,CUBE> pt(a, b, np.cast<index_t>());
    for(index_t c = 0; pt; ++pt, ++c)
        res.col(c) = *pt;
    return res;
}

// Specialization of the arguments for the 1D case
template<class T> inline
gsMatrix<T> gsPointGrid( T const & t1, T const & t2, unsigned const & n = 100)
{
    gsMatrix<T,1,2> ab(1,2); ab << t1, t2;
    gsGridIterator<T,CUBE,1> pt(ab,n);
    gsMatrix<T> rvo(ab.rows(), pt.numPoints() );
    for(index_t c = 0; pt; ++pt, ++c)
        rvo.col(c) = *pt;
    return rvo;
}

/**
   Approximately uniformly spaced grid in every direction, with
   approximately numPoints total points
*/
template<typename T>  // todo: remove, replace by next one
gsMatrix<T>uniformPointGrid(const gsVector<T>& lower, // note: structure lost
                            const gsVector<T>& upper,
                            int numPoints = 1000)
{
    const gsVector<unsigned> cwisePoints = uniformSampleCount(lower, upper, numPoints);
    return gsPointGrid(lower, upper, cwisePoints); // note: structure lost
}

/**
   Returns an approximately uniformly spaced grid in every direction,
   with approximately numPoints total points.

   Each column of the result correspond to a point.
*/
template<class T> inline
gsMatrix<T> gsPointGrid(gsMatrix<T> const & ab, int numPoints)
{   // Note: structure lost
    gsGridIterator<T,CUBE> pt(ab,numPoints);
    gsMatrix<T> rvo(ab.rows(), pt.numPoints() );
    for(index_t c = 0; pt; ++pt, ++c)
        rvo.col(c) = *pt;
    return rvo;
}


/* **************** Cartesian grids **************** */


/// Construct a grid of points by coordinate vectors in the container
/// cwise, use out argument res
template<class T, class CwiseContainer> inline
void gsPointGrid(CwiseContainer const & cwise, gsMatrix<T>& res)
{
    gsGridIterator<T,CWISE> pt(cwise);
    res.resize(cwise.size(), pt.numPoints() );
    for(index_t c = 0; pt; ++pt, ++c)
        res.col(c) = *pt;
}

/// Construct a grid of points by coordinate vectors in the container cwise
template<class T, class CwiseContainer> inline
gsMatrix<T> gsPointGrid(CwiseContainer const & cwise)
{
    gsMatrix<T> rvo;
    gsPointGrid(cwise, rvo);
    return rvo;
}


} // namespace gismo
