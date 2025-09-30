/** @file LinearAlgebra.h

    @brief This is the main header file that collects wrappers of Eigen for linear algebra.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#define gsEigen Eigen

#include <gismo/Common/Config.h>
#include <gismo/Common/Debug.h>

// Eigen linear algebra library (http://eigen.tuxfamily.org)

// Make Eigen use GISMO_ASSERT which throws exceptions
//
// Must be defined before including Eigen headers
// http://eigen.tuxfamily.org/dox-3.2/TopicPreprocessorDirectives.html
#define eigen_assert( cond ) GISMO_ASSERT( cond, "" )

// Plugin provides extra members - temporarily disabled to debug compilation issues
// #define EIGEN_MATRIXBASE_PLUGIN <gismo/Common/MatrixAddons.h>
// #define EIGEN_PLAINOBJECTBASE_PLUGIN <gismo/Common/PlainObjectBaseAddons.h>
#include <gismo/Common/EigenDeclarations.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/StdVector>
#include <Eigen/Geometry>

#undef eigen_assert
#undef EIGEN_MATRIXBASE_PLUGIN
#undef EIGEN_PLAINOBJECTBASE_PLUGIN

namespace gismo
{

using gsEigen::internal::cast; // from Core/MathFunctions.h

/**
   \brief Check if all the entries of the matrix \a x are not NAN (not
   a number)

   See https://en.wikipedia.org/wiki/Floating_point#Special_values
   and https://en.wikipedia.org/wiki/NaN
 */
template<typename Derived>
inline bool (isnumber)(const gsEigen::MatrixBase<Derived>& x)
{
    return x.allFinite() && !x.hasNaN();
}

/**
   \brief Check if all the entries of the matrix \a x are finite

   See https://en.wikipedia.org/wiki/Floating_point#Special_values
 */
template<typename Derived>
inline bool (isfinite)(const gsEigen::MatrixBase<Derived>& x)
{
    return x.allFinite();
}

} // namespace gismo