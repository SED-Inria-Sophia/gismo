/** @file ConstantFunction.xml.hpp

    @brief Provides XML serialization for gsConstantFunction.

    This file contains the XML serialization specialization for gsConstantFunction.
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
#include <gismo/Core/SpecializedFunction/ConstantFunction.hpp>

// XML serialization dependencies
#include <gsIO/gsXml.h>
#include <gsIO/gsXmlGenericUtils.hpp>

namespace gismo {

namespace internal
{

/// @brief Get a gsConstantFunction from XML data
template<class T>
class gsXml< gsConstantFunction<T> >
{
private:
    gsXml() { }
    typedef gsConstantFunction<T> Object;
public:
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);
    static std::string tag () { return "Function"; }
    static std::string type () { return "ConstantFunction"; }

    static Object * get (gsXmlNode * node)
    {
        GISMO_ASSERT( ( !strcmp( node->name(),"Function") )
                    &&  ( !strcmp(node->first_attribute("type")->value(),
                                internal::gsXml<Object>::type().c_str() ) ),
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

        return new Object(val, d);
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
};

} // namespace internal

} // namespace gismo