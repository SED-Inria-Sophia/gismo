
#pragma once

#include <gismo/IO/OptionList.h>
#include <gismo/IO/Xml.h>

namespace gismo
{

namespace internal
{

/** \brief Read OptionList from XML data
    \ingroup IO
*/
template<>
class GISMO_EXPORT gsXml<gsOptionList>
{
private:
    gsXml();
public:
    GSXML_COMMON_FUNCTIONS(gsOptionList)
    GSXML_GET_POINTER(gsOptionList)
    static std::string tag () { return "OptionList"; }
    static std::string type() { return ""; }

    static void get_into(gsXmlNode * node, gsOptionList & result);
    static gsXmlNode * put (const gsOptionList & obj, gsXmlTree & data);
};

} // namespace internal

}
