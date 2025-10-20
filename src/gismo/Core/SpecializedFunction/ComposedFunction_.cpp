
#include <gismo/Common/TemplateTools.h>

#include <gismo/Core/SpecializedFunction/ComposedFunction.h>
#include <gismo/Core/SpecializedFunction/ComposedFunction.hpp>

// Include XML serialization when available (breaks circular dependency)
#ifdef GISMO_WITH_XML_SERIALIZATION
#include <gismo/Core/SpecializedFunction/ComposedFunction.xml.hpp>
#endif

namespace gismo
{
CLASS_TEMPLATE_INST gsComposedFunction<real_t>;

// XML template instantiation - only when XML support is enabled
#ifdef GISMO_WITH_XML_SERIALIZATION
CLASS_TEMPLATE_INST internal::gsXml< gsComposedFunction<real_t> >;
#endif

}
