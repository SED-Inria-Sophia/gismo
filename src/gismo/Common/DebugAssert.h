/** @file DebugAssert.h

    @brief Debug messages and assertion macros for debugging and error checking.

    This file contains the debugging, messaging system, and assertion macros
    of G+Smo. It merges the functionality from Debug.h and GismoAssert.h
    into a unified debugging and assertion system.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

// Start DEBUG_GROUP of Doxygen
/** @{ */

#pragma once

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <typeinfo>

// See also about memory leak detection:
// http://msdn.microsoft.com/en-us/library/e5ewb1h3%28v=vs.90%29.aspx
#if _MSC_VER //>= 1400
#include <crtdbg.h>
//#include <errno.h>
#endif

//#ifdef GISMO_WITH_XDEBUG
//  #include <misc/gsStackWalker.h>
//#endif

namespace gismo {

// =====================================================================
// LOGGING AND DEBUG MESSAGES
// =====================================================================

/** Logging messages:
 *  gsInfo is ment to be the standard output stream, like for the output of the
 *  executables. In general, the library should not write to gsInfo.
 */
#define gsInfo std::cout

/** Logging messages:
 *  gsWarn is for warnings, eg, for missing functionality or problem in the input.
 *
 *  Note that gsWarn cannot be given as a parameter to another function.
 */
#define gsWarn std::cout<<"Warning: "

/** Logging messages:
 *  gsDebug is meant for debugging. The information is useful for understanding the
 *  control flow of the code. In general, the library should not write to gsDebug.
 *
 *  Note that gsDebug cannot be given as a parameter to another function.
 */
#ifdef NDEBUG
#define gsDebug if (0) std::cout
#else
#define gsDebug std::cout<<"Debug: "
#endif

/** Logging messages:
 *  gsDebugVar(variable) will output "variable = <value of variable>" to the debug stream.
 *
 *  Note that gsDebugVar cannot be given as a parameter to another function.
 */
#ifdef NDEBUG
#define gsDebugVar(variable)
#else
#define gsDebugVar(variable) gsDebug<< (void*)&variable <<" "<< #variable <<" = "<< variable <<"\n"
#endif

// =====================================================================
// ASSERTION MACROS
// =====================================================================

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

#ifndef GISMO_BUILD_LIB
#include <gismo/Common/Debug.hpp>
#endif

} //namespace gismo


/* Debugging extras

   Optional debugging functionality may be included with

   #include <gsDebugExtra.h>

   or by compiling with -DGISMO_EXTRA_DEBUG.
*/
#if defined(GISMO_EXTRA_DEBUG)
# include <gsCore/gsDebugExtra.h>
#endif

/** @} */ // end DEBUG_GROUP of Doxygen