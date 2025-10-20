/** @file ComposedBasis.xml.hpp

    @brief Provides XML serialization for gsComposedBasis.

    This file contains the XML serialization specialization for gsComposedBasis.
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
#include <gismo/Core/Basis/ComposedBasis.hpp>

// XML serialization dependencies
#include <gsIO/gsXml.h>
#include <gsIO/gsXmlGenericUtils.hpp>

namespace gismo {

namespace internal
{

/// @brief Get a gsComposedBasis from XML data
template<class T>
class gsXml< gsComposedBasis<T> >
{
private:
    gsXml() { }
    typedef gsComposedBasis<T> Object;
public:
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);
    static std::string tag () { return "Basis"; }
    static std::string type () { return "ComposedBasis"; }

    static Object * get (gsXmlNode * node)
    {
        GISMO_ASSERT( ( !strcmp( node->name(),"Basis") )
                    &&  ( !strcmp(node->first_attribute("type")->value(),
                                internal::gsXml<Object>::type().c_str() ) ),
                    "Something is wrong with the XML data: There should be a node with a "<<
                    internal::gsXml<Object>::type().c_str()<<" Basis.");

        typedef typename Object::CompositionT CompositionType;
        typedef typename Object::BasisT       BasisType;

        // The XML node will have two parts: a composition (gsGeometry) and a basis (gsBasis)
        // 1. Get the composition
        gsXmlNode* compNode = node->first_node("Composition");
        GISMO_ASSERT(compNode, "gsXmlUtils: get ComposedBasis: No composition found.");
        CompositionType * composition;
        if      (gsXmlNode* compData = compNode->first_node("Geometry"))
            composition = gsXml< gsGeometry<T> >::get (compData) ;
        else if (gsXmlNode* compData2 = compNode->first_node("Function"))
            composition = gsXml< gsFunction<T> >::get (compData2) ;
        else
            GISMO_ERROR("gsXmlUtils: get ComposedBasis: No composition found.");

        // 2. Get the basis
        gsXmlNode* basisNode = node->first_node("Basis");
        GISMO_ASSERT(basisNode, "gsXmlUtils: get ComposedBasis: No basis found.");
        gsXmlNode* basisData = basisNode->first_node("Basis");
        GISMO_ASSERT(basisData, "gsXmlUtils: get ComposedBasis: No basis data found.");
        BasisType * basis = gsXml<BasisType >::get (basisData) ;
        return new Object(memory::make_shared(composition), memory::make_shared(basis));
    }

    static gsXmlNode * put (const Object & obj,
                            gsXmlTree & data )
    {
        typedef typename Object::CompositionT CompositionType;
        typedef typename Object::BasisT       BasisType;

        // Add a new node
        gsXmlNode* node = internal::makeNode("Basis" , data);
        node->append_attribute( makeAttribute("type",
                                            internal::gsXml< Object >::type().c_str(), data) );

        // The XML node will have two parts: a composition (gsGeometry/gsFunction) and a basis (gsBasis)
        // 1. Write the composition
        gsXmlNode* compNode = internal::makeNode("Composition",data);
        gsXmlNode* compData;
        if      (const gsGeometry<T> * geo = dynamic_cast<const gsGeometry<T> *>( &obj.composition() ))
            compData = internal::gsXml< gsGeometry<T> >::put(*geo, data);
        else if (const gsFunction<T> * fun = dynamic_cast<const gsFunction<T> *>( &obj.composition() ))
            compData = internal::gsXml< gsFunction<T> >::put(*fun, data);
        else
            GISMO_ERROR("gsXmlUtils: put ComposedBasis: No known composition found.");
        compNode->append_node(compData);
        node->append_node(compNode);

        // 2. Write the basis
        gsXmlNode* basisNode = internal::makeNode("Basis",data);
        gsXmlNode* basisData = internal::gsXml< BasisType >::put(obj.basis(), data);
        basisNode->append_node(basisData);
        node->append_node(basisNode);

        return node;
    }
};

} // namespace internal

} // namespace gismo