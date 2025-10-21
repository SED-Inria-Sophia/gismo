/** @file FeatureFactory.h

    @brief Factory pattern for optional Core features.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#pragma once

#include <gismo/Common/Common>
#include <gismo/Core/Interface/FileManagerInterface.h>
#include <gismo/Core/Interface/SerializationInterface.h>
#include <memory>
#include <functional>

namespace gismo
{

/**
 * @brief Factory for creating optional Core feature implementations
 *
 * This factory provides a registration system for optional features
 * that can be provided by higher-level modules (like gsIO) without
 * creating circular dependencies.
 *
 * \ingroup Core
 */
class GISMO_EXPORT FeatureFactory
{
public:
    /// Function type for creating file manager instances
    using FileManagerCreator = std::function<std::shared_ptr<FileManagerInterface>()>;

    /// Function type for creating serialization instances
    using SerializationCreator = std::function<std::shared_ptr<SerializationInterface>()>;

    /**
     * @brief Get the singleton factory instance
     * @return Reference to the factory
     */
    static FeatureFactory& getInstance();

    /**
     * @brief Register a file manager creator function
     * @param creator Function that creates file manager instances
     */
    void registerFileManagerCreator(FileManagerCreator creator);

    /**
     * @brief Register a serialization creator function
     * @param name Name of the serialization format (e.g., "XML", "JSON")
     * @param creator Function that creates serialization instances
     */
    void registerSerializationCreator(const std::string& name, SerializationCreator creator);

    /**
     * @brief Create a file manager instance
     * @return Shared pointer to file manager (may be default implementation)
     */
    std::shared_ptr<FileManagerInterface> createFileManager();

    /**
     * @brief Create a serialization instance
     * @param format Serialization format name
     * @return Shared pointer to serializer (may be null if not available)
     */
    std::shared_ptr<SerializationInterface> createSerializer(const std::string& format = "XML");

    /**
     * @brief Check if a serialization format is available
     * @param format Format name to check
     * @return true if available, false otherwise
     */
    bool isSerializationAvailable(const std::string& format = "XML") const;

    /**
     * @brief Initialize factory with default implementations
     *
     * This sets up the factory to use default implementations that
     * don't require external dependencies.
     */
    void initializeDefaults();

private:
    FeatureFactory() = default;

    FileManagerCreator m_fileManagerCreator;
    std::map<std::string, SerializationCreator> m_serializationCreators;
};

/**
 * @brief Convenience function to get a file manager
 * @return Shared pointer to file manager interface
 */
GISMO_EXPORT std::shared_ptr<FileManagerInterface> createFileManager();

/**
 * @brief Convenience function to get a serializer
 * @param format Serialization format (default: "XML")
 * @return Shared pointer to serialization interface
 */
GISMO_EXPORT std::shared_ptr<SerializationInterface> createSerializer(const std::string& format = "XML");

/**
 * @brief Check if XML serialization is available
 * @return true if XML serialization can be created
 */
GISMO_EXPORT bool isXmlSerializationAvailable();

} // namespace gismo