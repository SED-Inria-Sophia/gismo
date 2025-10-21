/** @file XmlSerializer.h

    @brief XML implementation of SerializationInterface.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#pragma once

#include <gismo/Core/Interface/SerializationInterface.h>
#include <gismo/Core/Interface/FileManagerInterface.h>
#include <gismo/XmlIO/Core/Xml.h>
#include <gismo/XmlIO/Core/FileManager.h>

namespace gismo
{

/**
 * @brief Complete XML implementation of SerializationInterface
 *
 * This class provides full XML serialization functionality by connecting
 * the extracted XML specializations to the Core interface system.
 */
class GISMO_EXPORT XmlSerializationImpl : public SerializationInterface
{
public:
    XmlSerializationImpl();

    std::string getFormatName() const override { return "XML"; }

    std::string serialize(const void* obj, const std::type_info& typeInfo) const override;

    void* deserialize(const std::string& data, const std::type_info& typeInfo) const override;

    bool saveToFile(const void* obj, const std::type_info& typeInfo, const std::string& filename) const override;

    void* loadFromFile(const std::string& filename, const std::type_info& typeInfo) const override;

    bool supportsType(const std::type_info& typeInfo) const override;

private:
    /**
     * @brief Initialize XML serialization registry
     *
     * This sets up the type mappings for all Core types that have
     * XML specializations in the Serialization/ directory.
     */
    void initializeTypeRegistry();

    std::map<std::type_index, std::string> m_typeNames;
};

/**
 * @brief Complete gsIO implementation of FileManagerInterface
 *
 * This bridges the existing gsFileManager with the Core interface.
 */
class GISMO_EXPORT XmlFileManagerImpl : public FileManagerInterface
{
public:
    bool fileExists(const std::string& path) const override;

    std::string getTempPath() const override;

    std::string getBaseName(const std::string& path) const override;

    std::string getFileExtension(const std::string& path) const override;

    std::string joinPath(const std::string& dir, const std::string& file) const override;

    std::vector<std::string> listFiles(const std::string& directory, const std::string& pattern = "*") const override;

    bool createDirectory(const std::string& path) const override;

    bool removeFile(const std::string& path) const override;
};

} // namespace gismo