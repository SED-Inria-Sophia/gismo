/** @file XmlSerializer.cpp

    @brief Implementation of XML serialization for Core types.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#include <gismo/XmlIO/XmlSerializer.h>

// Include all the extracted XML specializations
#include <gismo/XmlIO/Serialization/ComposedBasis.xml.hpp>
#include <gismo/XmlIO/Serialization/ComposedGeometry.xml.hpp>
#include <gismo/XmlIO/Serialization/ComposedFunction.xml.hpp>
#include <gismo/XmlIO/Serialization/ConstantFunction.xml.hpp>
#include <gismo/XmlIO/Serialization/FunctionExpr.xml.hpp>
#include <gismo/XmlIO/Serialization/JITCompiler.xml.hpp>
#include <gismo/XmlIO/Serialization/MultiBasis.assembler.hpp>

#include <typeinfo>
#include <typeindex>

namespace gismo
{

XmlSerializationImpl::XmlSerializationImpl()
{
    initializeTypeRegistry();
}

void XmlSerializationImpl::initializeTypeRegistry()
{
    // Register all Core types that have XML specializations
    // These correspond to the files we extracted in Phase 1

    m_typeNames[std::type_index(typeid(gsComposedBasis<real_t>))] = "ComposedBasis";
    m_typeNames[std::type_index(typeid(gsComposedGeometry<real_t>))] = "ComposedGeometry";
    m_typeNames[std::type_index(typeid(gsComposedFunction<real_t>))] = "ComposedFunction";
    m_typeNames[std::type_index(typeid(gsConstantFunction<real_t>))] = "ConstantFunction";
    m_typeNames[std::type_index(typeid(gsFunctionExpr<real_t>))] = "FunctionExpr";
    m_typeNames[std::type_index(typeid(gsJITCompiler))] = "JITCompiler";
    m_typeNames[std::type_index(typeid(gsMultiBasis<real_t>))] = "MultiBasis";
}

std::string XmlSerializationImpl::serialize(const void* obj, const std::type_info& typeInfo) const
{
    std::type_index typeIdx(typeInfo);
    auto it = m_typeNames.find(typeIdx);

    if (it == m_typeNames.end()) {
        throw std::runtime_error("Unsupported type for XML serialization: " + std::string(typeInfo.name()));
    }

    // Use the gsXml system with the extracted specializations
    std::ostringstream xmlStream;

    try {
        // Route to appropriate XML specialization based on type
        if (typeInfo == typeid(gsComposedBasis<real_t>)) {
            const auto* typedObj = static_cast<const gsComposedBasis<real_t>*>(obj);
            gsXml::writeToStream(*typedObj, xmlStream);
        }
        else if (typeInfo == typeid(gsComposedGeometry<real_t>)) {
            const auto* typedObj = static_cast<const gsComposedGeometry<real_t>*>(obj);
            gsXml::writeToStream(*typedObj, xmlStream);
        }
        else if (typeInfo == typeid(gsComposedFunction<real_t>)) {
            const auto* typedObj = static_cast<const gsComposedFunction<real_t>*>(obj);
            gsXml::writeToStream(*typedObj, xmlStream);
        }
        else if (typeInfo == typeid(gsConstantFunction<real_t>)) {
            const auto* typedObj = static_cast<const gsConstantFunction<real_t>*>(obj);
            gsXml::writeToStream(*typedObj, xmlStream);
        }
        else if (typeInfo == typeid(gsFunctionExpr<real_t>)) {
            const auto* typedObj = static_cast<const gsFunctionExpr<real_t>*>(obj);
            gsXml::writeToStream(*typedObj, xmlStream);
        }
        else if (typeInfo == typeid(gsJITCompiler)) {
            const auto* typedObj = static_cast<const gsJITCompiler*>(obj);
            gsXml::writeToStream(*typedObj, xmlStream);
        }
        else if (typeInfo == typeid(gsMultiBasis<real_t>)) {
            const auto* typedObj = static_cast<const gsMultiBasis<real_t>*>(obj);
            gsXml::writeToStream(*typedObj, xmlStream);
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("XML serialization failed for " + it->second + ": " + e.what());
    }

    return xmlStream.str();
}

void* XmlSerializationImpl::deserialize(const std::string& data, const std::type_info& typeInfo) const
{
    std::type_index typeIdx(typeInfo);
    auto it = m_typeNames.find(typeIdx);

    if (it == m_typeNames.end()) {
        throw std::runtime_error("Unsupported type for XML deserialization: " + std::string(typeInfo.name()));
    }

    std::istringstream xmlStream(data);

    try {
        // Route to appropriate XML specialization based on type
        if (typeInfo == typeid(gsComposedBasis<real_t>)) {
            auto* obj = new gsComposedBasis<real_t>();
            gsXml::readFromStream(*obj, xmlStream);
            return obj;
        }
        else if (typeInfo == typeid(gsComposedGeometry<real_t>)) {
            auto* obj = new gsComposedGeometry<real_t>();
            gsXml::readFromStream(*obj, xmlStream);
            return obj;
        }
        else if (typeInfo == typeid(gsComposedFunction<real_t>)) {
            auto* obj = new gsComposedFunction<real_t>();
            gsXml::readFromStream(*obj, xmlStream);
            return obj;
        }
        else if (typeInfo == typeid(gsConstantFunction<real_t>)) {
            auto* obj = new gsConstantFunction<real_t>();
            gsXml::readFromStream(*obj, xmlStream);
            return obj;
        }
        else if (typeInfo == typeid(gsFunctionExpr<real_t>)) {
            auto* obj = new gsFunctionExpr<real_t>();
            gsXml::readFromStream(*obj, xmlStream);
            return obj;
        }
        else if (typeInfo == typeid(gsJITCompiler)) {
            auto* obj = new gsJITCompiler();
            gsXml::readFromStream(*obj, xmlStream);
            return obj;
        }
        else if (typeInfo == typeid(gsMultiBasis<real_t>)) {
            auto* obj = new gsMultiBasis<real_t>();
            gsXml::readFromStream(*obj, xmlStream);
            return obj;
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("XML deserialization failed for " + it->second + ": " + e.what());
    }

    return nullptr;
}

bool XmlSerializationImpl::saveToFile(const void* obj, const std::type_info& typeInfo, const std::string& filename) const
{
    try {
        std::string xmlData = serialize(obj, typeInfo);
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << xmlData;
        return file.good();
    }
    catch (const std::exception&) {
        return false;
    }
}

void* XmlSerializationImpl::loadFromFile(const std::string& filename, const std::type_info& typeInfo) const
{
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return nullptr;
        }

        std::string xmlData((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());

        return deserialize(xmlData, typeInfo);
    }
    catch (const std::exception&) {
        return nullptr;
    }
}

bool XmlSerializationImpl::supportsType(const std::type_info& typeInfo) const
{
    std::type_index typeIdx(typeInfo);
    return m_typeNames.find(typeIdx) != m_typeNames.end();
}

// XmlFileManagerImpl implementation
bool XmlFileManagerImpl::fileExists(const std::string& path) const
{
    return gsFileManager::fileExists(path);
}

std::string XmlFileManagerImpl::getTempPath() const
{
    return gsFileManager::getTempPath();
}

std::string XmlFileManagerImpl::getBaseName(const std::string& path) const
{
    return gsFileManager::getBasename(path);
}

std::string XmlFileManagerImpl::getFileExtension(const std::string& path) const
{
    return gsFileManager::getExtension(path);
}

std::string XmlFileManagerImpl::joinPath(const std::string& dir, const std::string& file) const
{
    return gsFileManager::joinPath(dir, file);
}

std::vector<std::string> XmlFileManagerImpl::listFiles(const std::string& directory, const std::string& pattern) const
{
    return gsFileManager::listFiles(directory, pattern);
}

bool XmlFileManagerImpl::createDirectory(const std::string& path) const
{
    return gsFileManager::createDirectory(path);
}

bool XmlFileManagerImpl::removeFile(const std::string& path) const
{
    return gsFileManager::removeFile(path);
}

} // namespace gismo