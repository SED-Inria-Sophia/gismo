/** @file XmlRegistry.cpp

    @brief Implementation of non-template methods for XmlRegistry.

    This file contains the implementation of non-template methods for the
    type-safe XML serialization registry. Template methods are implemented
    inline in the header file.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#include <gismo/IO/XmlRegistry.h>

namespace gismo {
namespace internal {

// =============================================================================
// Static Methods Implementation
// =============================================================================

XmlRegistry& XmlRegistry::instance() {
    static XmlRegistry registry; 
    return registry;
}

void* XmlRegistry::getByXmlType(const std::string& xmlType, gsXmlNode* node) {
    auto& inst = instance();
    auto it = inst.xmltype_to_cpptype_.find(xmlType);
    if (it == inst.xmltype_to_cpptype_.end()) {
        return nullptr;
    }
    
    auto ser_it = inst.serializers_.find(it->second);
    return (ser_it != inst.serializers_.end()) ? 
           ser_it->second.get_func(node) : nullptr;
}

bool XmlRegistry::isXmlTypeRegistered(const std::string& xmlType) {
    auto& inst = instance();
    return inst.xmltype_to_cpptype_.find(xmlType) != inst.xmltype_to_cpptype_.end();
}

} // namespace internal
} // namespace gismo