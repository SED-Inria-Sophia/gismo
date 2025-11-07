/** @file XmlCurve.h

    @brief XML specialization for gsCurve using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsCurve
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
#include <gismo/Core/Geometry/Curve.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsCurve (abstract base)
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsCurve objects and handles XmlRegistry registration for the base dispatcher.
 */
template<class T>
class XmlCurve
{
public:
    /// XML tag name
    static std::string tag() { return "Geometry"; }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return ""; }

    /// Deserialize gsCurve<T> from XML (polymorphic dispatcher)
    static gsCurve<T>* get(gsXmlNode* node)
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
        return XmlRegistry::getByTypeAttribute<gsCurve<T>>(node, s);
    }

    /// Serialize gsCurve<T> to XML (polymorphic dispatcher)
    static gsXmlNode* put(const gsCurve<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlUtils.hpp using XmlRegistry dispatch
        // The original implementation used dynamic_cast to check concrete types
        // We replace this with XmlRegistry polymorphic dispatch
        return XmlRegistry::putPolymorphic(obj, data);
    }

    /// Register with XmlRegistry
    static void registerType() {
        // Note: gsCurve<T> itself doesn't need direct registration since it's abstract
        // The actual registration happens for concrete derived types:
        // - gsBSpline<T>, gsNurbs<T>, gsHBSpline<>, etc.

        // However, we register the dispatcher for consistency with the pattern
        XmlRegistry::registerType<gsCurve<T>>(
            type(), // Empty for abstract base
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsCurve with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsCurve<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsCurve<T> > {
private:
    gsXml() { }
    typedef gsCurve<T> Object;

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);

    /// XML tag name
    static std::string tag() { return XmlCurve<T>::tag(); }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return XmlCurve<T>::type(); }

    /** @brief Deserialize gsCurve from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the curve data
     * @return Pointer to new gsCurve object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlGeometry_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsCurve to XML (dispatches to XmlRegistry)
     *
     * @param obj gsCurve object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the curve data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlGeometry_.cpp
        return XmlRegistry::put(obj, data);
    }
};

} // namespace internal
} // namespace gismo