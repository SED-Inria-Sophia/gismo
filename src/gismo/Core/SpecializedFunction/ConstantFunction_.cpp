#include <gismo/Common/TemplateTools.h>

#include <gismo/Core/SpecializedFunction/ConstantFunction.h>

// Include XML serialization when available (breaks circular dependency)
#ifdef GISMO_WITH_XML_SERIALIZATION
#include <gismo/Core/SpecializedFunction/ConstantFunction.xml.hpp>
#endif

namespace gismo
{

CLASS_TEMPLATE_INST gsConstantFunction<real_t> ;

// XML template instantiation - only when XML support is enabled
#ifdef GISMO_WITH_XML_SERIALIZATION
CLASS_TEMPLATE_INST internal::gsXml< gsConstantFunction<real_t> >;
#endif

}
