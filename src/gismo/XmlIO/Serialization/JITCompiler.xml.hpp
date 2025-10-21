/** @file JITCompiler.xml.hpp

    @brief XML serialization for JITCompiler classes.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): M. Moeller
*/

#pragma once

#ifdef GISMO_WITH_XML_SERIALIZATION

#include <gismo/Core/Utilities/JITCompiler.h>
#include <gsIO/gsXml.h>
#include <gsIO/gsFileManager.h>

namespace gismo
{

namespace internal
{

/** \brief Read a JITCompilerConfig from XML data
    \ingroup Core
*/
template<>
class gsXml< gsJITCompilerConfig >
{
private:
    gsXml() { }

public:
    GSXML_COMMON_FUNCTIONS(gsJITCompilerConfig)
    GSXML_GET_POINTER(gsJITCompilerConfig)
    static std::string tag () { return "JITCompilerConfig"; }
    static std::string type() { return ""; }

    static void get_into(gsXmlNode * node, gsJITCompilerConfig & result)
    {
        gsXmlAttribute * tmp = node->first_attribute("cmd");
        if (tmp!=NULL)
            result.setCmd(tmp->value());

        tmp = node->first_attribute("flags");
        if (tmp!=NULL)
            result.setFlags(tmp->value());

        tmp = node->first_attribute("lang");
        if (tmp!=NULL)
            result.setLang(tmp->value());

        tmp = node->first_attribute("out");
        if (tmp!=NULL)
            result.setOut(tmp->value());

        tmp = node->first_attribute("temp");
        if (tmp!=NULL)
            result.setTemp(tmp->value());
    }

    static gsXmlNode * put (const gsJITCompilerConfig & obj, gsXmlTree & data)
    {
        // Make a new XML CompilerConfig node
        gsXmlNode * tmp = internal::makeNode("JITCompilerConfig", data);

        // Append the attributes
        tmp->append_attribute( makeAttribute("cmd"  , obj.getCmd()  , data) );
        tmp->append_attribute( makeAttribute("flags", obj.getFlags(), data) );
        tmp->append_attribute( makeAttribute("lang" , obj.getLang() , data) );
        tmp->append_attribute( makeAttribute("out"  , obj.getOut()  , data) );
        tmp->append_attribute( makeAttribute("temp" , obj.getTemp() , data) );

        return tmp;
    }
};

/** \brief Read a JITCompiler from XML data
    \ingroup Core
*/
template<>
class gsXml< gsJITCompiler >
{
private:
    gsXml() { }

public:
    GSXML_COMMON_FUNCTIONS(gsJITCompiler)
    GSXML_GET_POINTER(gsJITCompiler)
    static std::string tag () { return "JITCompiler"; }
    static std::string type() { return ""; }

    static void get_into(gsXmlNode * node, gsJITCompiler & result)
    {
        result.getKernel() << node->value();
    }

    static gsXmlNode * put (const gsJITCompiler & obj, gsXmlTree & data)
    {
        // Make a new XML KnotVector node
        gsXmlNode * tmp = internal::makeNode("JITCompiler", obj.getKernel().str(), data);

        return tmp;
    }
};

} // namespace internal

// Member function implementations that depend on XML
inline void gsJITCompilerConfig::load(const std::string filename,
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

inline void gsJITCompilerConfig::load_id(const std::string filename,
             const int id)
{
    gsFileData<real_t> f(filename);
    gsJITCompilerConfig * cc = f.getId<gsJITCompilerConfig>(id).release();

    std::swap(*cc, *this);
    if (this->temp.empty()) this->temp=detectTemp();
    delete cc;
}

} // namespace gismo

#endif // GISMO_WITH_XML_SERIALIZATION