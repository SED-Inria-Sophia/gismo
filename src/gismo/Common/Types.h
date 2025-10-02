/** @file Types.h

    @brief Fundamental type definitions for GISMO Common module.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include <cstddef>
#include <cstdint>

namespace gismo {

/** @name Fundamental Types
 *  Basic type definitions used throughout GISMO library.
 */

///@{

// Basic scalar types (defined in Config.h but repeated here for clarity)
#ifndef real_t
/// Default floating point type
typedef double real_t;
#endif

#ifndef index_t
/// Default integer type for indices
typedef int index_t;
#endif

#ifndef short_t
/// Default short integer type
typedef short short_t;
#endif

// Additional fundamental types for Common module

/// Size type for containers and array indices
typedef std::size_t size_type;

/// Difference type for pointer arithmetic
typedef std::ptrdiff_t difference_type;

/// Unsigned integer types
typedef std::uint8_t  uint8_t;
typedef std::uint16_t uint16_t;
typedef std::uint32_t uint32_t;
typedef std::uint64_t uint64_t;

/// Signed integer types
typedef std::int8_t   int8_t;
typedef std::int16_t  int16_t;
typedef std::int32_t  int32_t;
typedef std::int64_t  int64_t;

/// Boolean type
typedef bool bool_t;

/// Character types
typedef char      char_t;
// wchar_t is already a built-in type, no need for typedef

/// Floating point types
typedef float     float32_t;
typedef double    float64_t;

///@}

/** @name Type Traits
 *  Basic type trait helpers for template programming.
 */

///@{

/// Helper to check if a type is arithmetic
template<typename T>
struct is_arithmetic_type {
    static const bool value = false;
};

// Specializations for arithmetic types
template<> struct is_arithmetic_type<bool>           { static const bool value = true; };
template<> struct is_arithmetic_type<char>           { static const bool value = true; };
template<> struct is_arithmetic_type<signed char>    { static const bool value = true; };
template<> struct is_arithmetic_type<unsigned char>  { static const bool value = true; };
template<> struct is_arithmetic_type<short>          { static const bool value = true; };
template<> struct is_arithmetic_type<unsigned short> { static const bool value = true; };
template<> struct is_arithmetic_type<int>            { static const bool value = true; };
template<> struct is_arithmetic_type<unsigned int>   { static const bool value = true; };
template<> struct is_arithmetic_type<long>           { static const bool value = true; };
template<> struct is_arithmetic_type<unsigned long>  { static const bool value = true; };
template<> struct is_arithmetic_type<long long>      { static const bool value = true; };
template<> struct is_arithmetic_type<unsigned long long> { static const bool value = true; };
template<> struct is_arithmetic_type<float>          { static const bool value = true; };
template<> struct is_arithmetic_type<double>         { static const bool value = true; };
template<> struct is_arithmetic_type<long double>    { static const bool value = true; };

/// Helper to check if a type is floating point
template<typename T>
struct is_floating_point_type {
    static const bool value = false;
};

template<> struct is_floating_point_type<float>       { static const bool value = true; };
template<> struct is_floating_point_type<double>      { static const bool value = true; };
template<> struct is_floating_point_type<long double> { static const bool value = true; };

/// Helper to check if a type is integral
template<typename T>
struct is_integral_type {
    static const bool value = is_arithmetic_type<T>::value && !is_floating_point_type<T>::value;
};

///@}

} // namespace gismo