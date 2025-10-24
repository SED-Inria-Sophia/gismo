/** @file XmlFunctionExpr.hpp

    @brief XML specialization for FunctionExpr class

    This file contains the XML serialization specialization for gsFunctionExpr.
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
#include <gismo/Core/SpecializedFunction/FunctionExpr.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlGenericUtils.hpp>

namespace gismo {

namespace internal
{

/// @brief Get a FunctionsExpr from XML data
template<class T>
class gsXml< gsFunctionExpr<T> >
{
private:
    gsXml() { }
    typedef gsFunctionExpr<T> Object;
public:
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);
    static std::string tag ()  { return "Function"; }
    static std::string type () { return "FunctionExpr"; }

    static Object * get(gsXmlNode * node)
    {
        GISMO_ASSERT( ( !strcmp( node->name(),"Function") )
                    &&  ( !strcmp(node->first_attribute("type")->value(),
                                internal::gsXml<Object>::type().c_str() ) ),
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

        return new Object(expr_strings, d);
    }

    static gsXmlNode * put (const Object & obj,
                            gsXmlTree & data )
    {
        // Add a new node
        gsXmlNode* node = internal::makeNode("Function" , data);
        node->append_attribute( makeAttribute("type",
                                            internal::gsXml< Object >::type().c_str(), data) );
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
};

} // namespace internal

} // namespace gismo