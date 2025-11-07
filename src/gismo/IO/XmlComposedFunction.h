/** @file XmlComposedFunction.h

    @brief XML specialization for gsComposedFunction using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsComposedFunction
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
#include <gismo/Core/SpecializedFunction/ComposedFunction.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsComposedFunction
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsComposedFunction objects and handles XmlRegistry registration.
 */
template<class T>
class XmlComposedFunction
{
public:
    /// XML tag name
    static std::string tag() { return "Function"; }

    /// XML type attribute value
    static std::string type() { return "ComposedFunction"; }

    /// Deserialize gsComposedFunction<T> from XML
    static gsComposedFunction<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlComposedFunction.hpp
        GISMO_ASSERT( ( !strcmp( node->name(),"Function") )
                    &&  ( !strcmp(node->first_attribute("type")->value(),
                                "ComposedFunction" ) ),
                    "Something is wrong with the XML data: There should be a node with a "<<
                    "ComposedFunction Function.");

        typedef typename gsComposedFunction<T>::CompositionT CompositionType;
        typedef typename gsComposedFunction<T>::FunctionT    FunctionType;

        // The XML node will have two parts: a composition (gsGeometry) and a function (gsFunction)
        // 1. Get the composition
        gsXmlNode* compNode = node->first_node("Composition");
        GISMO_ASSERT(compNode, "gsXmlUtils: get ComposedFunction: No composition found.");
        CompositionType * composition;
        if      (gsXmlNode* compData = compNode->first_node("Geometry"))
        {
            // Use non-typed XmlRegistry method to avoid including gsGeometry<T>
            gsXmlAttribute* gtype = compData->first_attribute("type");
            GISMO_ASSERT(gtype, "gsXmlUtils: get ComposedFunction: Geometry node missing type attribute.");
            std::string xmlType = gtype->value();
            composition = static_cast<CompositionType*>(
                XmlRegistry::getByXmlType(xmlType, compData)
            );
        }
        else if (gsXmlNode* compData2 = compNode->first_node("Function"))
        {
            // Use non-typed XmlRegistry method to avoid including gsFunction<T>
            gsXmlAttribute* ftype = compData2->first_attribute("type");
            GISMO_ASSERT(ftype, "gsXmlUtils: get ComposedFunction: Function node missing type attribute.");
            std::string xmlType = ftype->value();
            composition = static_cast<CompositionType*>(
                XmlRegistry::getByXmlType(xmlType, compData2)
            );
        }
        else
            GISMO_ERROR("gsXmlUtils: get ComposedFunction: No composition found.");

        // 2. Get the function
        gsXmlNode* functionNode = node->first_node("Function");
        GISMO_ASSERT(functionNode, "gsXmlUtils: get ComposedFunction: No function found.");
        gsXmlNode* functionData = functionNode->first_node("Function");
        GISMO_ASSERT(functionData, "gsXmlUtils: get ComposedFunction: No function data found.");

        // Use non-typed XmlRegistry method to avoid including FunctionType
        gsXmlAttribute* ftype = functionData->first_attribute("type");
        GISMO_ASSERT(ftype, "gsXmlUtils: get ComposedFunction: Function data missing type attribute.");
        std::string xmlType = ftype->value();
        FunctionType * function = static_cast<FunctionType*>(
            XmlRegistry::getByXmlType(xmlType, functionData)
        );

        return new gsComposedFunction<T>(memory::make_shared(composition), memory::make_shared(function));
    }

    /// Serialize gsComposedFunction<T> to XML
    static gsXmlNode* put(const gsComposedFunction<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlComposedFunction.hpp
        typedef typename gsComposedFunction<T>::FunctionT    FunctionType;

        // Add a new node
        gsXmlNode* node = makeNode("Function" , data);
        node->append_attribute( makeAttribute("type", "ComposedFunction", data) );

        // The XML node will have two parts: a composition (gsGeometry/gsFunction) and a function (gsFunction)
        // 1. Write the composition
        gsXmlNode* compNode = makeNode("Composition",data);
        gsXmlNode* compData;
        if      (const gsGeometry<T> * geo = dynamic_cast<const gsGeometry<T> *>( &obj.composition() ))
            compData = XmlRegistry::putPolymorphic(*geo, data);
        else if (const gsFunction<T> * fun = dynamic_cast<const gsFunction<T> *>( &obj.composition() ))
            compData = XmlRegistry::putPolymorphic(*fun, data);
        else
            GISMO_ERROR("gsXmlUtils: put gsComposedFunction: No known composition found.");
        compNode->append_node(compData);
        node->append_node(compNode);

        // 2. Write the function - use explicit FunctionType to match original behavior
        gsXmlNode* functionNode = makeNode("Function",data);
        gsXmlNode* functionData = XmlRegistry::put<FunctionType>(obj.function(), data);
        functionNode->append_node(functionData);
        node->append_node(functionNode);

        return node;
    }

    /// Register with XmlRegistry
    static void registerType() {
        XmlRegistry::registerType<gsComposedFunction<T>>(
            type(),
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsComposedFunction with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsComposedFunction<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsComposedFunction<T> > {
private:
    gsXml() { }
    typedef gsComposedFunction<T> Object;

    // Note: Registration handled in XmlFunction_.cpp to ensure proper initialization order

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);

    /// XML tag name
    static std::string tag() { return XmlComposedFunction<T>::tag(); }

    /// XML type attribute value
    static std::string type() { return XmlComposedFunction<T>::type(); }

    /** @brief Deserialize gsComposedFunction from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the function data
     * @return Pointer to new gsComposedFunction object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlFunction_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsComposedFunction to XML (dispatches to XmlRegistry)
     *
     * @param obj gsComposedFunction object to serialize
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