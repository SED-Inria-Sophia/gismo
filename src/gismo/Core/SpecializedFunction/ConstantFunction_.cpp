#include <gismo/Common/TemplateTools.h>

#include <gismo/Core/SpecializedFunction/ConstantFunction.h>
#include <gismo/Core/SpecializedFunction/ConstantFunction.hpp>


namespace gismo
{

CLASS_TEMPLATE_INST gsConstantFunction<real_t> ;
CLASS_TEMPLATE_INST internal::gsXml< gsConstantFunction<real_t> >;

}
