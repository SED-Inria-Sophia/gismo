/** @file MultiBasis.assembler.hpp

    @brief Assembler-dependent functionality for MultiBasis.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#ifdef GISMO_WITH_ASSEMBLER_INTEGRATION

#include <gismo/Core/MultiPatch/MultiBasis.h>
#include <gsIO/gsOptionList.h>

namespace gismo
{

template <typename T>
void gsMultiBasis<T>::uniformRefine_withTransfer(
        gsSparseMatrix<T, RowMajor>& transferMatrix,
        const gsBoundaryConditions<T>& boundaryConditions,
        const gsOptionList& assemblerOptions,
        int numKnots,
        int mul,
        index_t unk)
{
    // Get coarse mapper
    gsDofMapper coarseMapper;
    this->getMapper(
            (dirichlet::strategy)assemblerOptions.askInt("DirichletStrategy",11),
            (iFace    ::strategy)assemblerOptions.askInt("InterfaceStrategy", 1),
            boundaryConditions,
            coarseMapper,
            unk
    );

    // Refine the bases and collect local transfer matrices
    std::vector< gsSparseMatrix<T,RowMajor> > localTransferMatrices(m_bases.size());
    for ( size_t k = 0; k!=m_bases.size(); ++k )
        m_bases[k]->uniformRefine_withTransfer(localTransferMatrices[k],numKnots,mul);

    // Get fine mapper
    gsDofMapper fineMapper;
    this->getMapper(
            (dirichlet::strategy)assemblerOptions.askInt("DirichletStrategy",11),
            (iFace    ::strategy)assemblerOptions.askInt("InterfaceStrategy", 1),
            boundaryConditions,
            fineMapper,
            unk
    );

    // Compute global transfer matrix
    this->constructTransferMatrix(coarseMapper, fineMapper, localTransferMatrices, transferMatrix);
}

template <typename T>
void gsMultiBasis<T>::uniformCoarsen_withTransfer(
        gsSparseMatrix<T, RowMajor>& transferMatrix,
        const gsBoundaryConditions<T>& boundaryConditions,
        const gsOptionList& assemblerOptions,
        int numKnots,
        index_t unk)
{
    // Get fine mapper
    gsDofMapper fineMapper;
    this->getMapper(
            (dirichlet::strategy)assemblerOptions.askInt("DirichletStrategy",11),
            (iFace    ::strategy)assemblerOptions.askInt("InterfaceStrategy", 1),
            boundaryConditions,
            fineMapper,
            unk
    );

    // Coarsen the bases and collect local transfer matrices
    std::vector< gsSparseMatrix<T,RowMajor> > localTransferMatrices(m_bases.size());
    for ( size_t k = 0; k!=m_bases.size(); ++k )
        m_bases[k]->uniformCoarsen_withTransfer(localTransferMatrices[k],numKnots);

    // Get coarse mapper
    gsDofMapper coarseMapper;
    this->getMapper(
            (dirichlet::strategy)assemblerOptions.askInt("DirichletStrategy",11),
            (iFace    ::strategy)assemblerOptions.askInt("InterfaceStrategy", 1),
            boundaryConditions,
            coarseMapper,
            unk
    );

    // Compute global transfer matrix
    this->constructTransferMatrix(coarseMapper, fineMapper, localTransferMatrices, transferMatrix);
}

} // namespace gismo

#endif // GISMO_WITH_ASSEMBLER_INTEGRATION