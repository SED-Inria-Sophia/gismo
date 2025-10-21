/** @file FileManagerInterface.h

    @brief Abstract interface for file management operations.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#pragma once

#include <gismo/Common/Common>
#include <string>

namespace gismo
{

/**
 * @brief Abstract interface for file management operations
 *
 * This interface provides an abstraction layer for file operations,
 * allowing the Core module to work with file operations without
 * depending on specific implementations in gsIO.
 *
 * \ingroup Core
 */
class GISMO_EXPORT FileManagerInterface
{
public:
    virtual ~FileManagerInterface() = default;

    /**
     * @brief Get the system temporary directory path
     * @return Path to temporary directory
     */
    virtual std::string getTempPath() const = 0;

    /**
     * @brief Check if a file exists
     * @param filename Path to the file
     * @return true if file exists, false otherwise
     */
    virtual bool fileExists(const std::string& filename) const = 0;

    /**
     * @brief Create a directory if it doesn't exist
     * @param path Directory path to create
     * @return true if successful, false otherwise
     */
    virtual bool createDirectory(const std::string& path) const = 0;

    /**
     * @brief Get the file extension from a filename
     * @param filename Input filename
     * @return File extension (without dot)
     */
    virtual std::string getFileExtension(const std::string& filename) const = 0;

    /**
     * @brief Get the base name of a file (without path and extension)
     * @param filename Input filename
     * @return Base name
     */
    virtual std::string getBaseName(const std::string& filename) const = 0;

    /**
     * @brief Join path components
     * @param path1 First path component
     * @param path2 Second path component
     * @return Joined path
     */
    virtual std::string joinPath(const std::string& path1, const std::string& path2) const = 0;
};

/**
 * @brief Get the default file manager implementation
 *
 * This function returns a shared instance of the file manager.
 * The actual implementation is provided by the gsIO module when available.
 *
 * @return Shared pointer to file manager interface
 */
GISMO_EXPORT std::shared_ptr<FileManagerInterface> getDefaultFileManager();

/**
 * @brief Set a custom file manager implementation
 *
 * This allows dependency injection of file manager implementations
 * for testing or alternative backends.
 *
 * @param fileManager Custom file manager implementation
 */
GISMO_EXPORT void setDefaultFileManager(std::shared_ptr<FileManagerInterface> fileManager);

} // namespace gismo