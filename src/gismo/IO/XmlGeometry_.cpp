/** @file XmlGeometry_.cpp

    @brief Explicit instantiations and registrations for XML geometry serialization

    This file contains explicit template instantiations and XmlRegistry registrations
    for all geometry XML serialization types. This ensures proper instantiation order
    and avoids circular dependencies.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris, Architecture Refactoring Team
*/

#include <gismo/Common/TemplateTools.h>

// Include all geometry XML headers
#include <gismo/IO/XmlGeometry.h>
#include <gismo/IO/XmlComposedGeometry.h>
#include <gismo/IO/XmlCurve.h>
#include <gismo/IO/XmlSurface.h>

#define T real_t

namespace gismo {
namespace internal {

// =============================================================================
// Explicit Template Instantiations for gsXml Geometry Classes
// =============================================================================

TEMPLATE_INST class gsXml< gsGeometry<T> >;
TEMPLATE_INST class gsXml< gsComposedGeometry<T> >;
TEMPLATE_INST class gsXml< gsCurve<T> >;
TEMPLATE_INST class gsXml< gsSurface<T> >;

// =============================================================================
// Explicit Template Instantiations for XML Serialization Classes
// =============================================================================

TEMPLATE_INST class XmlGeometry<T>;
TEMPLATE_INST class XmlComposedGeometry<T>;
TEMPLATE_INST class XmlCurve<T>;
TEMPLATE_INST class XmlSurface<T>;

} // namespace internal
} // namespace gismo

// =============================================================================
// XmlRegistry Registration (at static initialization time)
// =============================================================================

namespace gismo {
namespace internal {

// Register all geometry XML serialization types for real_t
// This ensures XmlRegistry has all types available for polymorphic dispatch
// Registration happens at static initialization time, before any XML operations
namespace {
    struct GeometryXmlRegistrations {
        GeometryXmlRegistrations() {
            // Register concrete geometry types first (needed for polymorphic dispatch)
            XmlComposedGeometry<real_t>::registerType();

            // Register abstract base classes for polymorphic dispatch
            XmlCurve<real_t>::registerType();
            XmlSurface<real_t>::registerType();

            // Register base class dispatcher last
            XmlGeometry<real_t>::registerType();
        }
    };
    static GeometryXmlRegistrations geometry_registrations;
}

} // namespace internal
} // namespace gismo

#undef T