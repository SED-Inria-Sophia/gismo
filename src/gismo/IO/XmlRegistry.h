/** @file XmlRegistry.h

    @brief Type-safe XML serialization registry for GISMO objects.

    This file provides a modern, type-safe replacement for the template-based
    XML serialization system. It uses std::type_index for reliable type
    identification and function pointers for efficient dispatch.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#pragma once

#include <functional>
#include <map>
#include <typeindex>
#include <typeinfo>
#include <string>

#include <gismo/IO/XmlTypes.h>

// Import XML types into local scope for convenience
using gismo::internal::gsXmlNode;
using gismo::internal::gsXmlAttribute;
using gismo::internal::gsXmlTree;

namespace gismo {
namespace internal {

/** @brief Type-safe XML serialization registry
 *
 * This class provides a modern alternative to template-based XML serialization.
 * It uses std::type_index for reliable type identification and stores type-erased
 * function pointers for efficient dispatch.
 *
 * Key benefits:
 * - Compile-time type safety during registration
 * - Runtime type dispatch for polymorphic classes
 * - No template instantiation dependencies between modules
 * - Efficient std::type_index-based lookup
 */
class XmlRegistry {
private:
    /// Storage for type-erased serialization functions
    struct SerializerFunctions {
        std::function<void*(gsXmlNode*)> get_func;
        std::function<gsXmlNode*(const void*, gsXmlTree&)> put_func;
        std::string tag;       ///< XML tag name (e.g., "Function", "Basis")
        std::string xmlType;   ///< XML type attribute (e.g., "FunctionExpr")
    };

    /// Type-safe lookup: C++ type -> serializer functions
    std::map<std::type_index, SerializerFunctions> serializers_;

    /// Reverse lookup: XML type string -> C++ type (for polymorphic get operations)
    std::map<std::string, std::type_index> xmltype_to_cpptype_;

    /// Singleton instance accessor
    static XmlRegistry& instance();

    /// Private constructor for singleton
    XmlRegistry() = default;

public:
    /// Non-copyable, non-movable singleton
    XmlRegistry(const XmlRegistry&) = delete;
    XmlRegistry& operator=(const XmlRegistry&) = delete;
    XmlRegistry(XmlRegistry&&) = delete;
    XmlRegistry& operator=(XmlRegistry&&) = delete;

    /** @brief Register a type for XML serialization
     *
     * @tparam T The C++ type to register
     * @param xmlType The XML type attribute value (e.g., "FunctionExpr")
     * @param get_func Function to deserialize from XML node to T*
     * @param put_func Function to serialize from T& to XML node
     * @param tag The XML tag name (e.g., "Function", "Basis")
     */
    template<class T>
    static void registerType(const std::string& xmlType,
                           std::function<T*(gsXmlNode*)> get_func,
                           std::function<gsXmlNode*(const T&, gsXmlTree&)> put_func,
                           const std::string& tag);

    /** @brief Get object by C++ type (for concrete type deserialization)
     *
     * @tparam T The expected C++ type
     * @param node XML node to deserialize from
     * @return Pointer to deserialized object of type T, or nullptr if not found
     */
    template<class T>
    static T* get(gsXmlNode* node);

    /** @brief Serialize object by C++ type (for concrete type serialization)
     *
     * @tparam T The C++ type of the object
     * @param obj Object to serialize
     * @param data XML document for memory allocation
     * @return XML node representing the object, or nullptr if not found
     */
    template<class T>
    static gsXmlNode* put(const T& obj, gsXmlTree& data);

    /** @brief Get object by XML type string (for polymorphic base class deserialization)
     *
     * This is used when deserializing abstract base classes where the concrete
     * type is determined by the XML "type" attribute.
     *
     * @param xmlType XML type attribute value (e.g., "FunctionExpr")
     * @param node XML node to deserialize from
     * @return Pointer to deserialized object, or nullptr if type not found
     */
    static void* getByXmlType(const std::string& xmlType, gsXmlNode* node);

    /** @brief Get object by XML type attribute (typed version for polymorphic deserialization)
     *
     * This is a type-safe wrapper around getByXmlType that returns the correct type.
     * Used for deserializing abstract base classes where the concrete type is
     * determined by the XML "type" attribute.
     *
     * @tparam BaseT The expected base class type
     * @param node XML node to deserialize from
     * @param xmlType XML type attribute value (e.g., "FunctionExpr")
     * @return Pointer to deserialized object of base type, or nullptr if not found
     */
    template<class BaseT>
    static BaseT* getByTypeAttribute(gsXmlNode* node, const std::string& xmlType);

    /** @brief Serialize object using polymorphic dispatch (for abstract base classes)
     *
     * This method combines the functionality needed for polymorphic serialization.
     * It determines the runtime type and serializes accordingly.
     *
     * @tparam BaseT The base class type (can be abstract)
     * @param obj Object to serialize (actual type determined at runtime)
     * @param data XML document for memory allocation
     * @return XML node representing the object, or nullptr if concrete type not registered
     */
    template<class BaseT>
    static gsXmlNode* putPolymorphic(const BaseT& obj, gsXmlTree& data);

    /** @brief Serialize object using runtime type info (for polymorphic base class serialization)
     *
     * This is used when serializing through abstract base class pointers/references
     * where the concrete type is determined at runtime using typeid().
     *
     * @tparam BaseT The base class type (can be abstract)
     * @param obj Object to serialize (actual type determined at runtime)
     * @param data XML document for memory allocation
     * @return XML node representing the object, or nullptr if concrete type not registered
     */
    template<class BaseT>
    static gsXmlNode* putByRuntimeType(const BaseT& obj, gsXmlTree& data);

    /** @brief Check if a type is registered
     *
     * @tparam T The C++ type to check
     * @return True if type is registered, false otherwise
     */
    template<class T>
    static bool isRegistered();

    /** @brief Check if an XML type is registered
     *
     * @param xmlType XML type attribute value to check
     * @return True if XML type is registered, false otherwise
     */
    static bool isXmlTypeRegistered(const std::string& xmlType);

    /** @brief Get the XML type string for a C++ type
     *
     * @tparam T The C++ type
     * @return XML type string, or empty string if not registered
     */
    template<class T>
    static std::string getXmlType();

    /** @brief Get the XML tag for a C++ type
     *
     * @tparam T The C++ type
     * @return XML tag string, or empty string if not registered
     */
    template<class T>
    static std::string getXmlTag();
};

// =============================================================================
// Template Method Implementations
// =============================================================================

template<class T>
void XmlRegistry::registerType(const std::string& xmlType,
                              std::function<T*(gsXmlNode*)> get_func,
                              std::function<gsXmlNode*(const T&, gsXmlTree&)> put_func,
                              const std::string& tag) {
    std::type_index typeIdx(typeid(T));

    SerializerFunctions funcs;

    // Type-erased wrappers with compile-time type safety
    funcs.get_func = [get_func](gsXmlNode* node) -> void* {
        return get_func(node);
    };

    funcs.put_func = [put_func](const void* obj, gsXmlTree& data) -> gsXmlNode* {
        return put_func(*static_cast<const T*>(obj), data);
    };

    funcs.tag = tag;
    funcs.xmlType = xmlType;

    auto& inst = instance();
    inst.serializers_[typeIdx] = std::move(funcs);
    inst.xmltype_to_cpptype_.emplace(xmlType, typeIdx);
}

template<class T>
T* XmlRegistry::get(gsXmlNode* node) {
    std::type_index typeIdx(typeid(T));
    auto& inst = instance();
    auto it = inst.serializers_.find(typeIdx);
    return (it != inst.serializers_.end()) ?
           static_cast<T*>(it->second.get_func(node)) : nullptr;
}

template<class T>
gsXmlNode* XmlRegistry::put(const T& obj, gsXmlTree& data) {
    std::type_index typeIdx(typeid(T));
    auto& inst = instance();
    auto it = inst.serializers_.find(typeIdx);
    return (it != inst.serializers_.end()) ?
           it->second.put_func(&obj, data) : nullptr;
}

template<class BaseT>
gsXmlNode* XmlRegistry::putByRuntimeType(const BaseT& obj, gsXmlTree& data) {
    std::type_index actualType(typeid(obj));
    auto& inst = instance();
    auto it = inst.serializers_.find(actualType);
    return (it != inst.serializers_.end()) ?
           it->second.put_func(&obj, data) : nullptr;
}

template<class T>
bool XmlRegistry::isRegistered() {
    std::type_index typeIdx(typeid(T));
    auto& inst = instance();
    return inst.serializers_.find(typeIdx) != inst.serializers_.end();
}

template<class T>
std::string XmlRegistry::getXmlType() {
    std::type_index typeIdx(typeid(T));
    auto& inst = instance();
    auto it = inst.serializers_.find(typeIdx);
    return (it != inst.serializers_.end()) ? it->second.xmlType : "";
}

template<class T>
std::string XmlRegistry::getXmlTag() {
    std::type_index typeIdx(typeid(T));
    auto& inst = instance();
    auto it = inst.serializers_.find(typeIdx);
    return (it != inst.serializers_.end()) ? it->second.tag : "";
}

template<class BaseT>
BaseT* XmlRegistry::getByTypeAttribute(gsXmlNode* node, const std::string& xmlType) {
    void* obj = getByXmlType(xmlType, node);
    return static_cast<BaseT*>(obj);
}

template<class BaseT>
gsXmlNode* XmlRegistry::putPolymorphic(const BaseT& obj, gsXmlTree& data) {
    return putByRuntimeType(obj, data);
}

} // namespace internal
} // namespace gismo