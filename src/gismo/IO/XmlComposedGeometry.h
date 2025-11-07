/** @file XmlComposedGeometry.h

    @brief XML specialization for gsComposedGeometry using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsComposedGeometry
    that dispatches to the XmlRegistry system. This provides backward compatibility
    with the existing gsXml<T> interface while using the modern registry system.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris, Architecture Refactoring Team
*/

#pragma once

// Include the base class first
#include <gismo/Core/Geometry/ComposedGeometry.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>
#include <gismo/IO/XmlGeometry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsComposedGeometry
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsComposedGeometry objects and handles XmlRegistry registration.
 */
template<class T>
class XmlComposedGeometry
{
public:
    /// XML tag name
    static std::string tag() { return "Geometry"; }

    /// XML type attribute value
    static std::string type() { return "ComposedGeometry"; }

    /// Deserialize gsComposedGeometry<T> from XML
    static gsComposedGeometry<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlComposedGeometry.hpp
        return getGeometryFromXml< gsComposedGeometry<T> >( node );
    }

    /// Serialize gsComposedGeometry<T> to XML
    static gsXmlNode* put(const gsComposedGeometry<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlComposedGeometry.hpp
        return putGeometryToXml(obj, data);
    }

    /// Register with XmlRegistry
    static void registerType() {
        XmlRegistry::registerType<gsComposedGeometry<T>>(
            type(),
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsComposedGeometry with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsComposedGeometry<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsComposedGeometry<T> > {
private:
    gsXml() { }
    typedef gsComposedGeometry<T> Object;

    // Note: Registration handled in XmlGeometry_.cpp to ensure proper initialization order

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);

    /// XML tag name
    static std::string tag() { return XmlComposedGeometry<T>::tag(); }

    /// XML type attribute value
    static std::string type() { return XmlComposedGeometry<T>::type(); }

    /** @brief Deserialize gsComposedGeometry from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the geometry data
     * @return Pointer to new gsComposedGeometry object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlGeometry_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsComposedGeometry to XML (dispatches to XmlRegistry)
     *
     * @param obj gsComposedGeometry object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the geometry data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlGeometry_.cpp
        return XmlRegistry::put(obj, data);
    }
};

} // namespace internal
} // namespace gismo