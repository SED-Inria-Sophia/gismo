/** @file gsRationalBasis.h

    @brief Provides declaration of RationalBasis class.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include <gismo/Core/Basis/Basis.h>
#include <gismo/Core/Topology/Boundary.h>


namespace gismo
{

/** \brief
    Class that creates a rational counterpart for a given basis.

    A rational basis holds an inner (referred to as "source") basis of
    the given source type, and a matrix of coefficients defining a
    weight function in terms of the source basis.

    If \em w_i is the i-th weight coefficient and \em b_i the i-th basis
    function of the source basis, then the i-th basis function of the
    resulting rational basis is given by
    r_i(u) = b_i(u) w_i / (sum(b_j(u) w_j, j = 1, ..., size))

    If the weights are all equal to one (or all equal to a constant), the
    rational basis is identical (or a scalar multiple) to the source basis.

    Example: the rational version of a B-spline basis is a NURBS basis.

    \tparam SrcT the basis of which to create a rational version

    \ingroup basis
    \ingroup Core
*/

template<class SrcT>
class gsRationalBasis : public gsBasis< typename SrcT::Scalar_t >
{
public:
    typedef memory::shared_ptr< gsRationalBasis > Ptr;
    typedef memory::unique_ptr< gsRationalBasis > uPtr;

    typedef typename SrcT::Scalar_t Scalar_t;
    typedef Scalar_t T;

    // Obtain dimension from the source basis
    static const int Dim = SrcT::Dim;

    static const bool IsRational = true;

    typedef gsBasis<T> Base;

    /// Associated source basis type
    typedef SrcT SourceBasis;

public:

    /// Default empty constructor
    gsRationalBasis() :  Base(), m_src(nullptr) { }

    /// Construct a rational counterpart of basis
    gsRationalBasis(SrcT* basis)
    : m_src(basis)
    {
        m_weights.setOnes(basis->size(), 1);
    }

    /// Construct a rational counterpart of basis
    gsRationalBasis(const SrcT & basis)
    : m_src(basis.clone().release())
    {
        m_weights.setOnes(basis.size(), 1);
    }

    /// Construct a rational counterpart of basis
    gsRationalBasis(SrcT * basis, gsMatrix<T> w)
    : m_src (basis), m_weights(give(w))
    {
        GISMO_ASSERT(m_weights.rows() == m_src->size(),
                     "Invalid basis/weights ("<<m_weights.rows()<<"/"<<m_src->size());
    }

    /// Copy Constructor
    gsRationalBasis(const gsRationalBasis & o) : Base(o)
    {
        m_src= o.source().clone().release();
        m_weights = o.weights()  ;
    }

    /// Assignment operator
    gsRationalBasis& operator=( const gsRationalBasis & o)
    {
        if ( this != &o )
        {
            delete m_src;
            m_src = o.source().clone().release();
            m_weights = o.weights()  ;
        }
        return *this;
    }

    // Destructor
    virtual ~gsRationalBasis()
    {
        delete m_src;
    }

    /// Check the rational basis for consistency
    bool check() const
    {
        return (
            m_weights.size() == m_src->size()
            );
    }

    memory::unique_ptr<gsBasis<T> > makeNonRational() const
    { return m_src->clone(); }

public:

// ***********************************************
// Virtual member functions overriding source basis
// ***********************************************

    short_t domainDim() const { return Dim; }

    index_t size() const { return m_src->size(); }

    index_t size(index_t const& k) const{ return m_src->size(k); }

    size_t numElements(boxSide const & s = 0) const { return m_src->numElements(s); }
    //using Base::numElements; //unhide

    /// See \ref gsBasis for a description
    size_t elementIndex(const gsVector<T> & u ) const { return m_src->elementIndex(u); }

    /// See \ref gsBasis for a description
    gsMatrix<T> elementInSupportOf(index_t j) const { return m_src->elementInSupportOf(j); }

    void active_into(const gsMatrix<T> & u, gsMatrix<index_t>& result) const
    { m_src->active_into(u, result); }

    virtual const gsBasis<T> & component(short_t i) const { return m_src->component(i); }
    using Base::component;

    gsMatrix<index_t> allBoundary( ) const {return m_src->allBoundary(); }

    gsMatrix<index_t> boundaryOffset(boxSide const & s, index_t offset ) const
    { return m_src->boundaryOffset(s,offset); }

    virtual index_t functionAtCorner(boxCorner const & c) const
    { return m_src->functionAtCorner(c); }

    // Look at gsBasis class for a description
    short_t degree(short_t i = 0) const {return m_src->degree(i); }

    // Look at gsBasis class for a description
    short_t maxDegree()   const   {return m_src->maxDegree(); }

    // Look at gsBasis class for a description
    short_t minDegree()   const    {return m_src->minDegree(); }

    // Look at gsBasis class for a description
    short_t totalDegree() const     {return m_src->totalDegree(); }

    void uniformRefine(int numKnots = 1, int mul = 1, short_t dir = -1)
    {
        m_src->uniformRefine_withCoefs(m_weights, numKnots, mul, dir);
    }

    void uniformRefine_withCoefs(gsMatrix<T>& coefs, int numKnots = 1,  int mul = 1, short_t const dir = -1);

    void uniformRefine_withTransfer(gsSparseMatrix<T,RowMajor> & transfer, int numKnots = 1, int mul = 1);

    /// See \ref gsBasis
    void refine(gsMatrix<T> const & boxes, int refExt = 0)
    {
        GISMO_UNUSED(refExt);
        m_src->refine_withCoefs( m_weights, boxes );
    }

    /**
     * @brief Refines specified areas or boxes, depending on underlying basis.
     *
     * @param boxes See the function gsBasis::refineElements() of the underlying
     * basis for syntax.
     */
    void refineElements( std::vector<index_t> const & boxes)
    {
        // call the refineElements_withCoefs-function of the underlying
        // basis, where the weights are used as coefficients
        m_src->refineElements_withCoefs( m_weights, boxes );
    }


    /**
     * @brief Refines specified areas or boxes, depending on underlying basis.
     *
     * @param coefs Coefficients, given as gsMatrix of size \f$ n \times d\f$,
     * where \f$n\f$ is the number of basis functions and \f$d\f$ is the target
     * dimension.
     * @param boxes See the function gsBasis::refineElements() of the underlying
     * basis for syntax.
     */
    void refineElements_withCoefs(gsMatrix<T> & coefs,std::vector<index_t> const & boxes);

    void degreeElevate(short_t const& i = 1, short_t const dir = -1)
    {
        memory::unique_ptr<gsGeometry<T> > tmp = m_src->makeGeometry(give(m_weights));
        tmp->degreeElevate(i,dir);
        tmp->coefs().swap(m_weights);
        delete m_src;
        m_src = static_cast<SrcT*>(tmp->basis().clone().release());
    }

    void degreeIncrease(short_t const& i = 1, short_t const dir = -1)
    {
        memory::unique_ptr<gsGeometry<T> > tmp = m_src->makeGeometry(give(m_weights));
        tmp->degreeIncrease(i,dir);
        tmp->coefs().swap(m_weights);
        delete m_src;
        m_src = static_cast<SrcT*>(tmp->basis().clone().release());
    }

    void degreeReduce(short_t const& i = 1, short_t const dir = -1)
    {
        memory::unique_ptr<gsGeometry<T> > tmp = m_src->makeGeometry(give(m_weights));
        tmp->degreeReduce(i,dir);
        tmp->coefs().swap(m_weights);
        delete m_src;
        m_src = static_cast<SrcT*>(tmp->basis().clone().release());
    }

    void degreeDecrease(short_t const& i = 1, short_t const dir = -1)
    {
        memory::unique_ptr<gsGeometry<T> > tmp = m_src->makeGeometry(give(m_weights));
        tmp->degreeDecrease(i,dir);
        tmp->coefs().swap(m_weights);
    }

    /*
    GISMO_UPTR_FUNCTION_DEF(gsBasis<T>, boundaryBasis, boxSide const &)
    {
        typename SrcT::BoundaryBasisType * bb = m_src->boundaryBasis(s);
        gsMatrix<index_t> ind = m_src->boundary(s);
        gsMatrix<T> ww( ind.size(),1);
        for ( index_t i=0; i<ind.size(); ++i)
            ww(i,0) = m_weights( ind(i,0), 0);
        return new BoundaryBasisType(bb.release(), give(ww));// note: constructor consumes the pointer
    }
    */

    memory::shared_ptr<gsDomain<T> > domain() const { return m_src->domain(); }

    void anchors_into(gsMatrix<T> & result) const
    { return m_src->anchors_into(result); }

    void anchor_into(index_t i, gsMatrix<T> & result) const
    { return m_src->anchor_into(i,result); }

    // Look at gsBasis class for documentation
    void connectivity(const gsMatrix<T> & nodes, gsMesh<T> & mesh) const
    { return m_src->connectivity(nodes, mesh); }

    gsMatrix<T> support() const {return m_src->support(); }

    gsMatrix<T> support(const index_t & i) const {return m_src->support(i); }

    void eval_into(const gsMatrix<T> & u, gsMatrix<T>& result) const;

    void evalSingle_into(index_t i, const gsMatrix<T> & u, gsMatrix<T>& result) const ;

    void evalFunc_into(const gsMatrix<T> & u, const gsMatrix<T> & coefs, gsMatrix<T>& result) const;

    //void evalAllDers_into(const gsMatrix<T> & u, int n,
    //                      std::vector<gsMatrix<T> >& result, bool sameElement = false) const;

    void deriv_into(const gsMatrix<T> & u, gsMatrix<T>& result ) const ;
    void derivSingle_into(index_t i, const gsMatrix<T> & u, gsMatrix<T>& result) const ;

    void deriv2_into(const gsMatrix<T> & u, gsMatrix<T>& result ) const;
    // void deriv2Single_into(index_t i, const gsMatrix<T> & u, gsMatrix<T>& result) const ;

    /// Returns the source basis of the rational basis
    const SrcT & source () const
    { return *m_src; }

    SrcT & source ()
    { return *m_src; }

    /// Returns the weights of the rational basis
    const gsMatrix<T> & weights() const { return m_weights; }

    /// Returns the weights of the rational basis
    gsMatrix<T> & weights()  { return m_weights; }


    /// Returns true, since by definition a gsRationalBasis is rational.
    virtual bool isRational() const { return true;}

    /// Access to i-th weight
    T & weight(int i)             { return m_weights(i); }

    /// Const access to i-th weight
    const T & weight(int i) const { return m_weights(i); }

    /// Set weights
    void setWeights(gsMatrix<T> const & w)
    {
        GISMO_ASSERT( w.cols() == 1, "Weights should be scalars" ) ;
        m_weights = w;
    }

    virtual void matchWith(const boundaryInterface & bi, const gsBasis<T> & other,
                           gsMatrix<index_t> & bndThis, gsMatrix<index_t> & bndOther,
                           index_t offset = 0) const
    {
        if ( const gsRationalBasis * _other = dynamic_cast<const gsRationalBasis*>(&other) )
            m_src->matchWith(bi,*_other->m_src,bndThis,bndOther, offset);
        else
            m_src->matchWith(bi,other,bndThis,bndOther, offset);
    }

    /// Returns a matrix of projective coefficients. The input \a
    /// coefs are affine coefficients for this basis
    gsMatrix<T> projectiveCoefs(const gsMatrix<T> & coefs) const
    { return projectiveCoefs(coefs, m_weights); }

    /// Returns a matrix of projective coefficients. The input \a
    /// coefs are affine coefficients and weights
    static gsMatrix<T> projectiveCoefs(const gsMatrix<T> & coefs, const gsMatrix<T> & weights)
    {
        GISMO_ASSERT(coefs.rows() == weights.rows(),
                     "Invalid basis/coefficients ("<<coefs.rows()<<"/"<<weights.rows());
        const index_t n = coefs.cols();
        gsMatrix<T> rvo(coefs.rows(), n + 1);
        // switch from control points (n-dimensional) to
        // "projective control points" ((n+1)-dimensional),
        // where the last coordinate is the weight.
        rvo.leftCols(n).noalias() = weights.asDiagonal() * coefs;
        rvo.col(n)                = weights;
        return rvo;
    }

    /// Sets the weights and the \a coefs to be the affine
    /// coefficients corresponding to the projective coefficients
    /// \a pr_coefs
    static void setFromProjectiveCoefs(const gsMatrix<T> & pr_coefs,
                                       gsMatrix<T> & coefs, gsMatrix<T> & weights)
    {
        const index_t n = pr_coefs.cols() - 1;
        weights = pr_coefs.col( n );
        coefs   = pr_coefs.leftCols(n).array().colwise() / weights.col(0).array();
        // equiv: coefs = pr_coefs.leftCols(n).array() / weights.replicate(1,n).array();
    }

    GISMO_DEPRECATED
    typename gsBasis<T>::domainIter makeDomainIterator() const
    {
//        gsWarn<< "rational domain iterator with evaluate the source.\n";
        return m_src->makeDomainIterator();
    }

    GISMO_DEPRECATED
    typename gsBasis<T>::domainIter makeDomainIterator(const boxSide & s) const
    {
//        gsWarn<< "rational domain boundary iterator with evaluate the source.\n";
        return m_src->makeDomainIterator(s);
    }

// Data members
protected:

    // Source basis
    SrcT * m_src;

    // Weight vector of size: size() x 1
    gsMatrix<T> m_weights;

}; // class gsRationalBasis

} // namespace gismo

#include <gismo/Core/Basis/RationalBasis.hpp>