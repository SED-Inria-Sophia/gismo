/** @file ForwardDeclarations.h

    @brief Provides forward declarations of basic types.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

// STD includes
#include <vector>
#include <iterator>
#include <set>
#include <map>
#include <stack>
#include <algorithm>
#include <functional>
#include <limits>

// Basic type definitions will be in Types.h
// Eigen forward declarations will be in EigenDeclarations.h
// Memory utilities in Memory.h
// Debug utilities in Debug.h

namespace gismo
{

/**
    \brief Enumeration flags which define the needs of an evaluation object.

    \enum gsNeedEnum

    \ingroup enums
*/
enum gsNeedEnum
{
    NEED_VALUE             = 1U << 0, ///< Value of the object
    NEED_DERIV             = 1U << 1, ///< Gradient of the object
    NEED_GRAD              = NEED_DERIV,
    NEED_JACOBIAN          = NEED_DERIV, ///< Jacobian of the object
    NEED_MEASURE           = 1U << 3, ///< The density of the measure pull back
    NEED_GRAD_TRANSFORM    = 1U << 4, ///< Gradient transformation matrix
    NEED_DIV               = 1U << 5, ///< Div operator
    NEED_CURL              = 1U << 6, ///< Curl operator
    NEED_DERIV2            = 1U << 7, ///< Second derivatives
    NEED_2ND_DER           = NEED_DERIV2,
    NEED_HESSIAN           = NEED_DERIV2, ///< Hessian matrix
    NEED_LAPLACIAN         = 1U << 9, ///< Laplacian
    NEED_ACTIVE            = 1U <<10, ///< Active function ids
    NEED_NORMAL            = 1U <<11, ///< Normal vector of the object
    NEED_OUTER_NORMAL      = 1U <<12, ///< Outward normal on the boundary
    NEED_2ND_FFORM         = 1U <<13, ///< Second fundamental form

    SAME_ELEMENT           = 1U <<15  ///< Enable optimizations based on the assumption that all evaluation points are in the same bezier domain
};

} // namespace gismo