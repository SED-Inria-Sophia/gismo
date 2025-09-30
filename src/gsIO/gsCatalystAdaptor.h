/** @file gsCatalystAdaptor.h

    @brief Provides a thin adapter between gismo data structures and
    ParaView Catalyst pipelines.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): G+Smo Codex agent
*/

#pragma once

#include <string>
#include <vector>

#include <gsCore/gsForwardDeclarations.h>
#include <gsCore/gsMultiPatch.h>
#include <gsIO/gsWriteParaview.h>
#include <gsAssembler/gsExprEvaluator.h>
#include <gsIO/gsOptionList.h>

namespace gismo
{

/**
    @brief Minimal adaptor that forwards visualization data to ParaView Catalyst
    if available, or falls back to the legacy file writers otherwise.

    The class currently keeps the existing behaviour (writing .pvd/.vts files)
    when Catalyst support is not enabled at compile time. The interface mirrors
    the usage patterns present in paraview_example.cpp so client code can be
    migrated without changing the data sampling logic.

    @ingroup IO
*/
class gsCatalystAdaptor
{
public:
    gsCatalystAdaptor();
    ~gsCatalystAdaptor();

    /// Initialise the adaptor (start a Catalyst co-processing session).
    void initialize();

    /// Finalise the adaptor (close Catalyst session, flush state).
    void finalize();

    /// Publish a multi-patch geometry to Catalyst / fallback writer.
    void publishMultiPatch(const gsMultiPatch<real_t> & geometry,
                           const std::string & baseName,
                           const gsMultiBasis<real_t> * basis = nullptr,
                           unsigned npts = 1000u) const;

    /// Publish a set of points to Catalyst / fallback writer.
    void publishPoints(const gsMatrix<real_t> & points,
                       const std::string & baseName) const;

    /// Publish a Bezier representation of a multipatch geometry.
    void publishBezier(const gsMultiPatch<real_t> & geometry,
                       const std::string & baseName,
                       bool ctrlNet = false) const;

    /// Publish the evaluation of an expression with respect to a geometry map.
    template<class E>
    void publishExpression(gsExprEvaluator<real_t> & evaluator,
                           const expr::_expr<E> & expression,
                           typename gsExprEvaluator<real_t>::geometryMap geoMap,
                           const std::string & baseName) const
    {
#ifdef GISMO_WITH_CATALYST
        // Placeholder: integration with Catalyst should wrap the evaluator
        // result into Catalyst data descriptions. For now, we fall back to the
        // legacy file output to keep the workflow functional.
#endif
        evaluator.writeParaview(expression, geoMap, baseName);
    }

    /// RAII helper around time varying datasets.
    class Collection
    {
    public:
        Collection(gsCatalystAdaptor & adaptor,
                   const std::string & baseName,
                   gsExprEvaluator<real_t> * evaluator);

        /// Accessor for collection specific options.
        gsOptionList & options();

        /// Start a new time step for the given geometry.
        void newTimeStep(gsMultiPatch<real_t> * geometry, real_t time = -1.0);

        /// Publish a field associated with the current time step.
        template<class E>
        void addField(const expr::_expr<E> & expression,
                      const std::string & name)
        {
            m_collection.addField(expression, name);
        }

        /// Flush the pending time step.
        void saveTimeStep();

        /// Finalise the collection (writes the .pvd when Catalyst disabled).
        void save();

    private:
        gsCatalystAdaptor & m_adaptor;
        gsParaviewCollection m_collection;
    };

    /// Create a new collection helper.
    Collection createCollection(const std::string & baseName,
                                gsExprEvaluator<real_t> * evaluator);

private:
    bool m_initialized;
    index_t m_timeStep;
};

} // namespace gismo
