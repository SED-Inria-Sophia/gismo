/** @file JITCompiler.h

    @brief Provides declaration of JIT-compiler class.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): M. Moeller

    @note This class is based on the discussion on
    http://stackoverflow.com/questions/36040814/stdshared-ptr-and-dlopen-avoiding-undefined-behavior
*/

#pragma once

#include <gismo/IO/Xml.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <gismo/Common/Memory.h>
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace gismo {

/**
   @brief Supported languages
*/
struct gsJITLang
{
    enum {
        C       = 0, ///< C
        CXX     = 1, ///< C++
        CUDA    = 2, ///< Cuda
        Fortran = 3  ///< Fortran
    };
};

/**
   @brief Struct definig a compiler configuration

   This class defines a compiler configuration that is used by the
   \ref gsJITCompiler class to perform just-in-time compilation.
*/
struct gsJITCompilerConfig
{
    /// Constructor (default)
    gsJITCompilerConfig()
    : cmd("missing"), flags("missing"), lang("missing"), out("-o "), temp(detectTemp())
    {
        char *env;
        env = getenv ("JIT_COMPILER_CMD");
        if(env!=NULL) cmd = env;

        env = getenv ("JIT_COMPILER_FLAGS");
        if(env!=NULL) flags = env;

        env = getenv ("JIT_COMPILER_LANG");
        if(env!=NULL) lang = env;

        env = getenv ("JIT_COMPILER_TEMP");
        if(env!=NULL) temp = env;
    }

    virtual ~gsJITCompilerConfig() { }

    /// Constructor (passing arguments as strings)
    gsJITCompilerConfig(const std::string& cmd,
                        const std::string& flags,
                        const std::string& lang,
                        const std::string& out,
                        const std::string& temp = detectTemp())
    : cmd(cmd), flags(flags), lang(lang), out(out), temp(temp)
    {}

    void swap(gsJITCompilerConfig & other)
    {
        std::swap(cmd  , other.cmd  );
        std::swap(flags, other.flags);
        std::swap(lang , other.lang );
        std::swap(out  , other.out  );
        std::swap(temp , other.temp );
    }

#   if __cplusplus >= 201103L || _MSC_VER >= 1600

    /// Constructor (copy)
    gsJITCompilerConfig(gsJITCompilerConfig const& other)
    { operator=(other);}

    /// Assignment operator
    gsJITCompilerConfig& operator=(const gsJITCompilerConfig & other)
    {
        cmd   = other.cmd;
        flags = other.flags;
        lang  = other.lang;
        out   = other.out;
        temp  = other.temp;
        return *this;
    }

    /// Constructor (move)
    gsJITCompilerConfig(gsJITCompilerConfig && other)
    : cmd(std::move(other.cmd)), flags(std::move(other.flags)),
      lang(std::move(other.lang)), out(std::move(other.out)),
      temp(std::move(other.temp))
    {}

    /// Assignment operator (move)
    gsJITCompilerConfig& operator=(gsJITCompilerConfig && other)
    {
        cmd   = std::move(other.cmd);
        flags = std::move(other.flags);
        lang  = std::move(other.lang);
        out   = std::move(other.out);
        temp  = std::move(other.temp);
        return *this;
    }
#else
    /// Assignment operator
    gsJITCompilerConfig& operator=(gsJITCompilerConfig other)
    {
        this->swap(other);
        return *this;
    }
#   endif

    /// Return compiler command
    virtual const std::string& getCmd() const { return cmd; }

    /// Return compiler flags
    virtual const std::string& getFlags() const { return flags; }

    /// Return compiler language
    virtual const std::string& getLang() const { return lang; }

    /// Return compiler output flag
    virtual const std::string& getOut() const { return out; }

    /// Return compiler temporal directory
    virtual const std::string& getTemp() const { return temp; }

    /// Set compiler command
    void setCmd(const std::string& _cmd)
    { this->cmd = _cmd; }

    /// Set compiler flags
    void setFlags(const std::string& _flags)
    { this->flags = _flags; }

    /// Set compiler language
    void setLang(const std::string& _lang)
    { this->lang = _lang; }

    /// Set compiler output flag
    void setOut(const std::string& _out)
    { this->out = _out; }

    /// Set compiler temporal directory
    void setTemp(const std::string& _temp)
    { this->temp = _temp; }

    /// Prints the object as a string
    std::ostream& print(std::ostream &os) const
    {
        os << "JIT Compiler.\n"
           << "  cmd:                " << cmd << "\n"
           << "  flags:              " << flags << "\n"
           << "  language:           " << lang << "\n"
           << "  output flag:        " << out << "\n"
           << "  temporal directory: " << temp << "\n";

        return os;
    }

    /// Reads compiler configuration from XML file by language
    void load(const std::string filename,
              const int _lang = gsJITLang::CXX);

    /// Reads compiler configuration from XML file by ID
    void load_id(const std::string filename,
                 const int id);

    /// Initialize to default Clang compiler
    static gsJITCompilerConfig clang(const int lang = gsJITLang::CXX);

    /// Initialize to default GCC compiler
    static gsJITCompilerConfig gcc(const int lang = gsJITLang::CXX);

    /// Initialize to default Intel compiler
    static gsJITCompilerConfig intel(const int lang = gsJITLang::CXX);

    /// Initialize to default Microsoft Visual Studio compiler
    static gsJITCompilerConfig msvc(const int lang = gsJITLang::CXX);

    /// Initialize to default NVIDIA nvcc compiler
    static gsJITCompilerConfig nvcc(const int lang = gsJITLang::CUDA);

    /// Initialize to default PGI compiler
    static gsJITCompilerConfig pgi(const int lang = gsJITLang::CXX);

    /// Initialize to default Oracle/SunStudio compiler
    static gsJITCompilerConfig sunstudio(const int lang = gsJITLang::CXX);

    /// Try to initialize compiler automatically based on the context
    static gsJITCompilerConfig guess();

protected:
    /// Members variables
    std::string cmd;
    std::string flags;
    std::string lang;
    std::string out;
    std::string temp;

private:

    /// Auto-detect temp directory
    static std::string detectTemp();
};

/// Print (as string) operator to be used by all derived classes
inline std::ostream &operator<<(std::ostream &os,
                                const gsJITCompilerConfig& c)
{ return c.print(os); }

// XML serialization specializations in JITCompiler.hpp

/**
   @brief Class defining a dynamic library.

   This class stores a pointer to a dynamic library that can be
   compiled at runtime and provides extra functionality.
 */
struct gsDynamicLibrary
{
public:
    /// Default Constructor
    gsDynamicLibrary() {}

    /// Constructor (using file name)
    gsDynamicLibrary(const char* filename, int flag);

    /// Get symbol from dynamic library
    template<class T>
    T* getSymbol(const char* name) const
    {
        if (!handle)
            throw std::runtime_error("An error occured while accessing the dynamic library");

        T *symbol;
#if defined(_WIN32)
        *(void **)(&symbol) = (void*)GetProcAddress(handle.get(), name );
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix)
        *(void **)(&symbol) = ::dlsym( handle.get(), name );
#endif
        if (!symbol)
            throw std::runtime_error("An error occured while getting symbol from the dynamic library");

        return symbol;
    }

    /// Check if handle is assigned
    operator bool() const { return (bool)handle; }

private:

    /// Handle to dynamic library object
#if defined(_WIN32)
    memory::shared_ptr< util::remove_pointer<HMODULE>::type > handle;
#else //if defined(__APPLE__) || defined(__linux__) || defined(__unix)
    memory::shared_ptr<void> handle;
#endif
};

/**
   @brief Class defining a just-in-time compiler.

   This class compiles source code at runtime and links it to the
   running binary. This mechanism makes it possible to generate source
   code based on user-defined run-time parameters and still perform
   compile-time optimization.
*/
class gsJITCompiler
{
public:
    /// Constructor (default)
    gsJITCompiler()
    : kernel(), config()
    { }

    /// Constructor (copy)
    gsJITCompiler(gsJITCompiler const& other);

    /// Constructor (using compiler configuration)
    explicit gsJITCompiler(const gsJITCompilerConfig & config)
    : kernel(), config(config)
    {}

    /// Assignment operator (copy)
    gsJITCompiler& operator=(gsJITCompiler const& other);

#   if __cplusplus >= 201103L || _MSC_VER >= 1600
    /// Constructor (move)
    gsJITCompiler(gsJITCompiler && other);

    /// Assignment operator (move)
    gsJITCompiler& operator=(gsJITCompiler && other);
#   endif

    /// Input kernel source code from string
    gsJITCompiler & operator<<(const std::string & s)
    {
        kernel << s;
        return *this;
    }

    /// Input kernel source code from input stream
    gsJITCompiler & operator<<(std::istream & is)
    {
        kernel << is.rdbuf();
        return *this;
    }

    /// Compile kernel source code into dynamic library
    /// (determine filename from hash of kernel source code)
    gsDynamicLibrary build(bool force = false);

    /// Compile kernel source code into dynamic library
    /// (use given filename)
    gsDynamicLibrary build(const std::string &name, bool force = false);

    /// Clear kernel source code
    void clear();

    /// Prints the object as a string
    std::ostream& print(std::ostream &os) const;

    /// Return kernel source code (as output stringstream)
    const std::ostringstream &getKernel() const
    {
        return kernel;
    }

    /// Return pointer to kernel source code
    std::ostringstream &getKernel()
    {
        return kernel;
    }

private:
    /// Kernel source code
    std::ostringstream kernel;

    /// Compiler configuration
    gsJITCompilerConfig config;
};

/// Print (as string) operator to be used by all derived classes
inline std::ostream &operator<<(std::ostream &os, const gsJITCompiler& c)
{ return c.print(os); }

} // namespace gismo

#include <gismo/JIT/JITCompiler.hpp>
