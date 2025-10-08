
#include <gismo/Common/TemplateTools.h>

#include <gismo/Core/SpecializedFunction/ComposedGeometry.h>
#include <gismo/Core/SpecializedFunction/ComposedGeometry.hpp>

namespace gismo
{
CLASS_TEMPLATE_INST gsComposedGeometry<real_t>;

CLASS_TEMPLATE_INST internal::gsXml< gsComposedGeometry<real_t> >;

}
