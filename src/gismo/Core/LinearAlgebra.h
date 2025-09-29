/** @file gsLinearAlgebra.h

    @brief This is the main header file that collects wrappers of Eigen for linear algebra.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include "gismo/Core/Config.h"
#include "gismo/Core/Debug.h"

// Basic type definitions for Core module
namespace gismo {

// Forward declarations for basic types
typedef double real_t;
typedef int index_t;
typedef short short_t;

} // namespace gismo

// Eigen linear algebra library (http://eigen.tuxfamily.org)
// Note: Full Eigen integration will be in the Matrix module

// Make Eigen use GISMO_ASSERT which throws exceptions
//
// Must be defined before including Eigen headers
// http://eigen.tuxfamily.org/dox-3.2/TopicPreprocessorDirectives.html
#define eigen_assert( cond ) GISMO_ASSERT( cond, "" )

// Enable Eigen plugins when Matrix module is available
#ifdef GISMO_WITH_MATRIX
#define EIGEN_MATRIXBASE_PLUGIN <gismo/Matrix/gsMatrixAddons.h>
#define EIGEN_PLAINOBJECTBASE_PLUGIN <gismo/Matrix/gsPlainObjectBaseAddons.h>
#endif

#include <Eigen/Core>

#if defined(gsMpfr_ENABLED)
#include <unsupported/Eigen/MPRealSupport>
#endif

#if defined(gsGmp_ENABLED)
#include <unsupported/Eigen/MPQClassSupport>
#endif

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/StdVector>
#include <Eigen/Geometry>

namespace gismo {

// Basic type definitions for linear algebra
typedef double real_t;
typedef int index_t;
typedef short short_t;

// Forward declarations for matrix types
template<class T> class gsMatrix;
template<class T> class gsVector;
template<class T> class gsSparseMatrix;

// Eigen type aliases
namespace util {
    template<class T, int _Rows = Dynamic, int _Cols = Dynamic>
    using gsEigen_Matrix = Eigen::Matrix<T, _Rows, _Cols>;

    template<class T>
    using gsEigen_VectorX = Eigen::Matrix<T, Dynamic, 1>;

    template<class T>
    using gsEigen_RowVectorX = Eigen::Matrix<T, 1, Dynamic>;

    template<class T>
    using gsEigen_SparseMatrix = Eigen::SparseMatrix<T>;
}

} // namespace gismo