/** @file XmlTypes.h

    @brief XML type definitions and includes for GISMO.

    This file centralizes XML-related types used throughout the GISMO library.
    It includes RapidXML headers and defines GISMO's XML type aliases for
    consistent usage across modules.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#pragma once

// Include RapidXML headers with proper visibility handling
#define private public
#define protected public
#include <rapidxml/rapidxml.hpp>
#undef private
#undef protected

namespace rapidxml { namespace internal {
    template<class OutIt, class Ch>
    OutIt print_children(OutIt out, const xml_node<Ch> *node, int flags, int indent);
    template<class OutIt, class Ch>
    OutIt print_element_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
    template<class OutIt, class Ch>
    OutIt print_data_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
    template<class OutIt, class Ch>
    OutIt print_cdata_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
    template<class OutIt, class Ch>
    OutIt print_declaration_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
    template<class OutIt, class Ch>
    OutIt print_comment_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
    template<class OutIt, class Ch>
    OutIt print_doctype_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
    template<class OutIt, class Ch>
    OutIt print_pi_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
} }

#include <rapidxml/rapidxml_print.hpp>

namespace gismo {
namespace internal {

/// Primary XML node type used throughout GISMO
typedef rapidxml::xml_node<char> gsXmlNode;

/// XML attribute type used throughout GISMO
typedef rapidxml::xml_attribute<char> gsXmlAttribute;

/// XML document type used throughout GISMO
typedef rapidxml::xml_document<char> gsXmlDocument;

// Forward declaration of GISMO XML tree class
class gsXmlTree;

} // namespace internal
} // namespace gismo