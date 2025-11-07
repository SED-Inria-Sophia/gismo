/** @file XmlFunctionExpr.h

    @brief XML specialization for gsFunctionExpr using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsFunctionExpr
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
#include <gismo/Core/SpecializedFunction/FunctionExpr.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsFunctionExpr
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsFunctionExpr objects and handles XmlRegistry registration.
 */
template<class T>
class XmlFunctionExpr
{
public:
    /// XML tag name
    static std::string tag() { return "Function"; }

    /// XML type attribute value
    static std::string type() { return "FunctionExpr"; }

    /// Deserialize gsFunctionExpr<T> from XML
    static gsFunctionExpr<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlFunctionExpr.hpp
        GISMO_ASSERT( ( !strcmp( node->name(),"Function") )
                    &&  ( !strcmp(node->first_attribute("type")->value(),
                                "FunctionExpr" ) ),
                    "Reading gsFunctionExpr XML: No Function found" );

        GISMO_ASSERT( node->first_attribute("dim"), "Reading gsFunctionExpr XML: No dim found" ) ;
        const int d = atoi( node->first_attribute("dim")->value() );

        std::vector< std::string > expr_strings;

        gsXmlNode * child = node->first_node("c");

        if (child != NULL )
        {
            for (; child; child = child->next_sibling() )
                expr_strings.push_back(  child->value() );
        }
        else
            expr_strings.push_back(  node->value() );

        return new gsFunctionExpr<T>(expr_strings, d);
    }

    /// Serialize gsFunctionExpr<T> to XML
    static gsXmlNode* put(const gsFunctionExpr<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlFunctionExpr.hpp
        // Add a new node
        gsXmlNode* node = makeNode("Function" , data);
        node->append_attribute( makeAttribute("type", "FunctionExpr", data) );
        node->append_attribute(makeAttribute("dim", obj.domainDim(), data));

        const short_t tdim = obj.targetDim();

        if ( tdim == 1)
        {
            node->value( makeValue(obj.expression(), data) );
        }
        else
        {
            gsXmlNode * cnode;
            for (short_t c = 0; c!=tdim; ++c)
            {
                cnode = makeNode("c", obj.expression(c), data);
                node->append_node(cnode);
            }
        }

        return node;
    }

    /// Register with XmlRegistry
    static void registerType() {
        XmlRegistry::registerType<gsFunctionExpr<T>>(
            type(),
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsFunctionExpr with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsFunctionExpr<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsFunctionExpr<T> > {
private:
    gsXml() { }
    typedef gsFunctionExpr<T> Object;

    // Note: Registration handled in XmlFunction_.cpp to ensure proper initialization order

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);

    /// XML tag name
    static std::string tag() { return XmlFunctionExpr<T>::tag(); }

    /// XML type attribute value
    static std::string type() { return XmlFunctionExpr<T>::type(); }

    /** @brief Deserialize gsFunctionExpr from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the function data
     * @return Pointer to new gsFunctionExpr object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlFunction_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsFunctionExpr to XML (dispatches to XmlRegistry)
     *
     * @param obj gsFunctionExpr object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the function data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlFunction_.cpp
        return XmlRegistry::put(obj, data);
    }
};

// Note: Automatic registration happens via static constructors in anonymous namespace above

} // namespace internal
} // namespace gismo