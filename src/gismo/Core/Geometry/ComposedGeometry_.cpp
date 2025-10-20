
#include <gismo/Common/TemplateTools.h>

#include <gismo/Core/Geometry/ComposedGeometry.h>

// Include XML serialization when available (breaks circular dependency)
#ifdef GISMO_WITH_XML_SERIALIZATION
#include <gismo/Core/Geometry/ComposedGeometry.xml.hpp>
#endif

namespace gismo
{
CLASS_TEMPLATE_INST gsComposedGeometry<real_t>;

// XML template instantiation - only when XML support is enabled
#ifdef GISMO_WITH_XML_SERIALIZATION
CLASS_TEMPLATE_INST internal::gsXml< gsComposedGeometry<real_t> >;
#endif

}
