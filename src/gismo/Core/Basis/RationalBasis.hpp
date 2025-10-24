/** @file RationalBasis.hpp

    @brief Provides implementation of RationalBasis class.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

namespace gismo
{

template<class SrcT>
void gsRationalBasis<SrcT>::evalSingle_into(index_t i, const gsMatrix<T> & u, gsMatrix<T>& result) const
{
    m_src->evalSingle_into(i, u, result);
    result.array() *= m_weights.at(i);
    gsMatrix<T> denom;
    m_src->evalFunc_into(u, m_weights, denom);

    result.array() /= denom.array();
}


template<class SrcT>
void gsRationalBasis<SrcT>::eval_into(const gsMatrix<T> & u, gsMatrix<T>& result) const
{
    m_src->eval_into(u, result);
    const gsMatrix<index_t> act = m_src->active(u);

    gsMatrix<T> denom;
    m_src->evalFunc_into(u, m_weights, denom);

    for ( index_t j=0; j< act.cols(); ++j)
    {
        result.col(j) /= denom(j);
        for ( index_t i=0; i< act.rows(); ++i)
            result(i,j) *=  m_weights( act(i,j) ) ;
    }
}


// For non-specialized version (e.g. tensor product)
template<class SrcT>
void gsRationalBasis<SrcT>::evalFunc_into(const gsMatrix<T> & u, const gsMatrix<T> & coefs, gsMatrix<T>& result) const
{
    assert( coefs.rows() == m_weights.rows() ) ;

    // Compute projective coefficients
    const gsMatrix<T> tmp = m_weights.asDiagonal() * coefs;

    gsMatrix<T> denom;

    // Evaluate the projective numerator and the denominator
    m_src->evalFunc_into( u, tmp, result);
    m_src->evalFunc_into( u, m_weights, denom);

    // Divide numerator by denominator
    result.array().rowwise() /= denom.row(0).array();
    // equivalent:
    //for ( index_t j=0; j < u.cols(); j++ ) // for all points (columns of u)
    //    result.col(j) /= denom(0,j);
}


/* TODO
template<class SrcT>
void gsRationalBasis<SrcT>::evalAllDers_into(const gsMatrix<T> & u, int n,
                                             std::vector<gsMatrix<T> >& result, bool sameElement = false) const
{
    result.resize(n+1);

    std::vector<gsMatrix<T> > ev(n+1);

    m_src->evalAllDers_into(u, n, ev, sameElement);

    // find active basis functions
    gsMatrix<index_t> act;
    m_src->active_into(u,act);

    const int numAct = act.rows();

    // evaluate weights and their derivatives
    gsMatrix<T> Wval, Wder;
    m_src->evalFunc_into (u,  m_weights, Wval);
    m_src->derivFunc_into(u, m_weights, Wder);

    for (index_t i = 0; i < u.cols(); ++i)// for all parametric points
    {
        const T & Wval_i = Wval(0,i);
        // compute derivatives first since they depend on the function
        // values of the source basis
        der.col(i) *= Wval_i;

        for (index_t k = 0; k < numAct; ++k)
        {
            der.template block<Dim,1>(k*Dim,i).noalias() -=
                ev(k,i) * Wder.col(i);

            der.template block<Dim,1>(k*Dim,i) *=
                m_weights( act(k,i), 0 ) / (Wval_i*Wval_i);
        }

        // compute function values
        ev.col(i) /= Wval_i;

        for (index_t k = 0; k < numAct; ++k)
            ev(k,i) *= m_weights( act(k,i), 0 ) ;
    }
}
*/

template<class SrcT>
void gsRationalBasis<SrcT>::deriv_into(const gsMatrix<T> & u,
                                       gsMatrix<T>& result) const
{
    // Formula:
    // R_i' = (w_i N_i / W)' = w_i ( N_i'W - N_i W' ) / W^2

    gsMatrix<index_t> act;
    m_src->active_into(u,act);

    result.resize( act.rows()*Dim, u.cols() );

    std::vector<gsMatrix<T> > ev(2);
    m_src->evalAllDers_into(u, 1, ev);

    T W;
    gsMatrix<T> dW(Dim,1);

    for ( index_t i = 0; i!= result.cols(); ++i )// for all parametric points
    {
        // Start weight function computation
        W = 0.0;
        dW.setZero();
        for ( index_t k = 0; k != act.rows(); ++k ) // for all basis functions
        {
            const T curw = m_weights.at(act(k,i));
            W  += curw * ev[0](k,i);
            dW += curw * ev[1].template block<Dim,1>(k*Dim,i);
        }
        // End weight function computation

        result.col(i) = W * ev[1].col(i);  // N_i'W

        for ( index_t k = 0; k != act.rows() ; ++k) // for all basis functions
        {
            const index_t kd = k * Dim;

            result.template block<Dim,1>(kd,i).noalias() -=
                ev[0](k,i) * dW; // - N_i W'

            result.template block<Dim,1>(kd,i) *= m_weights.at( act(k,i) ) / (W * W);
        }
    }
}

template<class SrcT>
void gsRationalBasis<SrcT>::derivSingle_into(index_t i, const gsMatrix<T> & u, gsMatrix<T>& result) const
{
    // Formula:
    // R_i' = (w_i N_i / W)' = w_i ( N_i'W - N_i W' ) / W^2

    gsMatrix<T> W, dW;
    m_src->evalFunc_into (u, m_weights,  W);
    m_src->derivFunc_into(u, m_weights, dW);

    gsMatrix<> N, dN;
    m_src->evalSingle_into (i, u,  N);
    m_src->derivSingle_into(i, u, dN);

    result.resize(Dim, u.cols());
    for (index_t j = 0; j < u.cols(); ++j)
        result.col(j) = m_weights.at(i) * (W(0,j)*dN.col(j) - N(0,j)*dW.col(j)) / (math::pow(W(0,j),2));
}


template<class SrcT>
void gsRationalBasis<SrcT>::deriv2_into(const gsMatrix<T> & u, gsMatrix<T>& result ) const
{
    // Formula:
    // ( W^2 / w_k) * R_k'' = ( N_k'' W - N_k W'' ) - 2 N_k' W' + 2 N_k (W')^2 / W
    // ( W^2 / w_k) * d_ud_vR_k = ( d_ud_vN_k W - N_k d_ud_vW )
    //                          - d_uN_k d_vW - d_vN_k d_uW + 2 N_k d_uW d_vW / W

    static const int str = Dim * (Dim+1) / 2;

    gsMatrix<index_t> act;
    m_src->active_into(u,act);

    result.resize( act.rows()*str, u.cols() );

    T W;
    gsMatrix<T> dW(Dim,1), ddW(str,1);
    std::vector<gsMatrix<T> > ev(3);
    m_src->evalAllDers_into(u, 2, ev);

    for ( index_t i = 0; i!= u.cols(); ++i ) // for all points
    {
        // Start weight function computation
        W = 0.0;
        dW .setZero();
        ddW.setZero();
        for ( index_t k = 0; k != act.rows(); ++k ) // for all basis functions
        {
            //to do with lweights (local weights)
            const T curw = m_weights.at(act(k,i));
            W   += curw * ev[0](k,i);
            dW  += curw * ev[1].template block<Dim,1>(k*Dim,i);
            ddW += curw * ev[2].template block<str,1>(k*str,i);
        }
        // End weight function computation

        result.col(i) = W * ev[2].col(i); // N_k'' W

        for ( index_t k=0; k != act.rows() ; ++k ) // for all basis functions
        {
            const index_t kstr = k * str;
            const index_t kd   = k * Dim;

            result.template block<str,1>(kstr,i) -=
                ev[0](k,i) * ddW; // - N_k * W''

            result.template block<Dim,1>(kstr,i) +=
                // - 2 N_k' W' + 2 N_k (W')^2 / W
                ( (T)(2) * ev[0](k,i) / W ) * dW.cwiseProduct(dW)
                - (T)(2) * ev[1].template block<Dim,1>(kd,i).cwiseProduct(dW);

            int m = Dim;
            for ( int _u=0; _u != Dim; ++_u ) // for all mixed derivatives
                for ( int _v=_u+1; _v != Dim; ++_v )
                {
                    result(kstr + m++, i) +=
                        - ev[1](kd+_u,i) * dW.at(_v) // - du N_k * dv W
                        - ev[1](kd+_v,i) * dW.at(_u) // - dv N_k * du W
                        // + (T)(2) * N_k * du W * dv W / W
                        + (T)(2) * ev[0](k,i) * dW.at(_u) * dW.at(_v) / W;
                }

            result.template block<str,1>(kstr,i) *=
                m_weights.at( act(k,i) ) / (W*W); // * (w_k / W^2)
        }
    }
}

template<class SrcT>
void gsRationalBasis<SrcT>::uniformRefine_withCoefs(gsMatrix<T>& coefs, int numKnots,  int mul, int dir)
{
    GISMO_ASSERT( coefs.rows() == this->size() && m_weights.rows() == this->size(),
                  "Invalid dimensions" );
    /* // Using uniformRefine_withTransfer
    gsSparseMatrix<T, RowMajor> transfer;
    GISMO_ENSURE(-1==dir, "!!");
    m_src->uniformRefine_withTransfer(transfer, numKnots, mul);

    coefs     = transfer * ( m_weights.asDiagonal() * coefs);
    m_weights = transfer * m_weights;
    */

    // Using uniformRefine_withCoefs
    auto tmp = m_src->clone();
    coefs = m_weights.asDiagonal() * coefs;
    tmp->uniformRefine_withCoefs(coefs, numKnots);
    m_src->uniformRefine_withCoefs(m_weights, numKnots,mul,dir);

    // back to affine coefs
    coefs.array().colwise() /= m_weights.col(0).array();
    // equiv:
    // for (int i = 0; i < coefs.rows(); ++i)
    //    coefs.row(i) /= m_weights.at(i);
}

template<class SrcT>
void gsRationalBasis<SrcT>::uniformRefine_withTransfer(gsSparseMatrix<T,RowMajor> & transfer, int numKnots, int mul)
{
    // 1. Get source transfer matrix (while refining m_src)
    m_src->uniformRefine_withTransfer(transfer, numKnots, mul);

    // 2. Compute rational basis transfer matrix
    // To be applied on affine coefficients, as usual.
    // Transfer matrix for rational bases. Formula:
    //
    //   ( (T*m_weights)'*I )^{-1} * T*W
    //
    // Where ' denotes transpose, W is a diagonal matrix with
    // diagonal=m_weights, I is the identity and T the source
    // transfer matrix.
    // i.e. apply weight transform ( to compute weighted coefs),
    // apply T, return to affine by the inverse weight transform
    // (T*W)'*I )^{-1}.
    // In Eigen this could be something like
    // (transfer * m_weights).asDiagonal().inverse() * transfer * m_weights.asDiagonal() ;
    // but that has troubles with the sparse/diagonal expressions etc.
    // So we do it by using a temporary.

    const gsVector<T> tmp = m_weights ;
    m_weights.noalias() = transfer * tmp;  // Refine the weights as well

    transfer =  m_weights.cwiseInverse().asDiagonal() * transfer *  tmp.asDiagonal();
}


template<class SrcT>
void gsRationalBasis<SrcT>::refineElements_withCoefs(gsMatrix<T> & coefs,
                                                     std::vector<index_t> const & boxes)
{
    // switch from control points (n-dimensional) to
    // "projective control points" ((n+1)-dimensional),
    // where the last coordinate is the weight.
    gsMatrix<T> rw = projectiveCoefs(coefs, m_weights);

    // refine with these projective control points as coefficients.
    m_src->refineElements_withCoefs( rw, boxes );

    // Regain the new n-dimensional control points and the new
    // weights.
    setFromProjectiveCoefs(rw, coefs, m_weights);
}

} // namespace gismo