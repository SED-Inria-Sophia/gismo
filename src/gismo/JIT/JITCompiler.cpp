/** @file JITCompiler.cpp

    @brief Implementation of JIT-compiler class.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): M. Moeller, Architecture Refactoring Team
*/

#include <gismo/JIT/JITCompiler.h>
#include <gismo/IO/FileManager.h>
#include <gismo/IO/FileData.h>
#include <sstream>
#include <fstream>

namespace gismo {

// =====================================================
// gsJITCompilerConfig implementations
// =====================================================

gsJITCompilerConfig gsJITCompilerConfig::clang(const int lang)
{
    switch(lang)
    {
    case (gsJITLang::C) :
        return gsJITCompilerConfig("clang",
                                   "-O3 -shared",
                                   "c",
                                   "-o ");
        break;
    case (gsJITLang::CXX) :
        return gsJITCompilerConfig("clang++",
                                   "-O3 -shared",
                                   "cxx",
                                   "-o ");
        break;
    case (gsJITLang::Fortran) :
        GISMO_ERROR("Error : Clang does not provide any Fortran compiler.");
        break;
    default :
        GISMO_ERROR("Error : Invalid compiler language.");
    }
}

gsJITCompilerConfig gsJITCompilerConfig::gcc(const int lang)
{
    switch(lang)
    {
    case (gsJITLang::C) :
        return gsJITCompilerConfig("gcc",
                                   "-fPIC -O3 -shared",
                                   "c",
                                   "-o ");
        break;
    case (gsJITLang::CXX) :
        return gsJITCompilerConfig("g++",
                                   "-fPIC -O3 -shared",
                                   "cxx",
                                   "-o ");
        break;
    case (gsJITLang::Fortran) :
        return gsJITCompilerConfig("gfortran",
                                   "-fPIC -O3 -shared",
                                   "F90",
                                   "-o ");
        break;
    default :
        GISMO_ERROR("Error : Invalid compiler language.");
    }
}

gsJITCompilerConfig gsJITCompilerConfig::intel(const int lang)
{
    switch(lang)
    {
    case (gsJITLang::C) :
#if         defined(_WIN32)
        return gsJITCompilerConfig("icl",
                                   "/O3 /dll",
                                   "c",
                                   "/Fo");//no space
#       else
        return gsJITCompilerConfig("icc",
                                   "-O3 -shared",
                                   "c",
                                   "-o ");
#       endif
        break;
    case (gsJITLang::CXX) :
#if         defined(_WIN32)
        return gsJITCompilerConfig("icl",
                                   "/O3 /dll",
                                   "c",
                                   "/Fo");//no space
#       else
        return gsJITCompilerConfig("icpc",
                                   "-O3 -shared",
                                   "cxx",
                                   "-o ");
#       endif
        break;
    case (gsJITLang::Fortran) :
#if         defined(_WIN32)
        return gsJITCompilerConfig("ifort",
                                   "/O3 /dll",
                                   "F90",
                                   "/Fo");//no space
#       else
        return gsJITCompilerConfig("ifort",
                                   "-O3 -shared",
                                   "F90",
                                   "-o ");
#       endif
        break;
    default :
        GISMO_ERROR("Error : Invalid compiler language.");
    }
}

gsJITCompilerConfig gsJITCompilerConfig::msvc(const int lang)
{
    switch(lang)
    {
    case (gsJITLang::C) :
        return gsJITCompilerConfig("cl.exe",
                                   "/EHsc /Ox /LD",
                                   "c",
                                   "/Fe");//no space
        break;
    case (gsJITLang::CXX) :
        return gsJITCompilerConfig("cl.exe",
                                   "/EHsc /Ox /LD",
                                   "cxx",
                                   "/Fe");//no space
        break;
    default :
        GISMO_ERROR("Error : Invalid compiler language.");
    }
}

gsJITCompilerConfig gsJITCompilerConfig::nvcc(const int lang)
{
    switch(lang)
    {
    case (gsJITLang::CUDA) :
        return gsJITCompilerConfig("nvcc",
                                   "-O3 -shared",
                                   "cu",
                                   "-o ");
        break;
    default :
        GISMO_ERROR("Error : Invalid compiler language.");
    }
}

gsJITCompilerConfig gsJITCompilerConfig::pgi(const int lang)
{
    switch(lang)
    {
    case (gsJITLang::C) :
        return gsJITCompilerConfig("pgcc",
                                   "-O3 -shared",
                                   "c",
                                   "-o ");
        break;
    case (gsJITLang::CXX) :
        return gsJITCompilerConfig("pgc++",
                                   "-O3 -shared",
                                   "cxx",
                                   "-o ");
        break;
    case (gsJITLang::Fortran) :
        return gsJITCompilerConfig("pgf90",
                                   "-O3 -shared",
                                   "F90",
                                   "-o ");
        break;
    default :
        GISMO_ERROR("Error : Invalid compiler language.");
    }
}

gsJITCompilerConfig gsJITCompilerConfig::sunstudio(const int lang)
{
    switch(lang)
    {
    case (gsJITLang::C) :
        return gsJITCompilerConfig("sunstudio",
                                   "-O3 -shared",
                                   "c",
                                   "-o ");
        break;
    case (gsJITLang::CXX) :
        return gsJITCompilerConfig("sunstudio",
                                   "-O3 -shared",
                                   "cxx",
                                   "-o ");
        break;
    case (gsJITLang::Fortran) :
        return gsJITCompilerConfig("sunstudio",
                                   "-O3 -shared",
                                   "F90",
                                   "-o ");
        break;
    default :
        GISMO_ERROR("Error : Invalid compiler language.");
    }
}

gsJITCompilerConfig gsJITCompilerConfig::guess()
{
#   if defined(__INTEL_COMPILER)
#   if defined(__ICC)
    return intel(gsJITLang::CXX);
#   else
    return intel(gsJITLang::Fortran);
#   endif

#   elif  defined(_MSC_VER)
    return msvc();

#   elif defined(__clang__)
    return clang();

#   elif defined(__GNUC__)
#   if defined(__cplusplus)
    return gcc(gsJITLang::CXX);
#   elif defined(__GFortran__)
    return gcc(gsJITLang::Fortran);
#   else
    return gcc(gsJITLang::C);
#   endif

#   elif defined(__PGIC__)
    return pgi();

#   elif defined(__SUNPRO_C)
    return sunstudio(gsJITLang::C);
#   elif defined(__SUNPRO_CC)
    return sunstudio(gsJITLang::CXX);
#   elif defined(__SUNPRO_F90) || defined(__SUNPRO_F95)
    return sunstudio(gsJITLang::Fortran);

#   else
    GISMO_ERROR("Compiler not known");
#   endif
}

std::string gsJITCompilerConfig::detectTemp()
{
    // Use gsFileManager directly to get temp path
    return gsFileManager::getTempPath();
}

// =====================================================
// gsDynamicLibrary implementations
// =====================================================

gsDynamicLibrary::gsDynamicLibrary(const char* filename, int flag)
{
    gsDebug << "Loading dynamic library: " << filename << "\n";

#if defined(_WIN32)
    GISMO_UNUSED(flag);
    HMODULE dl = LoadLibrary(filename);
    if (!dl)
    {
        std::ostringstream err;
        err <<"LoadLibrary - error: " << GetLastError();
        throw std::runtime_error( err.str() );
    }
    handle.reset(dl, FreeLibrary);
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix)
    void * dl = ::dlopen(filename, flag);
    if (!dl)
        throw std::runtime_error( ::dlerror() );
    handle.reset(dl, ::dlclose);
#else
#error("Unsupported operating system")
#endif
}

// =====================================================
// gsJITCompiler implementations
// =====================================================

gsJITCompiler::gsJITCompiler(gsJITCompiler const& other)
: config(other.config)
{
    kernel << other.kernel.rdbuf();
}

gsJITCompiler& gsJITCompiler::operator=(gsJITCompiler const& other)
{
    kernel << other.kernel.rdbuf();
    config = other.config;
    return *this;
}

#if __cplusplus >= 201103L || _MSC_VER >= 1600
gsJITCompiler::gsJITCompiler(gsJITCompiler && other)
: config(std::move(other.config))
{
    kernel << other.kernel.rdbuf();
}

gsJITCompiler& gsJITCompiler::operator=(gsJITCompiler && other)
{
    kernel << other.kernel.rdbuf();
    config = std::move(other.config);
    return *this;
}
#endif

gsDynamicLibrary gsJITCompiler::build(bool force)
{
#   if __cplusplus >= 201103L || _MSC_VER >= 1600
    size_t h = std::hash<std::string>()(getKernel().str() +
                                        config.getCmd()+config.getFlags() +
                                        config.getLang());
    return build(std::to_string(h), force);
#   else
    return build("JIT", true);
#   endif
}

gsDynamicLibrary gsJITCompiler::build(const std::string &name, bool force)
{
    // Prepare library name
    std::stringstream libName;

#   if   defined(_WIN32)
    libName << config.getTemp() << "." << name << ".dll";
    //(void)std::system("del /f " + libName);
    //force = true;
#   elif defined(__APPLE__)
    libName << config.getTemp() << ".lib" << name << ".dylib";
#   elif defined(unix) || defined(__unix__) || defined(__unix)
    libName << config.getTemp() << ".lib" << name << ".so";
#   else
#   error("Unsupported operating system")
#   endif

    // Compile library (if required)
    std::ifstream libfile(libName.str().c_str());
    if(!libfile || force)
    {
        // Write kernel source code to file
        std::stringstream srcName;
#       ifdef _WIN32
        srcName<< config.getTemp() << "\\." << name << "." << config.getLang();
        std::ofstream file(srcName.str().c_str());
        file << "#ifdef __cplusplus\n";
        file << "#define EXPORT extern \"C\" __declspec(dllexport)\n";
        file << "#endif\n";
#       else
        srcName<< config.getTemp() << "." << name << "." << config.getLang();
        std::ofstream file(srcName.str().c_str());
        file << "#ifdef __cplusplus\n";
        file << "#define EXPORT extern \"C\"\n";
        file << "#endif\n";
#       endif
        file << getKernel().str() <<"\n";
        file.close();

        // Compile kernel source code into library
        std::stringstream systemcall;

#       ifdef _WIN32
        // double quotes are better than single quotes..
        systemcall << "\"\"" << config.getCmd() << "\" "
                   << config.getFlags() << " \""
                   << srcName.str()     << "\" "
                   << config.getOut() << "\"" << libName.str() << "\"\"";
#       else
        systemcall << "\"" << config.getCmd() << "\" "
                   << config.getFlags() << " \""
                   << srcName.str()     << "\" "
                   << config.getOut() << "\"" << libName.str() << "\"";
#       endif

        gsDebug << "Compiling dynamic library: " << systemcall.str() << "\n";
        if(std::system(systemcall.str().c_str()) != 0)
            throw std::runtime_error("An error occured while compiling the kernel source code");
    }

#ifdef _WIN32
    return gsDynamicLibrary( libName.str().c_str(), 0 );
#else
    return gsDynamicLibrary( libName.str().c_str(), RTLD_LAZY );
#endif
}

void gsJITCompiler::clear()
{
    kernel.clear();
    kernel.str(std::string());
}

std::ostream& gsJITCompiler::print(std::ostream &os) const
{
    os << kernel.str();
    return os;
}

// XML serialization method implementations
void gsJITCompilerConfig::load(const std::string filename,
          const int _lang)
{
    GISMO_ENSURE(_lang >= gsJITLang::C && _lang <= gsJITLang::Fortran,
        "Error: Invalid compiler language.");

    gsFileData<real_t> f(filename);
    gsJITCompilerConfig * cc = f.getId<gsJITCompilerConfig>(_lang).release();

    std::swap(*cc, *this);
    if (this->temp.empty()) this->temp=detectTemp();
    delete cc;
}

void gsJITCompilerConfig::load_id(const std::string filename,
             const int id)
{
    gsFileData<real_t> f(filename);
    gsJITCompilerConfig * cc = f.getId<gsJITCompilerConfig>(id).release();

    std::swap(*cc, *this);
    if (this->temp.empty()) this->temp=detectTemp();
    delete cc;
}

} // namespace gismo