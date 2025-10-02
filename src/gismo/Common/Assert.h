/** @file Assert.h

    @brief Assertion macros for debugging and error checking.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace gismo {

/** @name Assertion Macros
 *  These macros provide assertion functionality for debugging and error checking.
 */

///@{

/// \brief Basic assertion macro with message
#ifdef NDEBUG
#define GISMO_ASSERT(condition, message)
#else
#define GISMO_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << #condition << "\n" \
                      << "Message: " << message << "\n" \
                      << "File: " << __FILE__ << "\n" \
                      << "Line: " << __LINE__ << std::endl; \
            assert(false); \
        } \
    } while(0)
#endif

/// \brief Ensure condition is true, throw exception if not
#define GISMO_ENSURE(condition, message) \
    do { \
        if (!(condition)) { \
            std::ostringstream oss; \
            oss << "Condition failed: " << #condition << "\n" \
                << "Message: " << message << "\n" \
                << "File: " << __FILE__ << "\n" \
                << "Line: " << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    } while(0)

/// \brief Always throw an error with message
#define GISMO_ERROR(message) \
    do { \
        std::ostringstream oss; \
        oss << "Error: " << message << "\n" \
            << "File: " << __FILE__ << "\n" \
            << "Line: " << __LINE__; \
        throw std::runtime_error(oss.str()); \
    } while(0)

/// \brief Static assertion macro
#if __cplusplus >= 201103L
#define GISMO_STATIC_ASSERT(condition, message) static_assert(condition, message)
#else
template<bool> struct gismo_static_assert_failure;
template<> struct gismo_static_assert_failure<true> { enum { value = 1 }; };
#define GISMO_STATIC_ASSERT(condition, message) \
    typedef char GISMO_JOIN(gismo_static_assertion_failed_at_line_, __LINE__) \
        [gismo_static_assert_failure<(bool)(condition)>::value]
#endif

/// \brief Helper macro for joining tokens
#define GISMO_JOIN_IMPL(x, y) x##y
#define GISMO_JOIN(x, y) GISMO_JOIN_IMPL(x, y)

/// \brief Assertion for array bounds checking
#define GISMO_ASSERT_BOUNDS(index, size) \
    GISMO_ASSERT((index) >= 0 && (index) < (size), \
                 "Index " << (index) << " out of bounds [0, " << (size) << ")")

/// \brief Assertion for non-null pointer
#define GISMO_ASSERT_NOT_NULL(ptr) \
    GISMO_ASSERT((ptr) != nullptr, "Pointer must not be null")

///@}

} // namespace gismo