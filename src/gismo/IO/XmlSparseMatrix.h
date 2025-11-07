/** @file XmlSparseMatrix.h

    @brief XML specialization for gsSparseMatrix using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsSparseMatrix
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

/** @brief XML serialization class for gsSparseMatrix
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsSparseMatrix objects and handles XmlRegistry registration.
 */
template<class T>
class XmlSparseMatrix
{
public:
    /// XML tag name
    static std::string tag() { return "SparseMatrix"; }

    /// XML type attribute value
    static std::string type() { return ""; }

    /// Deserialize gsSparseMatrix<T> from XML
    static gsSparseMatrix<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlUtils.hpp
        GISMO_ASSERT( !strcmp( node->name(),"SparseMatrix"),
                      "Something went wrong. Expected SparseMatrix tag." );

        const index_t rows  = atoi ( node->first_attribute("rows")->value() ) ;
        const index_t cols  = atoi ( node->first_attribute("cols")->value() ) ;

        gsSparseEntries<T> entries;
        getSparseEntriesFromXml<T>(node, entries);

        gsSparseMatrix<T>* obj = new gsSparseMatrix<T>();
        obj->resize(rows,cols);
        obj->setFrom(entries);
        return obj;
    }

    /// Serialize gsSparseMatrix<T> to XML
    static gsXmlNode* put(const gsSparseMatrix<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlUtils.hpp
        gsXmlNode * mat_data = putSparseMatrixToXml(obj, data);

        mat_data->append_attribute(
            makeAttribute("rows", obj.rows(), data) );
        mat_data->append_attribute(
            makeAttribute("cols", obj.cols(), data) );

        return mat_data;
    }

    /// Register with XmlRegistry
    static void registerType() {
        XmlRegistry::registerType<gsSparseMatrix<T>>(
            type(),
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsSparseMatrix with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsSparseMatrix<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsSparseMatrix<T> > {
private:
    gsXml() { }
    typedef gsSparseMatrix<T> Object;

    // Note: Registration handled in XmlUtils_.cpp to ensure proper initialization order

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);

    /// XML tag name
    static std::string tag() { return XmlSparseMatrix<T>::tag(); }

    /// XML type attribute value
    static std::string type() { return XmlSparseMatrix<T>::type(); }

    /** @brief Deserialize gsSparseMatrix from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the sparse matrix data
     * @return Pointer to new gsSparseMatrix object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlUtils_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsSparseMatrix to XML (dispatches to XmlRegistry)
     *
     * @param obj gsSparseMatrix object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the sparse matrix data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlUtils_.cpp
        return XmlRegistry::put(obj, data);
    }

    /** @brief Deserialize gsSparseMatrix from XML into existing object
     *
     * @param node XML node containing the sparse matrix data
     * @param obj Reference to existing gsSparseMatrix object to populate
     */
    static void get_into(gsXmlNode* node, Object& obj) {
        // Original get_into logic from XmlUtils.hpp
        GISMO_ASSERT( !strcmp( node->name(),"SparseMatrix"),
                      "Something went wrong. Expected SparseMatrix tag." );

        const index_t rows  = atoi ( node->first_attribute("rows")->value() ) ;
        const index_t cols  = atoi ( node->first_attribute("cols")->value() ) ;

        gsSparseEntries<T> entries;
        getSparseEntriesFromXml<T>(node, entries);

        obj.resize(rows,cols);
        obj.setFrom(entries);
    }

};

} // namespace internal
} // namespace gismo