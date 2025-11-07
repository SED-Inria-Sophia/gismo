/** @file XmlComposedBasis.h

    @brief XML serialization for gsComposedBasis using XmlRegistry

    This file contains the XML serialization implementation for gsComposedBasis
    which has a single template parameter and can be directly registered.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris, Architecture Refactoring Team
*/

#pragma once

// Include the concrete class
#include <gismo/Core/Basis/ComposedBasis.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>
#include <gismo/IO/XmlBasis.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsComposedBasis
 *
 * This class provides XML serialization for gsComposedBasis<T> and handles
 * registration with the XmlRegistry system for polymorphic dispatch.
 *
 * @tparam T Scalar type (real_t, float, double, etc.)
 */
template<class T>
class XmlComposedBasis {
public:
    /// XML tag name
    static std::string tag() { return "Basis"; }

    /// XML type attribute value
    static std::string type() { return "ComposedBasis"; }

    /// Deserialize gsComposedBasis from XML
    static gsComposedBasis<T>* get(gsXmlNode* node) {
        GISMO_ASSERT( ( !strcmp( node->name(),"Basis") )
                    &&  ( !strcmp(node->first_attribute("type")->value(),
                                type().c_str() ) ),
                    "Something is wrong with the XML data: There should be a node with a "<<
                    type().c_str()<<" Basis.");

        typedef typename gsComposedBasis<T>::CompositionT CompositionType;
        typedef typename gsComposedBasis<T>::BasisT       BasisType;

        // The XML node will have two parts: a composition (gsGeometry) and a basis (gsBasis)
        // 1. Get the composition
        gsXmlNode* compNode = node->first_node("Composition");
        GISMO_ASSERT(compNode, "gsXmlUtils: get ComposedBasis: No composition found.");
        CompositionType * composition;
        if      (gsXmlNode* compData = compNode->first_node("Geometry"))
        {
            // Use non-typed XmlRegistry method to avoid including gsGeometry<T>
            gsXmlAttribute* gtype = compData->first_attribute("type");
            GISMO_ASSERT(gtype, "gsXmlUtils: get ComposedBasis: Geometry node missing type attribute.");
            std::string xmlType = gtype->value();
            composition = static_cast<CompositionType*>(
                XmlRegistry::getByXmlType(xmlType, compData)
            );
        }
        else if (gsXmlNode* compData2 = compNode->first_node("Function"))
        {
            // Use non-typed XmlRegistry method to avoid including gsFunction<T>
            gsXmlAttribute* ftype = compData2->first_attribute("type");
            GISMO_ASSERT(ftype, "gsXmlUtils: get ComposedBasis: Function node missing type attribute.");
            std::string xmlType = ftype->value();
            composition = static_cast<CompositionType*>(
                XmlRegistry::getByXmlType(xmlType, compData2)
            );
        }
        else
            GISMO_ERROR("gsXmlUtils: get ComposedBasis: No composition found.");

        // 2. Get the basis
        gsXmlNode* basisNode = node->first_node("Basis");
        GISMO_ASSERT(basisNode, "gsXmlUtils: get ComposedBasis: No basis found.");
        gsXmlNode* basisData = basisNode->first_node("Basis");
        GISMO_ASSERT(basisData, "gsXmlUtils: get ComposedBasis: No basis data found.");

        // Use non-typed XmlRegistry method to avoid including BasisType
        gsXmlAttribute* btype = basisData->first_attribute("type");
        GISMO_ASSERT(btype, "gsXmlUtils: get ComposedBasis: Basis data missing type attribute.");
        std::string xmlType = btype->value();
        BasisType * basis = static_cast<BasisType*>(
            XmlRegistry::getByXmlType(xmlType, basisData)
        );

        return new gsComposedBasis<T>(memory::make_shared(composition), memory::make_shared(basis));
    }

    /** @brief Serialize gsComposedBasis to XML node
     *
     * This preserves the original putBasisToXml logic while integrating
     * with the modern registry system using non-typed XmlRegistry methods.
     *
     * @param obj gsComposedBasis object to serialize
     * @param data XML document for memory allocation
     * @return XML node representing the composed basis
     */
    static gsXmlNode* put(const gsComposedBasis<T>& obj, gsXmlTree& data)
    {
        typedef typename gsComposedBasis<T>::BasisT BasisType;

        gsXmlNode* node = internal::makeNode("Basis", data);
        node->append_attribute( makeAttribute("type", type(), data) );

        // Put composition
        gsXmlNode* comp_node = internal::makeNode("Composition", data);
        gsXmlNode* comp_content;
        if      (const gsGeometry<T> * geo = dynamic_cast<const gsGeometry<T> *>( &obj.composition() ))
            comp_content = XmlRegistry::putPolymorphic(*geo, data);
        else if (const gsFunction<T> * fun = dynamic_cast<const gsFunction<T> *>( &obj.composition() ))
            comp_content = XmlRegistry::putPolymorphic(*fun, data);
        else
            GISMO_ERROR("gsXmlUtils: put ComposedBasis: No known composition found.");
        comp_node->append_node(comp_content);
        node->append_node(comp_node);

        // Put basis - use explicit BasisT type to match original behavior
        gsXmlNode* basis_node = internal::makeNode("Basis", data);
        gsXmlNode* basis_content = XmlRegistry::put<BasisType>(obj.basis(), data);
        basis_node->append_node(basis_content);
        node->append_node(basis_node);

        return node;
    }

    /// Register with XmlRegistry
    static void registerType() {
        XmlRegistry::registerType<gsComposedBasis<T>>(
            type(),
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsComposedBasis with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsComposedBasis<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsComposedBasis<T> > {
private:
    gsXml() { }
    typedef gsComposedBasis<T> Object;

    // Note: Registration handled in XmlBasis_.cpp to ensure proper initialization order

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);

    /// XML tag name
    static std::string tag() { return XmlComposedBasis<T>::tag(); }

    /// XML type attribute value
    static std::string type() { return XmlComposedBasis<T>::type(); }

    /** @brief Deserialize gsComposedBasis from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the basis data
     * @return Pointer to new gsComposedBasis object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlBasis_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsComposedBasis to XML (dispatches to XmlRegistry)
     *
     * @param obj gsComposedBasis object to serialize
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
