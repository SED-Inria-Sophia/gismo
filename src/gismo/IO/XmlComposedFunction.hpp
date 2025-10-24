/** @file XmlComposedFunction.hpp

    @brief XML specialization for ComposedFunction class

    This file contains the XML serialization specialization for gsComposedFunction.
    It is separated from the main class definition to break circular dependencies
    between Core and gsIO modules.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

// Include the base class first
#include <gismo/Core/SpecializedFunction/ComposedFunction.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlGenericUtils.hpp>

namespace gismo {

namespace internal
{

/// @brief Get a gsComposedFunction from XML data
template<class T>
class gsXml< gsComposedFunction<T> >
{
private:
    gsXml() { }
    typedef gsComposedFunction<T> Object;
public:
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);
    static std::string tag () { return "Function"; }
    static std::string type () { return "ComposedFunction"; }

    static Object * get (gsXmlNode * node)
    {
        GISMO_ASSERT( ( !strcmp( node->name(),"Function") )
                    &&  ( !strcmp(node->first_attribute("type")->value(),
                                internal::gsXml<Object>::type().c_str() ) ),
                    "Something is wrong with the XML data: There should be a node with a "<<
                    internal::gsXml<Object>::type().c_str()<<" Function.");

        typedef typename Object::CompositionT CompositionType;
        typedef typename Object::FunctionT    FunctionType;

        // The XML node will have two parts: a composition (gsGeometry) and a function (gsFunction)
        // 1. Get the composition
        gsXmlNode* compNode = node->first_node("Composition");
        GISMO_ASSERT(compNode, "gsXmlUtils: get ComposedFunction: No composition found.");
        CompositionType * composition;
        if      (gsXmlNode* compData = compNode->first_node("Geometry"))
            composition = gsXml< gsGeometry<T> >::get (compData) ;
        else if (gsXmlNode* compData2 = compNode->first_node("Function"))
            composition = gsXml< gsFunction<T> >::get (compData2) ;
        else
            GISMO_ERROR("gsXmlUtils: get ComposedFunction: No composition found.");

        // 2. Get the function
        gsXmlNode* functionNode = node->first_node("Function");
        GISMO_ASSERT(functionNode, "gsXmlUtils: get ComposedFunction: No function found.");
        gsXmlNode* functionData = functionNode->first_node("Function");
        GISMO_ASSERT(functionData, "gsXmlUtils: get ComposedFunction: No function data found.");
        FunctionType * function = gsXml<FunctionType >::get (functionData) ;
        return new Object(memory::make_shared(composition), memory::make_shared(function));
    }

    static gsXmlNode * put (const Object & obj,
                            gsXmlTree & data )
    {
        typedef typename Object::CompositionT CompositionType;
        typedef typename Object::FunctionT    FunctionType;

        // Add a new node
        gsXmlNode* node = internal::makeNode("Function" , data);
        node->append_attribute( makeAttribute("type",
                                            internal::gsXml< Object >::type().c_str(), data) );

        // The XML node will have two parts: a composition (gsGeometry/gsFunction) and a function (gsFunction)
        // 1. Write the composition
        gsXmlNode* compNode = internal::makeNode("Composition",data);
        gsXmlNode* compData;
        if      (const gsGeometry<T> * geo = dynamic_cast<const gsGeometry<T> *>( &obj.composition() ))
            compData = internal::gsXml< gsGeometry<T> >::put(*geo, data);
        else if (const gsFunction<T> * fun = dynamic_cast<const gsFunction<T> *>( &obj.composition() ))
            compData = internal::gsXml< gsFunction<T> >::put(*fun, data);
        else
            GISMO_ERROR("gsXmlUtils: put gsComposedFunction: No known composition found.");
        compNode->append_node(compData);
        node->append_node(compNode);

        // 2. Write the function
        gsXmlNode* functionNode = internal::makeNode("Function",data);
        gsXmlNode* functionData = internal::gsXml< FunctionType >::put(obj.function(), data);
        functionNode->append_node(functionData);
        node->append_node(functionNode);

        return node;
    }
};

} // namespace internal

} // namespace gismo