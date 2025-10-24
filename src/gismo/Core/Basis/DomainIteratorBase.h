/** @file DomainIteratorBase.h

    @brief Provides minimal base class for domain iterators needed by Core module.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#pragma once

#include <gismo/Common/Memory.h>

namespace gismo
{

/**
 * @brief Minimal base class for domain iterators needed by Core module
 *
 * This abstract base class provides the essential domain iterator operations
 * that Core/Mesh requires for the original mesh construction algorithm,
 * eliminating the circular dependency between Core and Domain modules.
 *
 * The full gsDomainIterator classes inherit from this base and provide
 * the complete iterator interface with efficient implementations.
 */
template<class T>
class gsDomainIteratorBase
{
public:
    virtual ~gsDomainIteratorBase() = default;

    /// @brief Advance to the next element
    /// @return Reference to this iterator (for chaining)
    virtual gsDomainIteratorBase<T>& operator++() = 0;

    /// @brief Check if this iterator is not equal to another
    /// @param other Iterator to compare with
    /// @return true if iterators point to different elements
    virtual bool operator!=(const gsDomainIteratorBase<T>& other) const = 0;

    /// @brief Check if this iterator is less than another (for loop termination)
    /// @param other Iterator to compare with
    /// @return true if this iterator comes before other
    virtual bool operator<(const gsDomainIteratorBase<T>& other) const = 0;

    /// @brief Get the lower corner of the current element
    /// @return Vector containing the lower corner coordinates
    virtual const gsVector<T>& lowerCorner() const = 0;

    /// @brief Get the upper corner of the current element
    /// @return Vector containing the upper corner coordinates
    virtual const gsVector<T>& upperCorner() const = 0;

    /// @brief Get the volume/measure of the current element
    /// @return The volume (area in 2D, length in 1D) of the current element
    virtual T volume() const = 0;

    /// @brief Check if iterator is valid (points to a valid element)
    /// @return true if iterator is valid, false if at end
    virtual bool isValid() const = 0;

    /// @brief Create a copy of this iterator (for polymorphic copying)
    /// @return Smart pointer to a copy of this iterator
    virtual memory::unique_ptr<gsDomainIteratorBase<T>> clone() const = 0;
};

/**
 * @brief Wrapper class to hold polymorphic domain iterators
 *
 * This class allows the Core module to work with domain iterators
 * through the base interface while maintaining value semantics.
 */
template<class T>
class gsDomainIteratorWrapper
{
private:
    memory::unique_ptr<gsDomainIteratorBase<T>> m_iter;

public:
    /// @brief Constructor from base iterator
    explicit gsDomainIteratorWrapper(memory::unique_ptr<gsDomainIteratorBase<T>> iter)
        : m_iter(std::move(iter)) {}

    /// @brief Copy constructor
    gsDomainIteratorWrapper(const gsDomainIteratorWrapper<T>& other)
        : m_iter(other.m_iter ? other.m_iter->clone() : nullptr) {}

    /// @brief Assignment operator
    gsDomainIteratorWrapper<T>& operator=(const gsDomainIteratorWrapper<T>& other)
    {
        if (this != &other) {
            m_iter = other.m_iter ? other.m_iter->clone() : nullptr;
        }
        return *this;
    }

    /// @brief Move constructor
    gsDomainIteratorWrapper(gsDomainIteratorWrapper<T>&& other) noexcept
        : m_iter(std::move(other.m_iter)) {}

    /// @brief Move assignment
    gsDomainIteratorWrapper<T>& operator=(gsDomainIteratorWrapper<T>&& other) noexcept
    {
        if (this != &other) {
            m_iter = std::move(other.m_iter);
        }
        return *this;
    }

    /// @brief Advance to next element
    gsDomainIteratorWrapper<T>& operator++()
    {
        if (m_iter) ++(*m_iter);
        return *this;
    }

    /// @brief Inequality comparison
    bool operator!=(const gsDomainIteratorWrapper<T>& other) const
    {
        if (!m_iter || !other.m_iter) return m_iter != other.m_iter;
        return *m_iter != *(other.m_iter);
    }

    /// @brief Less-than comparison
    bool operator<(const gsDomainIteratorWrapper<T>& other) const
    {
        if (!m_iter) return other.m_iter != nullptr;
        if (!other.m_iter) return false;
        return *m_iter < *(other.m_iter);
    }

    /// @brief Access iterator interface
    const gsDomainIteratorBase<T>& operator*() const { return *m_iter; }
    const gsDomainIteratorBase<T>* operator->() const { return m_iter.get(); }
};

} // namespace gismo