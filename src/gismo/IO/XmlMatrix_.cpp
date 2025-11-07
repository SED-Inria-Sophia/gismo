/** @file XmlMatrix_.cpp

    @brief Explicit instantiations for XML matrix serialization

    This file contains explicit template instantiations for matrix XML
    serialization classes to ensure they are available in the compiled library.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#include <gismo/Common/TemplateTools.h>

// Include all matrix XML headers
#include <gismo/IO/XmlMatrix.h>
#include <gismo/IO/XmlSparseMatrix.h>

namespace gismo {
namespace internal {

// =============================================================================
// Explicit Template Instantiations for XML Matrix Classes
// =============================================================================

TEMPLATE_INST class XmlMatrix<real_t>;
TEMPLATE_INST class XmlMatrix<index_t>;

TEMPLATE_INST class XmlSparseMatrix<real_t>;
TEMPLATE_INST class XmlSparseMatrix<index_t>;
TEMPLATE_INST class XmlSparseMatrix<bool>;

// =============================================================================
// Explicit Template Instantiations for gsXml Matrix Classes
// =============================================================================

TEMPLATE_INST class gsXml< gsMatrix<real_t> >;
TEMPLATE_INST class gsXml< gsMatrix<index_t> >;

TEMPLATE_INST class gsXml< gsSparseMatrix<real_t> >;
TEMPLATE_INST class gsXml< gsSparseMatrix<index_t> >;
TEMPLATE_INST class gsXml< gsSparseMatrix<bool> >;

} // namespace internal
} // namespace gismo