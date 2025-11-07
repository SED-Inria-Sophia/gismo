/** @file XmlMesh.h

    @brief XML specialization for gsMesh using XmlRegistry dispatch

    This file contains the XML serialization specialization for gsMesh
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
#include <gsUtils/gsMesh/gsMesh.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>
#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

/** @brief XML serialization class for gsMesh
 *
 * This class provides static methods for XML serialization/deserialization
 * of gsMesh objects and handles XmlRegistry registration.
 */
template<class T>
class XmlMesh
{
public:
    /// XML tag name
    static std::string tag() { return "Mesh"; }

    /// XML type attribute value
    static std::string type() { return "off"; }

    /// Deserialize gsMesh<T> from XML
    static gsMesh<T>* get(gsXmlNode* node)
    {
        // Original get logic from XmlUtils.hpp
        GISMO_ASSERT( ( !strcmp( node->name(),"Mesh") )
                    &&  ( !strcmp(node->first_attribute("type")->value(),"off") ),
                    "Reading gsMesh XML: No Mesh with type 'off' found" );

        gsMesh<T> * m = new gsMesh<T>;
        std::istringstream str;
        str.str( node->value() );

        unsigned n  = atoi ( node->first_attribute("vertices")->value() ) ;
        T x,y, z;
        for (unsigned i=0; i<n; ++i)
        {
            gsGetReal(str, x);
            gsGetReal(str, y);
            gsGetReal(str, z);
            m->addVertex(x,y,z);
        }

        n  = atoi ( node->first_attribute("faces")->value() ) ;
        unsigned c = 0;
        std::vector<int> face;
        for (unsigned i=0; i<n; ++i)
        {
            gsGetInt(str, c);
            face.resize(c);
            for (unsigned j=0; j<c; ++j)
                gsGetInt(str, face[j]);
            m->addFace(face);
        }
        m->cleanMesh();
        return m;
    }

    /// Serialize gsMesh<T> to XML
    static gsXmlNode* put(const gsMesh<T>& obj, gsXmlTree& data)
    {
        // Original put logic from XmlUtils.hpp
        // Note: Original implementation returned NULL, indicating incomplete serialization
        // TODO: Implement actual mesh serialization if needed
        GISMO_UNUSED(obj);
        GISMO_UNUSED(data);
        return NULL;
    }

    /// Register with XmlRegistry
    static void registerType() {
        XmlRegistry::registerType<gsMesh<T>>(
            type(),
            get,
            put,
            tag()
        );
    }
};

/** @brief XML specialization for gsMesh with XmlRegistry dispatch
 *
 * This class maintains the familiar gsXml<gsMesh<T>> interface
 * while dispatching to the modern XmlRegistry system under the hood.
 * This provides backward compatibility and a smooth migration path.
 */
template<class T>
class gsXml< gsMesh<T> > {
private:
    gsXml() { }
    typedef gsMesh<T> Object;

    // Note: Registration handled in XmlUtils_.cpp to ensure proper initialization order

public:
    // Legacy macro support for backward compatibility
    GSXML_COMMON_FUNCTIONS(Object);
    GSXML_GET_INTO(Object);

    /// XML tag name
    static std::string tag() { return XmlMesh<T>::tag(); }

    /// XML type attribute value
    static std::string type() { return XmlMesh<T>::type(); }

    /** @brief Deserialize gsMesh from XML (dispatches to XmlRegistry)
     *
     * @param node XML node containing the mesh data
     * @return Pointer to new gsMesh object, or nullptr on failure
     */
    static Object* get(gsXmlNode* node) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlUtils_.cpp
        return XmlRegistry::get<Object>(node);
    }

    /** @brief Serialize gsMesh to XML (dispatches to XmlRegistry)
     *
     * @param obj gsMesh object to serialize
     * @param data XML document for memory allocation
     * @return XML node containing the mesh data
     */
    static gsXmlNode* put(const Object& obj, gsXmlTree& data) {
        // Dispatch to XmlRegistry system
        // Registration handled in XmlUtils_.cpp
        return XmlRegistry::put(obj, data);
    }
};

} // namespace internal
} // namespace gismo
