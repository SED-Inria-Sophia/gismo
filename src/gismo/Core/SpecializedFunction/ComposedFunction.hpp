/** @file gsComposedFunction.hpp

    @brief Implementation of the gsComposedFunction class.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s):
        H.M. Verhelst
        S. Imperatore
*/

#pragma once

#include <gismo/Core/Function/FuncData.h>
#include <gismo/Core/Geometry/Geometry.h>

namespace gismo
{

template <class T>
gsComposedFunction<T>::gsComposedFunction()
:
m_composition(nullptr),
m_function(nullptr)
{
}

template <class T>
gsComposedFunction<T>::gsComposedFunction( const CompositionT * composition,
                                           const FunctionT * function)
:
gsComposedFunction(memory::make_shared_not_owned(composition),
                   memory::make_shared_not_owned(function))
{}

template <class T>
gsComposedFunction<T>::gsComposedFunction( const CompositionT & composition,
                                           const FunctionT & function)
:
gsComposedFunction(memory::make_shared(composition.clone().release()),
                   memory::make_shared(function.clone().release()))
{}

template <class T>
gsComposedFunction<T>::gsComposedFunction( const typename gsFunction<T>::Ptr composition,
                                           const typename gsFunction<T>::Ptr function)
:
m_composition(composition),
m_function(function)
{
    GISMO_ENSURE(m_function->domainDim()==m_composition->targetDim(),
        "Domain dimension of the function "<<
        " should be equal to the target dimension of the composition "<<
        ", but basis.domainDim() = "<<m_function->domainDim()<<
        " and composition.targetDim() = )"<<m_composition->targetDim());
}

template <class T>
const typename gsComposedFunction<T>::CompositionT & gsComposedFunction<T>::composition() const
{
    return *m_composition;
}

template <class T>
const typename gsComposedFunction<T>::FunctionT & gsComposedFunction<T>::function() const
{
    return *m_function;
}

template <class T>
short_t gsComposedFunction<T>::domainDim() const
{
    return m_composition->domainDim();
}

template <class T>
short_t gsComposedFunction<T>::targetDim() const
{
    return m_function->targetDim();
}

template <class T>
gsMatrix<T> gsComposedFunction<T>::support() const
{
    return m_composition->support();
}

template <class T>
void gsComposedFunction<T>::eval_into(const gsMatrix<T>& u, gsMatrix<T>& result) const
{
    gsMatrix<T> coords = m_composition->eval(u);
    m_function->eval_into(coords,result);
}

template <class T>
void gsComposedFunction<T>::deriv_into(const gsMatrix<T>& u, gsMatrix<T>& result) const
{
    index_t domainDim, targetDim;
    domainDim = m_composition->domainDim();
    targetDim = m_composition->targetDim();

    gsFuncData<T> fd(NEED_VALUE | NEED_DERIV);
    m_composition->compute(u,fd);

    gsMatrix<T> coord, deriv, tmp, compderiv;
    coord = fd.values[0];
    compderiv = fd.values[1];

    m_function->deriv_into(coord,deriv);
    result.resize(m_function->targetDim()*domainDim,u.cols());
    for (index_t k = 0; k!=u.cols(); k++)
    {
        gsAsMatrix<T,Dynamic,Dynamic> compderivMat = compderiv.reshapeCol(k,domainDim,targetDim);
        gsAsMatrix<T,Dynamic,Dynamic> derivMat = deriv.reshapeCol(k,m_function->domainDim(),m_function->targetDim());
        // The product has size:
        // (domainDim x targetDim) x (m_function->domainDim(),m_function->targetDim())
        //  =
        // (domainDim x m_function->targetDim())
        gsAsMatrix<T,Dynamic,Dynamic> resultMat = result.reshapeCol(k,domainDim,m_function->targetDim());
        resultMat = compderivMat*derivMat;
    }
}

template <class T>
void gsComposedFunction<T>::deriv2_into(const gsMatrix<T>& u, gsMatrix<T>& result) const
{
    GISMO_NO_IMPLEMENTATION;
}

template <class T>
std::ostream & gsComposedFunction<T>::print(std::ostream &os) const
{
    os <<"Composite function:\n";
    os << "* Composition ( R^" << m_composition->domainDim() << " --> R^" << m_composition->targetDim() << "):\n"
       << *m_composition<<"\n"
       << "(address: "<<m_composition<<")\n";
    os << "* Function ( R^" << m_function->domainDim() << " --> R^" << m_function->targetDim() << "):\n"
       << *m_function<<"\n"
       << "(address: "<<m_function<<")\n";
    return os;
}

} // namespace gismo
