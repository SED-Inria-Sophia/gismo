/** @file LinearAlgebra.h

    @brief Eigen integration with linear algebra implementations.

    This file provides comprehensive Eigen library integration including:
    - Core Eigen headers (Dense, Sparse, Geometry)
    - GISMO Eigen extensions (BlockDiag, RowSelection, VecAsSymmMatrix, etc.)
    - Matrix and Vector implementations with mathematical functions
    - Type aliases and cross-platform compatibility layers

    SCOPE: Complete Eigen-based linear algebra system (headers + implementations).
    FOR BROADER MATH: Use <gismo/Math/Math> which includes this + geometry utilities.

    NOTE: Includes MathFunctions.h because Matrix/Vector implementations use mathematical
    functions (abs, acos, min, etc.). This is architecturally correct.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris

    Migrated from gsCore/gsLinearAlgebra.h as part of Task 1.1: Create Math Module.
    Clarified as PURE Eigen integration (Option B: Clear Functional Separation).
*/


# pragma once

#include <gismo/Math/MathFunctions.h>

// Eigen linear algebra library (http://eigen.tuxfamily.org)


// Make Eigen use GISMO_ASSERT which throws exceptions
//
// Must be defined before including Eigen headers
// http://eigen.tuxfamily.org/dox-3.2/TopicPreprocessorDirectives.html
#define eigen_assert( cond ) GISMO_ASSERT( cond, "" )

// Forward declarations for Eigen extensions
namespace gsEigen
{
    template<typename MatrixType,int RowFactor> class BlockDiag;
    template<typename MatrixType,int RowFactor> class BlockTranspose;
    template<typename XprType, typename IndicesType> class RowSelection;
    template<typename MatrixType, int Dim> class VecAsSymmMatrix;

    namespace internal
    {
        template<typename MatrixType> struct adjugate_impl;
    }
}

// Plugin provides extra members
#define EIGEN_MATRIXBASE_PLUGIN <gismo/Math/MatrixAddons.h>
#define EIGEN_PLAINOBJECTBASE_PLUGIN <gismo/Math/PlainObjectBaseAddons.h>

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

// Extra Eigen code
#include <gismo/Math/Adjugate.h>
#include <gismo/Math/BlockDiag.h>
#include <gismo/Math/BlockTranspose.h>
#include <gismo/Math/RowSelection.h>
#include <gismo/Math/VecAsSymmMatrix.h>

#ifdef GISMO_WITH_SUPERLU
#include <Eigen/SuperLUSupport>
#endif

#ifdef GISMO_WITH_PARDISO
#include <Eigen/PardisoSupport>
#endif

#ifdef GISMO_WITH_PASTIX
#include <Eigen/PaStiXSupport>
#endif

// sparsesuite
//#include <Eigen/UmfPackSupport>
//#include <Eigen/SPQRSupport>
//#include <Eigen/CholmodSupport>

// METIS
//#include <Eigen/MetisSupport>

// PaStiX
//#include <Eigen/PaStiXSupport>

#ifdef GISMO_WITH_PYBIND11
#include <pybind11/eigen.h>
#endif


#undef eigen_assert
#undef EIGEN_MATRIXBASE_PLUGIN
#undef EIGEN_PLAINOBJECTBASE_PLUGIN


namespace gismo
{

using gsEigen::internal::cast; // from Core/MathFunctions.h

/**
   \brief Check if all the entires if the matrix \a x are not NAN (not
   a number)

   See https://en.wikipedia.org/wiki/Floating_point#Special_values
   and https://en.wikipedia.org/wiki/NaN
 */
template<typename Derived>
inline bool (isnumber)(const gsEigen::MatrixBase<Derived>& x)
{ return ((x.array() == x.array())).all(); }

/**
   \brief Check if all the entires if the matrix \a x are not INF (infinite)

   See https://en.wikipedia.org/wiki/Floating_point#Special_values
 */
template<typename Derived>
inline bool isfinite(const gsEigen::MatrixBase<Derived>& x)
{ return ( (x - x).array() == (x - x).array()).all(); }



//Constantss related to gsMatrix
//( see also external/Eigen/src/Core/util/Constants.h )
using gsEigen::Dynamic ;//=-1

using gsEigen::Lower;//=1
using gsEigen::Upper;//=2

// Values for matrix align options
using gsEigen::RowMajor;//=0
using gsEigen::ColMajor;//=1
using gsEigen::AutoAlign;//=0

// Forward declarations for AsMatrix implementation classes
template<class T, int _Rows, int _Cols> class gsAsMatrixImpl ;
template<class T, int _Rows, int _Cols> class gsAsConstMatrixImpl ;

template<class T, int _Rows> class gsAsVectorImpl ;
template<class T, int _Rows> class gsAsConstVectorImpl ;

// Forward declarations for matrix types (implementation classes)
template<class T, int _Rows, int _Cols, int _Options> class gsMatrixImpl;
template<class T, int _Rows, int _Options> class gsVectorImpl;

// helper template for changing the dimension of a matrix
template <int Dim, int Change>
struct ChangeDim
{
    enum { D = Dim + Change <= 0 ? 1 : Dim + Change };
};
template <int Change>
struct ChangeDim<Dynamic, Change>
{
    enum { D = Dynamic };
};


/**
   @brief Adaptor for Eigen types
*/
template<typename T>
struct gsEigenAdaptor
{
public:
    // Note: IncompleteILU is not compatible with
    // Eigen::ConjugateGradient because this preconditionner does not
    // preserve symmetry.

    /// Congugate gradient without preconditioner (identity as preconditioner)
    typedef gsEigen::ConjugateGradient<gsEigen::SparseMatrix<T,0,index_t>,
            gsEigen::Lower|Eigen::Upper, gsEigen::IdentityPreconditioner> CGIdentity;

    /// Congugate gradient with diagonal (Jacobi) preconditioner
    typedef gsEigen::ConjugateGradient<gsEigen::SparseMatrix<T,0,index_t>,
            gsEigen::Lower|Eigen::Upper, gsEigen::DiagonalPreconditioner<T> > CGDiagonal;

    /// BiCGSTAB with Incomplete LU factorization with dual-threshold strategy
    typedef gsEigen::BiCGSTAB<gsEigen::SparseMatrix<T,0,index_t>,
                            gsEigen::IncompleteLUT<T, index_t> > BiCGSTABILUT;

    /// BiCGSTAB with Diagonal (Jacobi) preconditioner
    typedef gsEigen::BiCGSTAB<gsEigen::SparseMatrix<T,0,index_t>,
                            gsEigen::DiagonalPreconditioner<T> > BiCGSTABDiagonal;

    /// BiCGSTAB without preconditioner (identity as preconditioner)
    typedef gsEigen::BiCGSTAB<gsEigen::SparseMatrix<T,0,index_t>,
                            gsEigen::IdentityPreconditioner > BiCGSTABIdentity;

    /// Direct LDLt factorization
    typedef gsEigen::SimplicialLDLT<gsEigen::SparseMatrix<T,0,index_t> > SimplicialLDLT;

    /// Direct LLt factorization
    typedef gsEigen::SimplicialLLT<gsEigen::SparseMatrix<T,0,index_t> > SimplicialLLT;

    /// Sparse LU solver
    typedef gsEigen::SparseLU<gsEigen::SparseMatrix<T,0,index_t>,
                            gsEigen::COLAMDOrdering<index_t> > SparseLU;

    /// Sparse QR solver
    typedef gsEigen::SparseQR<gsEigen::SparseMatrix<T,0,index_t>,
                            gsEigen::COLAMDOrdering<index_t> > SparseQR;

    #ifdef GISMO_WITH_SUPERLU
    /// SuperLU (if enabled)
    typedef gsEigen::SuperLU<gsEigen::SparseMatrix<T,0,index_t> > SuperLU;
    #endif

    #ifdef GISMO_WITH_PARDISO
    /// Pardiso (if enabled)
    typedef gsEigen::PardisoLDLT<gsEigen::SparseMatrix<T,0,int> > PardisoLDLT;
    typedef gsEigen::PardisoLLT <gsEigen::SparseMatrix<T,0,int> > PardisoLLT;
    typedef gsEigen::PardisoLU  <gsEigen::SparseMatrix<T,0,int> > PardisoLU;
    #endif

};

} // namespace gismo

// Type aliases (included after complete Eigen setup, like original gsMatrix.h pattern)
#include <gismo/Math/MatrixTypeAliases.h>

#include <gismo/Math/MatrixBlockView.h>
#include <gismo/Math/Matrix.h>
#include <gismo/Math/Vector.h>
#include <gismo/Math/AsMatrix.h>
#include <gismo/Math/SparseMatrix.h>
#include <gismo/Math/SparseVector.h>
#include <gismo/Math/SparseSolver.h>
#include <gismo/Math/Point.h>