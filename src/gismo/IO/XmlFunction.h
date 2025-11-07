/** @file XmlFunction.h

    @brief XML specialization for gsFunction using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsFunction
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
#include <gismo/Core/Function/Function.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsFunction (abstract base)
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsFunction objects and handles XmlRegistry registration for the base dispatcher.
 */
template<class T>
class XmlFunction
{
public:
    /// XML tag name
    static std::string tag() { return "Function"; }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return ""; }

    /// Deserialize gsFunction<T> from XML (polymorphic dispatcher)
    static gsFunction<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlFunction.hpp
        GISMO_ASSERT( ( !strcmp( node->name(),"Function") ),
                      "Something went wrong, was waiting for a Function or Geometry tag.\n" );

        gsXmlAttribute * ftype = node->first_attribute("type");
        if ( ! ftype )
        {
            gsWarn<< "Object without a type in the xml file\n";
            return NULL;
        }

        std::string s = ftype->value();

        // Dispatch to XmlRegistry based on type attribute
        return XmlRegistry::getByTypeAttribute<gsFunction<T>>(node, s);
    }

    /// Serialize gsFunction<T> to XML (polymorphic dispatcher)
    static gsXmlNode* put(const gsFunction<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlFunction.hpp using XmlRegistry dispatch
        // Try to dispatch to XmlRegistry using dynamic type information
        return XmlRegistry::putPolymorphic(obj, data);
    }

    /// Register with XmlRegistry
    static void registerType() {
        // Note: gsFunction<T> itself doesn't need direct registration since it's abstract
        // The actual registration happens for concrete derived types:
        // - gsConstantFunction<T> (via XmlConstantFunction.h)
        // - gsFunctionExpr<T> (via XmlFunctionExpr.h)
        // - gsComposedFunction<T> (via XmlComposedFunction.h)

        // However, we register the dispatcher for consistency with the pattern
        XmlRegistry::registerType<gsFunction<T>>(
            type(), // Empty for abstract base
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsFunction with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsFunction<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsFunction<T> > {
private:
    gsXml() { }
    typedef gsFunction<T> Object;

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);

    /// XML tag name
    static std::string tag() { return XmlFunction<T>::tag(); }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return XmlFunction<T>::type(); }

    /** @brief Deserialize gsFunction from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the function data
     * @return Pointer to new gsFunction object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlFunction_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsFunction to XML (dispatches to XmlRegistry)
     *
     * @param obj gsFunction object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the function data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlFunction_.cpp
        return XmlRegistry::put(obj, data);
    }
};

// /////////////////////////////////////////////////////////////////////////////
// Helper functions for XML serialization of gsFunction
// /////////////////////////////////////////////////////////////////////////////

template < class T >
gsXmlNode * putFunctionToXml ( const gsFunction<T> & obj, gsXmlTree & data, int index)
{
    gsXmlNode * result = internal::gsXml< gsFunction<T> >::put(obj, data);
    gsXmlAttribute *indexNode = internal::makeAttribute("index", index, data);
    result->append_attribute(indexNode);
    return result;
}

} // namespace internal
} // namespace gismo