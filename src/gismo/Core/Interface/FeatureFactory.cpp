/** @file FeatureFactory.cpp

    @brief Implementation of feature factory.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#include <gismo/Core/Interface/FeatureFactory.h>
#include <map>

namespace gismo
{

FeatureFactory& FeatureFactory::getInstance()
{
    static FeatureFactory instance;
    return instance;
}

void FeatureFactory::registerFileManagerCreator(FileManagerCreator creator)
{
    m_fileManagerCreator = creator;
}

void FeatureFactory::registerSerializationCreator(const std::string& name, SerializationCreator creator)
{
    m_serializationCreators[name] = creator;
}

std::shared_ptr<FileManagerInterface> FeatureFactory::createFileManager()
{
    if (m_fileManagerCreator)
        return m_fileManagerCreator();

    // Return default implementation
    return getDefaultFileManager();
}

std::shared_ptr<SerializationInterface> FeatureFactory::createSerializer(const std::string& format)
{
    auto it = m_serializationCreators.find(format);
    if (it != m_serializationCreators.end())
        return it->second();

    return nullptr; // No implementation available
}

bool FeatureFactory::isSerializationAvailable(const std::string& format) const
{
    return m_serializationCreators.find(format) != m_serializationCreators.end();
}

void FeatureFactory::initializeDefaults()
{
    // File manager defaults are already handled by getDefaultFileManager()
    // No serialization defaults - requires external registration
}

// Convenience functions
std::shared_ptr<FileManagerInterface> createFileManager()
{
    return FeatureFactory::getInstance().createFileManager();
}

std::shared_ptr<SerializationInterface> createSerializer(const std::string& format)
{
    return FeatureFactory::getInstance().createSerializer(format);
}

bool isXmlSerializationAvailable()
{
    return FeatureFactory::getInstance().isSerializationAvailable("XML");
}

} // namespace gismo