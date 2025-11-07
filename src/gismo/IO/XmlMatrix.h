/** @file XmlMatrix.h

    @brief XML specialization for gsMatrix using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsMatrix
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
#include <gismo/Math/LinearAlgebra.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsMatrix
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsMatrix objects and handles XmlRegistry registration.
 */
template<class T>
class XmlMatrix
{
public:
    /// XML tag name
    static std::string tag() { return "Matrix"; }

    /// XML type attribute value
    static std::string type() { return ""; }

    /// Deserialize gsMatrix<T> from XML
    static gsMatrix<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlUtils.hpp
        GISMO_ASSERT( !strcmp( node->name(),"Matrix"),
                      "Something went wrong. Expected Matrix tag." );

        unsigned rows = atoi(node->first_attribute("rows")->value());
        unsigned cols = atoi(node->first_attribute("cols")->value());
        gsXmlAttribute *format = node->first_attribute("format");
        std::string format_flag = format ? format->value() : "ascii";

        gsMatrix<T> * obj = new gsMatrix<T>();
        getMatrixFromXml<T>(node, rows, cols, *obj, format_flag);
        return obj;
    }

    /// Serialize gsMatrix<T> to XML
    static gsXmlNode* put(const gsMatrix<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlUtils.hpp
        gsXmlNode * mat_data = putMatrixToXml(obj, data);
        // Record matrix dimensions
        mat_data->append_attribute(
            makeAttribute("rows", obj.rows(), data) );
        mat_data->append_attribute(
            makeAttribute("cols", obj.cols(), data) );

        return mat_data;
    }

    /// Register with XmlRegistry
    static void registerType() {
        XmlRegistry::registerType<gsMatrix<T>>(
            type(),
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsMatrix with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsMatrix<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsMatrix<T> > {
private:
    gsXml() { }
    typedef gsMatrix<T> Object;

    // Note: Registration handled in XmlUtils_.cpp to ensure proper initialization order

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);

    /// XML tag name
    static std::string tag() { return XmlMatrix<T>::tag(); }

    /// XML type attribute value
    static std::string type() { return XmlMatrix<T>::type(); }

    /** @brief Deserialize gsMatrix from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the matrix data
     * @return Pointer to new gsMatrix object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlUtils_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsMatrix to XML (dispatches to XmlRegistry)
     *
     * @param obj gsMatrix object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the matrix data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlUtils_.cpp
        return XmlRegistry::put(obj, data);
    }

    /** @brief Deserialize gsMatrix from XML into existing object
     *
     * @param node XML node containing the matrix data
     * @param obj Reference to existing gsMatrix object to populate
     */
    static void get_into(gsXmlNode* node, Object& obj) {
        // Original get_into logic from XmlUtils.hpp
        GISMO_ASSERT( !strcmp( node->name(),"Matrix"),
                      "Something went wrong. Expected Matrix tag." );

        unsigned rows = atoi(node->first_attribute("rows")->value());
        unsigned cols = atoi(node->first_attribute("cols")->value());
        gsXmlAttribute *format = node->first_attribute("format");
        std::string format_flag = format ? format->value() : "ascii";
        getMatrixFromXml<T>(node, rows, cols, obj, format_flag);
    }

};

} // namespace internal
} // namespace gismo