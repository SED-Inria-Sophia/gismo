/** @file XmlBasis_.cpp

    @brief Explicit instantiations and registrations for XML basis serialization

    This file contains explicit template instantiations and XmlRegistry registrations
    for all basis XML serialization types. This ensures proper instantiation order
    and avoids circular dependencies.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris, Architecture Refactoring Team
*/

#include <gismo/Common/TemplateTools.h>

// Include all basis XML headers
#include <gismo/IO/XmlComposedBasis.h>
#include <gismo/IO/XmlBasis.h>

#define T real_t

namespace gismo {
namespace internal {

// =============================================================================
// Explicit Template Instantiations for gsXml Basis Classes
// =============================================================================

TEMPLATE_INST class gsXml< gsBasis<T> >;
TEMPLATE_INST class gsXml< gsComposedBasis<T> >;


// =============================================================================
// Explicit Template Instantiations for XML Serialization Classes
// =============================================================================

TEMPLATE_INST class XmlBasis<T>;
TEMPLATE_INST class XmlComposedBasis<T>;

} // namespace internal
} // namespace gismo

// =============================================================================
// XmlRegistry Registration (at static initialization time)
// =============================================================================

namespace gismo {
namespace internal {

// Register all basis XML serialization types for real_t
// This ensures XmlRegistry has all types available for polymorphic dispatch
// Registration happens at static initialization time, before any XML operations
namespace {
    struct BasisXmlRegistrations {
        BasisXmlRegistrations() {
            // Register concrete basis types first (needed for polymorphic dispatch)
            XmlComposedBasis<real_t>::registerType();

            // Register base class dispatcher last
            XmlBasis<real_t>::registerType();
        }
    };
    static BasisXmlRegistrations basis_registrations;
}

} // namespace internal
} // namespace gismo

#undef T