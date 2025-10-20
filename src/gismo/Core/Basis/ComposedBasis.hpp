/** @file gsComposedBasis.hpp

    @brief Implementation of the gsComposedBasis class.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s):
        H.M. Verhelst
        S. Imperatore
*/

#pragma once

#include <gismo/Math/GridIterator.h>
#include <gismo/Core/Mesh/Mesh.h>
#include <gismo/Core/Geometry/ComposedGeometry.h>
//#include <gsDomain/gsDomainIterator.h>
#include <gismo/Common/Memory.h>

namespace gismo
{

template <class T>
gsComposedBasis<T>::gsComposedBasis()
:
m_composition(nullptr),
m_basis(nullptr)
{
}

template <class T>
gsComposedBasis<T>::gsComposedBasis(const typename gsComposedBasis<T>::CompositionT * composition,
                                    const typename gsComposedBasis<T>::BasisT * basis)
:
gsComposedBasis(memory::make_shared_not_owned(composition),
                memory::make_shared_not_owned(basis) )
{
}

template <class T>
gsComposedBasis<T>::gsComposedBasis(const typename gsComposedBasis<T>::CompositionT & composition,
                                    const typename gsComposedBasis<T>::BasisT & basis)
:
gsComposedBasis(&composition,&basis)
{
}

template <class T>
gsComposedBasis<T>::gsComposedBasis(typename gsComposedBasis<T>::CompositionT::Ptr composition,
                                    typename gsComposedBasis<T>::BasisT::Ptr basis)
:
m_composition(composition),
m_basis(basis)
{
    GISMO_ENSURE(m_composition->domainDim()==m_composition->targetDim(),
        "Domain dimension of the composition "<<
        " should be equal to the target dimension of the composition "<<
        ", but composition.domainDim() = "<<m_composition->domainDim()<<
        " and composition.targetDim() = )"<<m_composition->targetDim());
    GISMO_ENSURE(m_basis->domainDim()==m_composition->targetDim(),
        "Domain dimension of the basis "<<
        " should be equal to the target dimension of the composition "<<
        ", but basis.domainDim() = "<<m_basis->domainDim()<<
        " and composition.targetDim() = )"<<m_composition->targetDim());
}

template <class T>
gsComposedBasis<T>::gsComposedBasis(const gsComposedBasis<T> & other)
:
m_composition(other.m_composition),
m_basis(other.m_basis->clone())
{
}

template <class T>
gsComposedBasis<T> & gsComposedBasis<T>::operator=(const gsComposedBasis<T> & other)
{
    if (this != &other)
    {
        m_composition = other.m_composition;
        m_basis = other.m_basis->clone();
    }
    return *this;
}

template <class T>
memory::unique_ptr<gsGeometry<T> > gsComposedBasis<T>::makeGeometry( gsMatrix<T>coefs ) const
{
    return memory::unique_ptr<gsGeometry<T> >(new gsComposedGeometry<T>(*this, give(coefs)));
}

template <class T>
short_t gsComposedBasis<T>::domainDim() const
{
    return m_composition->domainDim();
}

template <class T>
short_t gsComposedBasis<T>::targetDim() const
{
    return m_basis->targetDim();
}

template <class T>
memory::shared_ptr<gsDomain<T> > gsComposedBasis<T>::domain() const
{
    return m_basis->domain();
}

template <class T>
short_t gsComposedBasis<T>::maxDegree() const
{
    return m_basis->maxDegree();
}

template <class T>
gsMatrix<T> gsComposedBasis<T>::support() const
{
    return m_basis->support();
}

template <class T>
gsMatrix<T> gsComposedBasis<T>::support(const index_t & i) const
{
    // gsMatrix<T> supp = m_basis->support(i);
    // gsGridIterator<T,CUBE> pt(supp,math::pow(2,this->domainDim()));
    // supp = pt.toMatrix();
    // gsMatrix<T> result = supp;

    // m_composition->invertPoints(supp,result,1e-4,true);

    // supp.conservativeResize(this->domainDim(),2);
    // for (index_t d=0; d!=this->domainDim(); d++)
    //     supp.row(d)<<result.row(d).array().minCoeff(),result.row(d).array().maxCoeff();

    // // If the volume is zero, the computation failed
    // if (supp.prod()==0)
    //     return this->support();
    // else
    //     return supp;

    // @hverhelst: the above implementation is not robust and might yield zero volumes.
    // Therefore, we return the full support. This function is usually only called in plots
    return this->support();
}

template <class T>
void gsComposedBasis<T>::active_into(const gsMatrix<T> & u, gsMatrix<index_t>& result) const
{
    gsMatrix<T> coords = m_composition->eval(u);
    this->_applyBounds(coords);
    m_basis->active_into(coords,result);
}

// void evalAllDers_into(const gsMatrix<T> & u, int n,
//                         std::vector<gsMatrix<T> >& result,
//                         bool sameElement) const
// {
//     gsMatrix<T> coords = m_composition->eval(u);
//     this->_applyBounds(coords);
//     m_basis->evalAllDers_into(coords,n,result,sameElement);
// }

template <class T>
void gsComposedBasis<T>::eval_into(const gsMatrix<T>& u, gsMatrix<T>& result) const
{
    gsMatrix<T> coords = m_composition->eval(u);
    this->_applyBounds(coords);
    m_basis->eval_into(coords,result);
}

template <class T>
void gsComposedBasis<T>::evalSingle_into(index_t i, const gsMatrix<T>& u, gsMatrix<T>& result) const
{
    gsMatrix<T> coords = m_composition->eval(u);
    this->_applyBounds(coords);
    m_basis->evalSingle_into(i,coords,result);
}

template <class T>
void gsComposedBasis<T>::deriv_into(const gsMatrix<T>& u, gsMatrix<T>& result) const
{
    index_t domainDim, targetDim;
    domainDim = m_composition->domainDim();
    targetDim = m_composition->targetDim();

    gsFuncData<T> fd(NEED_VALUE | NEED_DERIV);
    m_composition->compute(u,fd);

    gsMatrix<T> coord, deriv, tmp, compderiv;
    coord = fd.values[0];
    compderiv = fd.values[1];

    this->_applyBounds(coord);
    m_basis->deriv_into(coord,deriv);
    const index_t numAct = deriv.rows() / domainDim;

    result.resize(numAct*domainDim*m_basis->targetDim(),u.cols());
    for (index_t k = 0; k!=u.cols(); k++)
    {
        gsAsMatrix<T,Dynamic,Dynamic> compderivMat = compderiv.reshapeCol(k,domainDim,targetDim);
        // gsAsMatrix<T,Dynamic,Dynamic> derivMat = deriv.reshapeCol(k,m_basis->domainDim(),m_basis->targetDim());
        for (index_t act = 0; act!=numAct; act++)
            result.block(act*domainDim,k,domainDim*m_basis->targetDim(),1).reshaped(domainDim,m_basis->targetDim()) = compderivMat*deriv.block(act*domainDim,k,domainDim*m_basis->targetDim(),1).reshaped(domainDim,m_basis->targetDim());
    }
}

template <class T>
void gsComposedBasis<T>::derivSingle_into(index_t i, const gsMatrix<T>& u, gsMatrix<T>& result) const
{
    index_t domainDim, targetDim;
    domainDim = m_composition->domainDim();
    targetDim = m_composition->targetDim();

    gsFuncData<T> fd(NEED_VALUE | NEED_DERIV);
    m_composition->compute(u,fd);

    gsMatrix<T> coord, deriv, tmp, compderiv;
    coord = fd.values[0];
    compderiv = fd.values[1];

    m_basis->derivSingle_into(i,coord,deriv);

    result.resize(m_basis->targetDim()*domainDim,u.cols());
    for (index_t k = 0; k!=u.cols(); k++)
    {
        gsAsMatrix<T,Dynamic,Dynamic> compderivMat = compderiv.reshapeCol(k,domainDim,targetDim);
        gsAsMatrix<T,Dynamic,Dynamic> derivMat = deriv.reshapeCol(k,m_basis->domainDim(),m_basis->targetDim());
        // The product has size:
        // (domainDim x targetDim) x (m_basis->domainDim(),m_basis->targetDim())
        //  =
        // (domainDim x m_basis->targetDim())
        gsAsMatrix<T,Dynamic,Dynamic> resultMat = result.reshapeCol(k,domainDim,m_basis->targetDim());
        resultMat = compderivMat * derivMat;
    }
}

template <class T>
void gsComposedBasis<T>::deriv2_into(const gsMatrix<T>& u, gsMatrix<T>& result) const
{
    // The second derivative of the composed basis is computed as follows:
    // Let d2B/dudv be the second derivative of the basis, dC/dx be the derivative of the composition.
    // In 2D:
    // d2B/dudv = [[ d2B/dudu, d2B/dudv],
    //             [ d2B/dvdu, d2B/dvdv]]  (2x2 matrix)
    // dC/dx    = [[ du/dx1  , dv/dx1  ],
    //             [ du/dx2  , dv/dx2  ]]  (2x2 matrix)
    // Then the second derivative of the basis is:
    // d2B/dxidxj = dC/dx * d2B/dudv * dC/dx^T
    //            = [[ du/dx1  , dv/dx1  ],  * [[ d2B/dudu, d2B/dudv],  * [[ du/dx1  , du/dx2  ],
    //               [ du/dx2  , dv/dx2  ]]     [ d2B/dvdu, d2B/dvdv]]     [ dv/dx1  , dv/dx2  ]]
    // d2B/dxdx   = ( d2B/dudu * du/dx1 + d2B/dudv * dv/dx1 ) * du/dx1 + ( d2B/dvdu * du/dx1 + d2B/dvdv * dv/dx1 ) * dv/dx1
    //            = d2B/dudu * du/dx1 * du/dx1 + d2B/dudv * du/dx1 * dv/dx1 + d2B/dvdu * du/dx1 * dv/dx1 + d2B/dvdv * dv/dx1 * du/dx1
    // d2B/dydy   = ( d2B/dudu * du/dx2 + d2B/dudv * dv/dx2 ) * du/dx2 + ( d2B/dvdu * du/dx2 + d2B/dvdv * dv/dx2 ) * dv/dx2
    //            = d2B/dudu * du/dx2 * du/dx2 + d2B/dudv * du/dx2 * dv/dx2 + d2B/dvdu * du/dx2 * dv/dx2 + d2B/dvdv * dv/dx2 * du/dx2
    // d2B/dxdy   = ( d2B/dudu * du/dx1 + d2B/dudv * dv/dx1 ) * du/dx2 + ( d2B/dvdu * du/dx1 + d2B/dvdv * dv/dx1 ) * dv/dx2
    //            = d2B/dudu * du/dx1 * du/dx2 + d2B/dudv * du/dx1 * dv/dx2 + d2B/dvdu * du/dx2 * dv/dx1 + d2B/dvdv * dv/dx1 * du/dx2
    // d2B/dydx   = ( d2B/dudu * du/dx2 + d2B/dudv * dv/dx2 ) * du/dx1 + ( d2B/dvdu * du/dx2 + d2B/dvdv * dv/dx2 ) * dv/dx1
    //            = d2B/dudu * du/dx2 * du/dx1 + d2B/dudv * du/dx2 * dv/dx1 + d2B/dvdu * du/dx1 * dv/dx2 + d2B/dvdv * dv/dx2 * du/dx1

    // Get the domain and target dimensions
    index_t domainDim, targetDim, bDomainDim;
    domainDim = m_composition->domainDim();
    targetDim = m_composition->targetDim();
    bDomainDim = m_basis->domainDim();
    GISMO_ASSERT(1==m_basis->targetDim(),"The basis should be scalar-valued"); // HMV: I think

    // Compute the composition and its derivatives
    gsFuncData<T> fd(NEED_VALUE | NEED_DERIV);
    m_composition->compute(u,fd);

    gsMatrix<T> coord, deriv2, tmphess, tmpder2, compderiv, hessMat;
    coord = fd.values[0];
    compderiv = fd.values[1];

    this->_applyBounds(coord);

    // Compute the second derivative of the basis
    // The number of second derivatives per component is d(d+1)/2
    const index_t numSecDeriv = bDomainDim*(bDomainDim+1)/2;
    m_basis->deriv2_into(coord,deriv2);

    // Compute for every point for every basis function
    const index_t numAct = deriv2.rows() / numSecDeriv;
    result.resize(numAct*numSecDeriv,u.cols());
    for (index_t k = 0; k!=u.cols(); k++)
    {
        gsAsMatrix<T,Dynamic,Dynamic> compderivMat = compderiv.reshapeCol(k,domainDim,targetDim);
        for (index_t act = 0; act!=numAct; act++)
        {
            tmpder2 = deriv2.block(act*numSecDeriv,k,numSecDeriv,1);
            hessMat = util::secDerToHessian(tmpder2,bDomainDim).reshape(bDomainDim,bDomainDim);
            tmphess = compderivMat*hessMat*compderivMat.transpose();
            util::hessianToSecDer(tmphess,bDomainDim,tmpder2);
            result.block(act*numSecDeriv,k,numSecDeriv,1) = tmpder2;
        }
    }
}

// void control_deriv_into(const gsMatrix<T> & points, gsMatrix<T> & result)
// {
//     // The number of rows is the target dimension times the number of controls
//     // The number of cols is the number of points
//     result.resize(targetDim()*m_composition->nControls(),points.cols());

//     // Pre-compute the coordinates of the composition, the derivatives of G and the derivatives of the composition
//     gsMatrix<T> c, dc, dG;
//     m_composition->eval_into(points,c);
//     m_composition->control_deriv_into(points,dc);
//     m_geom->deriv_into(c,dG);

//     // Store some sizes
//     index_t nControls = m_composition->nControls();
//     index_t dd = m_geom->domainDim();
//     index_t td = m_geom->targetDim();

//     // Loop over the points
//     for (index_t k=0; k!=points.cols(); k++)
//     {
//         // We need to compute dG/dpi = dG/dc * dc/dpi
//         gsAsMatrix<T> DG = result.reshapeCol(k,nControls,td);
//         DG = dc.reshapeCol(k,nControls,dd) * dG.reshapeCol(k,dd,td);
//     }
// }

template <class T>
short_t gsComposedBasis<T>::degree(short_t i) const
{
    return m_basis->degree(i);
}

template <class T>
gsMatrix<index_t> gsComposedBasis<T>::boundaryOffset(boxSide const & s, index_t offset) const
{
    return m_basis->boundaryOffset(s,offset);
}

template <class T>
void gsComposedBasis<T>::matchWith(const boundaryInterface & bi, const gsBasis<T> & other,
                                    gsMatrix<index_t> & bndThis, gsMatrix<index_t> & bndOther, index_t offset) const
{
    return m_basis->matchWith(bi,other,bndThis,bndOther,offset);
}

// template <class T>
// typename gsBasis<T>::domainIter gsComposedBasis<T>::makeDomainIterator() const
// {
//     return m_basis->makeDomainIterator();
// }

// template <class T>
// typename gsBasis<T>::domainIter gsComposedBasis<T>::makeDomainIterator(const boxSide & s) const
// {
//     return m_basis->makeDomainIterator(s);
// }

template <class T>
std::string gsComposedBasis<T>::detail() const
{
    return m_basis->detail();
}

template <class T>
size_t gsComposedBasis<T>::numElements(boxSide const & s) const
{
    return m_basis->numElements(s);
}

template <class T>
index_t gsComposedBasis<T>::size() const
{
    return m_basis->size();
}

template <class T>
void gsComposedBasis<T>::anchors_into(gsMatrix<T> & result) const
{
    gsMatrix<T> anchors = m_basis->anchors();
    m_composition->invertPoints(anchors,result,1e-2,true);
}

template <class T>
void gsComposedBasis<T>::connectivity(const gsMatrix<T> & nodes, gsMesh<T> & mesh) const
{
    m_basis->connectivity(nodes,mesh);
}

template <class T>
void gsComposedBasis<T>::uniformRefine(int numKnots, int mul, short_t dir)
{
    m_basis->uniformRefine(numKnots,mul,dir);
}

template <class T>
void gsComposedBasis<T>::uniformRefine_withCoefs(gsMatrix<T>& coefs, int numKnots, int mul, short_t dir)
{
    m_basis->uniformRefine_withCoefs(coefs,numKnots,mul,dir);
}

template <class T>
void gsComposedBasis<T>::degreeElevate(short_t const & i, short_t const dir)
{
    m_basis->degreeElevate(i,dir);
}

template <class T>
void gsComposedBasis<T>::mapMesh(gsMesh<T> & mesh) const
{
    const int pDim = this->domainDim();

    gsMatrix<T> tmp, point;

    for (size_t i = 0; i!= mesh.numVertices(); ++i)
    {
        point = tmp = mesh.vertex(i).topRows(pDim);
        m_composition->invertPoints(point,tmp,1e-2,true);
        // m_composition->eval_into(point,tmp);
        mesh.vertex(i).topRows(pDim) = tmp.topRows(pDim);
    }
}

template <class T>
const typename gsComposedBasis<T>::CompositionT & gsComposedBasis<T>::composition() const
{
    return *m_composition;
}

template <class T>
typename gsComposedBasis<T>::CompositionT & gsComposedBasis<T>::composition()
{
    return *m_composition;
}

template <class T>
const typename gsComposedBasis<T>::BasisT & gsComposedBasis<T>::basis() const
{
    return *m_basis;
}

template <class T>
std::ostream & gsComposedBasis<T>::print(std::ostream &os) const
{
    os <<"Composite basis:\n";
    os << "* Composition"
        << " ( R^" << m_composition->domainDim() << " --> R^" << m_composition->targetDim() << ") at "<<m_composition<<":\n"
        << *m_composition<<"\n";
    os << "* Basis"
        << " ( R^" << m_basis->domainDim() << " --> R^" << m_basis->targetDim() << ") at "<<m_basis<<":\n"
        << *m_basis<<"\n";
    return os;
}


template <class T>
void gsComposedBasis<T>::_applyBounds(gsMatrix<T> & coords) const
{
    for (index_t k=0; k!=coords.cols(); k++)
    {
        coords.col(k) = coords.col(k).cwiseMax(m_basis->support().col(0));
        coords.col(k) = coords.col(k).cwiseMin(m_basis->support().col(1));
    }
}

} // namespace gismo
