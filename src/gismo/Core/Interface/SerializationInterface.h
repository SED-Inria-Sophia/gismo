/** @file SerializationInterface.h

    @brief Abstract interface for object serialization.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#pragma once

#include <gismo/Common/Common>
#include <string>
#include <memory>
#include <typeinfo>

namespace gismo
{

/**
 * @brief Abstract interface for object serialization
 *
 * This interface provides a pluggable serialization system that allows
 * different serialization backends (XML, JSON, Binary, etc.) to be used
 * without tight coupling to specific implementations.
 *
 * \ingroup Core
 */
class GISMO_EXPORT SerializationInterface
{
public:
    virtual ~SerializationInterface() = default;

    /**
     * @brief Serialize an object to a string
     * @param obj Pointer to the object to serialize
     * @param typeInfo Type information for the object
     * @return Serialized string representation
     */
    virtual std::string serialize(const void* obj, const std::type_info& typeInfo) const = 0;

    /**
     * @brief Deserialize an object from a string
     * @param data Serialized string data
     * @param typeInfo Type information for the target object
     * @return Pointer to deserialized object (caller owns)
     */
    virtual void* deserialize(const std::string& data, const std::type_info& typeInfo) const = 0;

    /**
     * @brief Save an object to a file
     * @param obj Pointer to the object to save
     * @param typeInfo Type information for the object
     * @param filename Output filename
     * @return true if successful, false otherwise
     */
    virtual bool saveToFile(const void* obj, const std::type_info& typeInfo, const std::string& filename) const = 0;

    /**
     * @brief Load an object from a file
     * @param filename Input filename
     * @param typeInfo Type information for the target object
     * @return Pointer to loaded object (caller owns)
     */
    virtual void* loadFromFile(const std::string& filename, const std::type_info& typeInfo) const = 0;

    /**
     * @brief Check if this serializer supports the given type
     * @param typeInfo Type information to check
     * @return true if supported, false otherwise
     */
    virtual bool supportsType(const std::type_info& typeInfo) const = 0;

    /**
     * @brief Get the name/identifier of this serialization format
     * @return Format name (e.g., "XML", "JSON", "Binary")
     */
    virtual std::string getFormatName() const = 0;
};

/**
 * @brief Template wrapper for type-safe serialization
 *
 * This template provides a convenient, type-safe interface over
 * the generic SerializationInterface.
 */
template<typename T>
class TypedSerializer
{
private:
    std::shared_ptr<SerializationInterface> m_serializer;

public:
    explicit TypedSerializer(std::shared_ptr<SerializationInterface> serializer)
        : m_serializer(serializer) {}

    /**
     * @brief Serialize an object of type T
     * @param obj Object to serialize
     * @return Serialized string
     */
    std::string serialize(const T& obj) const
    {
        if (!m_serializer->supportsType(typeid(T)))
            throw std::runtime_error("Type not supported by serializer");
        return m_serializer->serialize(&obj, typeid(T));
    }

    /**
     * @brief Deserialize an object of type T
     * @param data Serialized data
     * @return Unique pointer to deserialized object
     */
    std::unique_ptr<T> deserialize(const std::string& data) const
    {
        if (!m_serializer->supportsType(typeid(T)))
            throw std::runtime_error("Type not supported by serializer");
        void* ptr = m_serializer->deserialize(data, typeid(T));
        return std::unique_ptr<T>(static_cast<T*>(ptr));
    }

    /**
     * @brief Save object to file
     * @param obj Object to save
     * @param filename Output filename
     * @return true if successful
     */
    bool saveToFile(const T& obj, const std::string& filename) const
    {
        if (!m_serializer->supportsType(typeid(T)))
            return false;
        return m_serializer->saveToFile(&obj, typeid(T), filename);
    }

    /**
     * @brief Load object from file
     * @param filename Input filename
     * @return Unique pointer to loaded object
     */
    std::unique_ptr<T> loadFromFile(const std::string& filename) const
    {
        if (!m_serializer->supportsType(typeid(T)))
            return nullptr;
        void* ptr = m_serializer->loadFromFile(filename, typeid(T));
        return std::unique_ptr<T>(static_cast<T*>(ptr));
    }
};

/**
 * @brief Get the default serialization interface
 *
 * Returns the currently registered serialization backend.
 * If no XML serializer is available, returns a null pointer.
 *
 * @return Shared pointer to serialization interface
 */
GISMO_EXPORT std::shared_ptr<SerializationInterface> getDefaultSerializer();

/**
 * @brief Register a serialization backend
 *
 * @param serializer Serialization implementation to register
 */
GISMO_EXPORT void setDefaultSerializer(std::shared_ptr<SerializationInterface> serializer);

/**
 * @brief Create a typed serializer for a specific type
 *
 * @tparam T Type to create serializer for
 * @return TypedSerializer instance
 */
template<typename T>
TypedSerializer<T> createTypedSerializer()
{
    return TypedSerializer<T>(getDefaultSerializer());
}

} // namespace gismo