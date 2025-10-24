/** @file DomainBase.h

    @brief Minimal domain base class for gsBasis operations

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#pragma once

namespace gismo
{

/**
 * @brief Minimal base class for domain operations needed by gsBasis
 *
 * This abstract base class provides the essential domain operations that
 * gsBasis requires, eliminating the circular dependency between Core/Basis
 * and gsDomain modules.
 *
 * The full gsDomain class inherits from this base and provides the complete
 * domain interface with efficient iterator-based implementations.
 */
template<class T>
class gsDomainBase
{
public:
    virtual ~gsDomainBase() = default;

    /// @brief Get the dimension of the domain
    /// @return The parametric dimension of the domain
    virtual short_t dim() const = 0;

    /// @brief Get the minimum cell length in the domain
    /// @return The minimum cell length across all domain elements
    virtual T getMinCellLength() const = 0;

    /// @brief Get the maximum cell length in the domain
    /// @return The maximum cell length across all domain elements
    virtual T getMaxCellLength() const = 0;

    /// @brief Get the number of elements in the domain
    /// @return Total number of elements in the domain
    virtual size_t numElements() const = 0;

    /// @brief Get the bounding box of the domain
    /// @param result Matrix where result(i,0) = min in dimension i, result(i,1) = max in dimension i
    virtual void boundingBox_into(gsMatrix<T>& result) const = 0;
};

} // namespace gismo