/** @file Macros.h

    @brief Utility macros for general use throughout GISMO.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

/** @name Utility Macros
 *  General utility macros used throughout GISMO library.
 */

///@{

/// Convert macro argument to string
#define GISMO_STRINGIFY(x) #x
#define GISMO_TOSTRING(x) GISMO_STRINGIFY(x)

/// Concatenate two tokens
#define GISMO_CONCAT_IMPL(x, y) x##y
#define GISMO_CONCAT(x, y) GISMO_CONCAT_IMPL(x, y)

/// Create unique identifier by appending line number
#define GISMO_UNIQUE_ID(prefix) GISMO_CONCAT(prefix, __LINE__)

/// Unused parameter marker to avoid compiler warnings
#define GISMO_UNUSED(x) ((void)(x))

/// Mark a variable as potentially unused
#if defined(__GNUC__)
#define GISMO_MAYBE_UNUSED __attribute__((unused))
#elif defined(_MSC_VER)
#define GISMO_MAYBE_UNUSED __pragma(warning(suppress: 4100))
#else
#define GISMO_MAYBE_UNUSED
#endif

/// Force inline (compiler-specific)
#if defined(__GNUC__)
#define GISMO_FORCE_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define GISMO_FORCE_INLINE __forceinline
#else
#define GISMO_FORCE_INLINE inline
#endif

/// No inline hint (compiler-specific)
#if defined(__GNUC__)
#define GISMO_NO_INLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define GISMO_NO_INLINE __declspec(noinline)
#else
#define GISMO_NO_INLINE
#endif

/// Likely/unlikely branch prediction hints
#if defined(__GNUC__)
#define GISMO_LIKELY(x)   __builtin_expect(!!(x), 1)
#define GISMO_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define GISMO_LIKELY(x)   (x)
#define GISMO_UNLIKELY(x) (x)
#endif

/// Compiler-specific alignment
#if defined(__GNUC__)
#define GISMO_ALIGN(n) __attribute__((aligned(n)))
#elif defined(_MSC_VER)
#define GISMO_ALIGN(n) __declspec(align(n))
#else
#define GISMO_ALIGN(n)
#endif

/// Disable specific warnings (compiler-specific)
#if defined(__GNUC__)
#define GISMO_DISABLE_WARNING_PUSH           _Pragma("GCC diagnostic push")
#define GISMO_DISABLE_WARNING_POP            _Pragma("GCC diagnostic pop")
#define GISMO_DISABLE_WARNING(warningName)   _Pragma(GISMO_STRINGIFY(GCC diagnostic ignored warningName))
#elif defined(_MSC_VER)
#define GISMO_DISABLE_WARNING_PUSH           __pragma(warning(push))
#define GISMO_DISABLE_WARNING_POP            __pragma(warning(pop))
#define GISMO_DISABLE_WARNING(warningNumber) __pragma(warning(disable: warningNumber))
#else
#define GISMO_DISABLE_WARNING_PUSH
#define GISMO_DISABLE_WARNING_POP
#define GISMO_DISABLE_WARNING(warningNameOrNumber)
#endif

/// Version checking macros
#define GISMO_VERSION_CHECK(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))

/// Compiler version checking
#if defined(__GNUC__)
#define GISMO_GCC_VERSION GISMO_VERSION_CHECK(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#endif

#if defined(__clang__)
#define GISMO_CLANG_VERSION GISMO_VERSION_CHECK(__clang_major__, __clang_minor__, __clang_patchlevel__)
#endif

#if defined(_MSC_VER)
#define GISMO_MSVC_VERSION _MSC_VER
#endif

/// C++ standard version checking
#define GISMO_CXX98_OR_LATER (__cplusplus >= 199711L)
#define GISMO_CXX11_OR_LATER (__cplusplus >= 201103L)
#define GISMO_CXX14_OR_LATER (__cplusplus >= 201402L)
#define GISMO_CXX17_OR_LATER (__cplusplus >= 201703L)
#define GISMO_CXX20_OR_LATER (__cplusplus >= 202002L)

/// Platform detection macros
#if defined(_WIN32) || defined(_WIN64)
#define GISMO_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
#define GISMO_PLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#define GISMO_PLATFORM_MACOS 1
#elif TARGET_OS_IPHONE
#define GISMO_PLATFORM_IOS 1
#endif
#elif defined(__linux__)
#define GISMO_PLATFORM_LINUX 1
#elif defined(__unix__) || defined(__unix)
#define GISMO_PLATFORM_UNIX 1
#endif

/// Architecture detection
#if defined(__x86_64__) || defined(_M_X64)
#define GISMO_ARCH_X86_64 1
#elif defined(__i386) || defined(_M_IX86)
#define GISMO_ARCH_X86_32 1
#elif defined(__aarch64__) || defined(_M_ARM64)
#define GISMO_ARCH_ARM64 1
#elif defined(__arm__) || defined(_M_ARM)
#define GISMO_ARCH_ARM32 1
#endif

/// Deprecated attribute
#if GISMO_CXX14_OR_LATER
#define GISMO_DEPRECATED [[deprecated]]
#define GISMO_DEPRECATED_MSG(msg) [[deprecated(msg)]]
#elif defined(__GNUC__)
#define GISMO_DEPRECATED __attribute__((deprecated))
#define GISMO_DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define GISMO_DEPRECATED __declspec(deprecated)
#define GISMO_DEPRECATED_MSG(msg) __declspec(deprecated(msg))
#else
#define GISMO_DEPRECATED
#define GISMO_DEPRECATED_MSG(msg)
#endif

/// Fallthrough attribute for switch statements
#if GISMO_CXX17_OR_LATER
#define GISMO_FALLTHROUGH [[fallthrough]]
#elif defined(__GNUC__) && GISMO_GCC_VERSION >= GISMO_VERSION_CHECK(7, 0, 0)
#define GISMO_FALLTHROUGH __attribute__((fallthrough))
#else
#define GISMO_FALLTHROUGH
#endif

/// Nodiscard attribute
#if GISMO_CXX17_OR_LATER
#define GISMO_NODISCARD [[nodiscard]]
#elif defined(__GNUC__)
#define GISMO_NODISCARD __attribute__((warn_unused_result))
#elif defined(_MSC_VER)
#define GISMO_NODISCARD _Check_return_
#else
#define GISMO_NODISCARD
#endif

///@}