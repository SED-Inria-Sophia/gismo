/** @file XmlGeometry.h

    @brief XML specialization for gsGeometry using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsGeometry
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
#include <gismo/Core/Geometry/Geometry.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsGeometry (abstract base)
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsGeometry objects and handles XmlRegistry registration for the base dispatcher.
 */
template<class T>
class XmlGeometry
{
public:
    /// XML tag name
    static std::string tag() { return "Geometry"; }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return ""; }

    /// Deserialize gsGeometry<T> from XML (polymorphic dispatcher)
    static gsGeometry<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlGeometry.hpp
        GISMO_ASSERT( ( !strcmp( node->name(),"Geometry") ),
                      "Something went wrong, was waiting for a Geometry tag.\n" );

        gsXmlAttribute * gtype = node->first_attribute("type");
        if ( ! gtype )
        {
            gsWarn<< "Geometry without a type in the xml file\n";
            return NULL;
        }

        std::string s = gtype->value();

        // Dispatch to XmlRegistry based on type attribute
        return XmlRegistry::getByTypeAttribute<gsGeometry<T>>(node, s);
    }

    /// Serialize gsGeometry<T> to XML (polymorphic dispatcher)
    static gsXmlNode* put(const gsGeometry<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlGeometry.hpp using XmlRegistry dispatch
        // Try to dispatch to XmlRegistry using dynamic type information
        return XmlRegistry::putPolymorphic(obj, data);
    }

    /// Register with XmlRegistry
    static void registerType() {
        // Note: gsGeometry<T> itself doesn't need direct registration since it's abstract
        // The actual registration happens for concrete derived types:
        // - gsComposedGeometry<T> (via XmlComposedGeometry.h)
        // - Other geometry types can be added here as they're implemented

        // However, we register the dispatcher for consistency with the pattern
        XmlRegistry::registerType<gsGeometry<T>>(
            type(), // Empty for abstract base
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsGeometry with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsGeometry<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsGeometry<T> > {
private:
    gsXml() { }
    typedef gsGeometry<T> Object;

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);

    /// XML tag name
    static std::string tag() { return XmlGeometry<T>::tag(); }

    /// XML type attribute value (empty for abstract base)
    static std::string type() { return XmlGeometry<T>::type(); }

    /** @brief Deserialize gsGeometry from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the geometry data
     * @return Pointer to new gsGeometry object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlGeometry_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsGeometry to XML (dispatches to XmlRegistry)
     *
     * @param obj gsGeometry object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the geometry data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlGeometry_.cpp
        return XmlRegistry::put(obj, data);
    }
};

// /////////////////////////////////////////////////////////////////////////////
// Helpers to get/put geometries
// /////////////////////////////////////////////////////////////////////////////

template<class Object>
Object * getGeometryFromXml ( gsXmlNode * node)
{
    //gsWarn<<"Reading "<< gsXml<Object>::type() <<" Geometry..\n";
    assert ( ( !strcmp( node->name(),"Geometry") ) &&
             ( !strcmp(node->first_attribute("type")->value(), gsXml<Object>::type().c_str() ) ) );

    gsXmlNode * tmp = node->first_node("Basis");

    // to do: avoid copy object here (remove Ptr)
    typename Object::Basis::Ptr b( gsXml<typename Object::Basis>::get(tmp) );

    //gsWarn<<"Read basis from node "<< tmp <<", got "<< *b <<"\n";

    tmp = node->first_node("coefs");
    GISMO_ASSERT( tmp, "Did not find any coefficients for "<< gsXml<Object>::type().c_str() );
    gsXmlAttribute * at_geodim = tmp->first_attribute("geoDim");
    GISMO_ASSERT( at_geodim , "geoDim attribute not found in Geometry XML tag");
    unsigned geoDim = atoi(at_geodim->value() ) ;

    //gsWarn<<"Read mat "<< b->size()<<"x"<< geoDim <<"\n";

    // Read the Coefficients and store them in a matrix
    gsMatrix<typename Object::Scalar_t> coefficient_matrix;
    gsXmlAttribute* format = tmp->first_attribute("format");
    std::string format_flag = format ? format->value() : "ascii";
    getMatrixFromXml<typename Object::Scalar_t>(
        tmp, b->size(), geoDim, coefficient_matrix, format_flag);


    gsXmlAttribute* coef_order = tmp->first_attribute("order");
    if (nullptr != coef_order)
        if (!strcmp(coef_order->value(), "coordinates")) {
            coefficient_matrix.transposeInPlace();
            coefficient_matrix.resize(b->size(), geoDim);
        }

    // Looking for transformations
    tmp = node->first_node("transform");
    gsMatrix<typename Object::Scalar_t> a;
    if ( tmp )
    {
        for (gsXmlNode * tr = tmp->first_node();
             tr; tr= tr->next_sibling() )
        {
            std::string val( tr->name() );

            if (val == "translation")
            {
                getMatrixFromXml<typename Object::Scalar_t>(tmp, 3, 1 ,a);
                // coefficient_matrix->rowwise() += a->transpose(); // TO DO
            }
            if (val ==  "rotation" ) // 3d
            {
                getMatrixFromXml<typename Object::Scalar_t>(tmp, 4, 1, a);
                gsEigen::Transform<typename Object::Scalar_t,3,gsEigen::Affine>
                    rot( gsEigen::AngleAxis<typename Object::Scalar_t>
                         ( a(3,0), a.template block<3,1>(0,0).normalized() ) );
                coefficient_matrix = (coefficient_matrix. rowwise().homogeneous() *
                     rot.matrix().transpose() ).leftCols(3) ;

            }
            if (val == "scale")
            {

            }
            else
            {
                gsWarn<< "Unidentified transform tag in XML.\n";
            }
        }
    }

    Object * result = new Object(*b, coefficient_matrix);
    return result;
}

/// Helper to put geometries to XML
//template<class Object>
//gsXmlNode * putGeometryToXml ( Object const & obj, gsXmlTree & data);
template<class Object>
gsXmlNode * putGeometryToXml ( Object const & obj, gsXmlTree & data)
{
    // Make a new XML Geometry node
    gsXmlNode * bs = internal::makeNode("Geometry", data);
    bs->append_attribute( makeAttribute("type",
                                        internal::gsXml<Object>::type().c_str(), data) );

    // Add the basis
    gsXmlNode* tmp =
	    internal::gsXml< typename Object::Basis >::put(obj.basis(), data);
	if ( ! tmp )
    {
	    gsWarn<<"XML Warning: Writing basis failed.\n";
	    return NULL;
    }

    bs->append_node(tmp);

    // Write the coefficient matrix
    tmp = putMatrixToXml( obj.coefs(), data, "coefs" );
    tmp->append_attribute( makeAttribute("geoDim", obj.geoDim(), data) );
    bs->append_node(tmp);
    return bs;
}

} // namespace internal
} // namespace gismo