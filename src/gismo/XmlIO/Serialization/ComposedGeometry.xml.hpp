/** @file ComposedGeometry.xml.hpp

    @brief Provides XML serialization for gsComposedGeometry.

    This file contains the XML serialization specialization for gsComposedGeometry.
    It is separated from the main class definition to break circular dependencies
    between Core and gsIO modules.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

// Include the base class first
#include <gismo/Core/Geometry/ComposedGeometry.hpp>

// XML serialization dependencies
#include <gsIO/gsXml.h>
#include <gsIO/gsXmlGenericUtils.hpp>

namespace gismo {

namespace internal
{

/// @brief Get a Tensor BSpline from XML data
///
/// \ingroup Nurbs
template<class T>
class gsXml< gsComposedGeometry<T> >
{
private:
    gsXml() { }
public:
    GSXML_COMMON_FUNCTIONS(gsComposedGeometry<T>);
    GSXML_GET_INTO(gsComposedGeometry<T>);
    static std::string tag ()  { return "Geometry"; }
    static std::string type () { return "ComposedGeometry"; }

    static gsComposedGeometry<T> * get (gsXmlNode * node)
    {
        return getGeometryFromXml< gsComposedGeometry<T> >( node );
    }

    static gsXmlNode * put (const gsComposedGeometry<T> & obj,
                            gsXmlTree & data)
    {
        return putGeometryToXml(obj,data);
    }
};

}// namespace internal

} // namespace gismo