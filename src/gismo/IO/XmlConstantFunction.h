/** @file XmlConstantFunction.h

    @brief XML specialization for gsConstantFunction using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsConstantFunction
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
#include <gismo/Core/SpecializedFunction/ConstantFunction.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsConstantFunction
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsConstantFunction objects and handles XmlRegistry registration.
 */
template<class T>
class XmlConstantFunction
{
public:
    /// XML tag name
    static std::string tag() { return "Function"; }

    /// XML type attribute value
    static std::string type() { return "ConstantFunction"; }

    /// Deserialize gsConstantFunction<T> from XML
    static gsConstantFunction<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlConstantFunction.hpp
        GISMO_ASSERT( ( !strcmp( node->name(),"Function") )
                    &&  ( !strcmp(node->first_attribute("type")->value(),
                                "ConstantFunction" ) ),
                    "Reading gsConstantFunction XML: No Function found" );

        GISMO_ASSERT( node->first_attribute("dim"), "Reading gsConstantFunction XML: No dim found" ) ;
        const int d = atoi( node->first_attribute("dim")->value() );

        gsVector<T> val;
        gsXmlNode * child = node->first_node("c");
        if (child != NULL )
        {
            val.resize(countByTag("c",node));
            index_t idx = 0;
            std::istringstream str;
            str.str(child->value());
            for (; child; child = child->next_sibling(), idx++ )
                gsGetValue(str, val.at(idx));
        }
        else
        {
            val.resize(1);
            std::istringstream str;
            str.str(node->value());
            gsGetValue(str, val.at(0));
        }

        return new gsConstantFunction<T>(val, d);
    }

    /// Serialize gsConstantFunction<T> to XML
    static gsXmlNode* put(const gsConstantFunction<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlConstantFunction.hpp
        // Add a new node
        gsXmlNode* node = makeNode("Function" , data);
        node->append_attribute( makeAttribute("type", "ConstantFunction", data) );
        node->append_attribute(makeAttribute("dim", obj.domainDim(), data));

        const short_t tdim = obj.targetDim();

        if ( tdim == 1)
        {
            node->value( makeValue(std::to_string(obj.value(0)), data) );
        }
        else
        {
            gsXmlNode * cnode;
            for (short_t c = 0; c!=tdim; ++c)
            {
                cnode = makeNode("c", std::to_string(obj.value(c)), data);
                node->append_node(cnode);
            }
        }

        return node;
    }

    /// Register with XmlRegistry
    static void registerType() {
        XmlRegistry::registerType<gsConstantFunction<T>>(
            type(),
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsConstantFunction with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsConstantFunction<T>> interface
 * while dispatching to the XmlConstantFunction class methods.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsConstantFunction<T> > {
private:
    gsXml() { }
    typedef gsConstantFunction<T> Object;

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);

    /// XML tag name
    static std::string tag() { return XmlConstantFunction<T>::tag(); }

    /// XML type attribute value
    static std::string type() { return XmlConstantFunction<T>::type(); }

    /** @brief Deserialize gsConstantFunction from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the function data
     * @return Pointer to new gsConstantFunction object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlFunction_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsConstantFunction to XML (dispatches to XmlRegistry)
     *
     * @param obj gsConstantFunction object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the function data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlFunction_.cpp
        return XmlRegistry::put(obj, data);
    }
};

} // namespace internal
} // namespace gismo