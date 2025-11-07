/** @file XmlSurface.h

    @brief XML specialization for gsSurface using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsSurface
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
#include <gismo/Core/Geometry/Surface.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsSurface (abstract base)
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsSurface objects and handles XmlRegistry registration for the base dispatcher.
 */
template<class T>
class XmlSurface
{
public:
    /// XML tag name
    static std::string tag() { return "Geometry"; }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return ""; }

    /// Deserialize gsSurface<T> from XML (polymorphic dispatcher)
    static gsSurface<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlUtils.hpp
        GISMO_ASSERT( ( !strcmp( node->name(),"Geometry") ),
                      "Something went wrong, was waiting for a Geometry tag.\n" );

        gsXmlAttribute * gtype = node->first_attribute("type");
        if ( ! gtype )
        {
            gsWarn<< "Geometry without a type in the xml file\n";
            return NULL;
        }

        std::string s = gtype->value();

        // Dispatch to XmlRegistry based on type attribute
        return XmlRegistry::getByTypeAttribute<gsSurface<T>>(node, s);
    }

    /// Serialize gsSurface<T> to XML (polymorphic dispatcher)
    static gsXmlNode* put(const gsSurface<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlUtils.hpp using XmlRegistry dispatch
        // The original implementation used dynamic_cast to check concrete types
        // We replace this with XmlRegistry polymorphic dispatch
        return XmlRegistry::putPolymorphic(obj, data);
    }

    /// Register with XmlRegistry
    static void registerType() {
        // Note: gsSurface<T> itself doesn't need direct registration since it's abstract
        // The actual registration happens for concrete derived types:
        // - gsTHBSpline<2,T>, gsHBSpline<2,T>, gsTensorBSpline<2,T>, gsTensorNurbs<2,T>, etc.

        // However, we register the dispatcher for consistency with the pattern
        XmlRegistry::registerType<gsSurface<T>>(
            type(), // Empty for abstract base
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsSurface with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsSurface<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsSurface<T> > {
private:
    gsXml() { }
    typedef gsSurface<T> Object;

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);

    /// XML tag name
    static std::string tag() { return XmlSurface<T>::tag(); }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return XmlSurface<T>::type(); }

    /** @brief Deserialize gsSurface from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the surface data
     * @return Pointer to new gsSurface object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlGeometry_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsSurface to XML (dispatches to XmlRegistry)
     *
     * @param obj gsSurface object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the surface data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlGeometry_.cpp
        return XmlRegistry::put(obj, data);
    }
};

} // namespace internal
} // namespace gismo