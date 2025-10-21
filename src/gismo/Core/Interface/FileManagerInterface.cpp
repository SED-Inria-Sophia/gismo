/** @file FileManagerInterface.cpp

    @brief Implementation of file manager interface.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#include <gismo/Core/Interface/FileManagerInterface.h>
#include <fstream>
#include <cstdlib>

#if defined(_WIN32)
#include <direct.h>
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace gismo
{

/**
 * @brief Default implementation of FileManagerInterface
 *
 * This provides basic file operations using standard C++ and system calls.
 * More advanced functionality is available when gsIO is linked.
 */
class DefaultFileManager : public FileManagerInterface
{
public:
    std::string getTempPath() const override
    {
#if defined(_WIN32)
        const char* tmp = std::getenv("TEMP");
        if (tmp) return std::string(tmp);
        tmp = std::getenv("TMP");
        if (tmp) return std::string(tmp);
        return "C:\\temp";
#else
        const char* tmp = std::getenv("TMPDIR");
        if (tmp) return std::string(tmp);
        return "/tmp";
#endif
    }

    bool fileExists(const std::string& filename) const override
    {
        std::ifstream file(filename.c_str());
        return file.good();
    }

    bool createDirectory(const std::string& path) const override
    {
#if defined(_WIN32)
        return _mkdir(path.c_str()) == 0;
#else
        return mkdir(path.c_str(), 0755) == 0;
#endif
    }

    std::string getFileExtension(const std::string& filename) const override
    {
        size_t pos = filename.find_last_of('.');
        if (pos != std::string::npos && pos < filename.length() - 1)
            return filename.substr(pos + 1);
        return "";
    }

    std::string getBaseName(const std::string& filename) const override
    {
        // Find last path separator
        size_t pathPos = filename.find_last_of("/\\");
        std::string nameWithExt = (pathPos != std::string::npos) ?
                                  filename.substr(pathPos + 1) : filename;

        // Remove extension
        size_t extPos = nameWithExt.find_last_of('.');
        if (extPos != std::string::npos)
            return nameWithExt.substr(0, extPos);
        return nameWithExt;
    }

    std::string joinPath(const std::string& path1, const std::string& path2) const override
    {
        if (path1.empty()) return path2;
        if (path2.empty()) return path1;

        char sep =
#if defined(_WIN32)
            '\\';
#else
            '/';
#endif

        if (path1.back() == '/' || path1.back() == '\\')
            return path1 + path2;
        else
            return path1 + sep + path2;
    }
};

// Global file manager instance
static std::shared_ptr<FileManagerInterface> g_fileManager;

std::shared_ptr<FileManagerInterface> getDefaultFileManager()
{
    if (!g_fileManager)
        g_fileManager = std::make_shared<DefaultFileManager>();
    return g_fileManager;
}

void setDefaultFileManager(std::shared_ptr<FileManagerInterface> fileManager)
{
    g_fileManager = fileManager;
}

} // namespace gismo