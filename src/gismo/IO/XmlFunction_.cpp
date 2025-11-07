/** @file XmlFunction_.cpp

    @brief Explicit instantiations and registrations for XML function serialization

    This file contains explicit template instantiations and XmlRegistry registrations
    for all function XML serialization types. This ensures proper instantiation order
    and avoids circular dependencies.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris, Architecture Refactoring Team
*/

#include <gismo/Common/TemplateTools.h>

// Include all function XML headers
#include <gismo/IO/XmlFunctionExpr.h>
#include <gismo/IO/XmlConstantFunction.h>
#include <gismo/IO/XmlComposedFunction.h>
#include <gismo/IO/XmlFunction.h>

#define T real_t

namespace gismo {
namespace internal {

// =============================================================================
// Explicit Template Instantiations for gsXml Function Classes
// =============================================================================

TEMPLATE_INST class gsXml< gsFunction<T> >;
TEMPLATE_INST class gsXml< gsFunctionSet<T> >;
TEMPLATE_INST class gsXml< gsFunctionExpr<T> >;
TEMPLATE_INST class gsXml< gsConstantFunction<T> >;
TEMPLATE_INST class gsXml< gsComposedFunction<T> >;

// =============================================================================
// Explicit Template Instantiations for XML Serialization Classes
// =============================================================================

TEMPLATE_INST class XmlFunction<T>;
TEMPLATE_INST class XmlFunctionExpr<T>;
TEMPLATE_INST class XmlConstantFunction<T>;
TEMPLATE_INST class XmlComposedFunction<T>;

} // namespace internal
} // namespace gismo

// =============================================================================
// XmlRegistry Registration (at static initialization time)
// =============================================================================

namespace gismo {
namespace internal {

// Register all function XML serialization types for real_t
// This ensures XmlRegistry has all types available for polymorphic dispatch
// Registration happens at static initialization time, before any XML operations
namespace {
    struct FunctionXmlRegistrations {
        FunctionXmlRegistrations() {
            // Register concrete function types first (needed for polymorphic dispatch)
            XmlFunctionExpr<real_t>::registerType();
            XmlConstantFunction<real_t>::registerType();
            XmlComposedFunction<real_t>::registerType();

            // Register base class dispatcher last
            XmlFunction<real_t>::registerType();
        }
    };
    static FunctionXmlRegistrations function_registrations;
}

} // namespace internal
} // namespace gismo

#undef T