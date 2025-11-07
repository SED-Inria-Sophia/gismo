/** @file XmlBasis.h

    @brief XML specialization for gsBasis using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsBasis
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
#include <gismo/Core/Basis/Basis.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsBasis (abstract base)
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsBasis objects and handles XmlRegistry registration for the base dispatcher.
 */
template<class T>
class XmlBasis
{
public:
    /// XML tag name
    static std::string tag() { return "Basis"; }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return ""; }

    /// Deserialize gsBasis<T> from XML (polymorphic dispatcher)
    static gsBasis<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlBasis.hpp
        GISMO_ASSERT( ( !strcmp( node->name(),"Basis") ),
                      "Something went wrong, waiting for a basis." );

        gsXmlAttribute * btype = node->first_attribute("type");
        if ( ! btype )
        {
            gsWarn<< "Basis without a type in the xml file.\n";
            return NULL;
        }

        std::string s = btype->value();

        // Dispatch to XmlRegistry based on type attribute
        return XmlRegistry::getByTypeAttribute<gsBasis<T>>(node, s);
    }

    /// Serialize gsBasis<T> to XML (polymorphic dispatcher)
    static gsXmlNode* put(const gsBasis<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlBasis.hpp using XmlRegistry dispatch
        // Try to dispatch to XmlRegistry using dynamic type information
        return XmlRegistry::putPolymorphic(obj, data);
    }

    /// Register with XmlRegistry
    static void registerType() {
        // Note: gsBasis<T> itself doesn't need direct registration since it's abstract
        // The actual registration happens for concrete derived types:
        // - gsComposedBasis<T> (via XmlComposedBasis.h)
        // - gsTHBSplineBasis<dim,T> (via XmlTHBSplineBasis.h)
        // - gsTensorBSplineBasis<dim,T> (via XmlTensorBSplineBasis.h)

        // However, we register the dispatcher for consistency with the pattern
        XmlRegistry::registerType<gsBasis<T>>(
            type(), // Empty for abstract base
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsBasis with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsBasis<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsBasis<T> > {
private:
    gsXml() { }
    typedef gsBasis<T> Object;

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);

    /// XML tag name
    static std::string tag() { return XmlBasis<T>::tag(); }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return XmlBasis<T>::type(); }

    /** @brief Deserialize gsBasis from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the basis data
     * @return Pointer to new gsBasis object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlBasis_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsBasis to XML (dispatches to XmlRegistry)
     *
     * @param obj gsBasis object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the basis data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlBasis_.cpp
        return XmlRegistry::put(obj, data);
    }
};

} // namespace internal
} // namespace gismo