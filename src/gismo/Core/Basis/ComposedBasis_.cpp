
#include <gismo/Common/TemplateTools.h>

#include <gismo/Core/Basis/ComposedBasis.h>

namespace gismo
{
CLASS_TEMPLATE_INST gsComposedBasis<real_t>;

CLASS_TEMPLATE_INST internal::gsXml< gsComposedBasis<real_t> >;

}
