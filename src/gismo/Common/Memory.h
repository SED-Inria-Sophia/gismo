/** @file Memory.h

    @brief Provides utility function related to memory management.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): C. Hofreither, A. Mantzaflaris, J. Vogl
*/

#pragma once

#include <memory>

namespace gismo {

/** @namespace gismo::memory

    @brief
    This namespace contains functions related to memory management.

    \ingroup Core
*/
namespace memory
{

/* \brief Adaptor for a shared pointer

usage:
\code
memory::shared_ptr<int> B;
\endcode
*/

using std::shared_ptr;
using std::weak_ptr;

/* \brief Adaptor for a unique pointer

usage:
\code
memory::unique_ptr<int> B;
\endcode
*/

using std::unique_ptr;
using std::nullptr_t;

} // namespace memory

} // namespace gismo