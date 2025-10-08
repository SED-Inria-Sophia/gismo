
#include <gismo/Common/TemplateTools.h>

#include <gismo/Core/SpecializedFunction/ComposedFunction.h>
#include <gismo/Core/SpecializedFunction/ComposedFunction.hpp>

namespace gismo
{
CLASS_TEMPLATE_INST gsComposedFunction<real_t>;

CLASS_TEMPLATE_INST internal::gsXml< gsComposedFunction<real_t> >;

}
