/** @file Utils.h

    @brief Several utility functions for miscellaneous tasks

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

 Author(s): A. Mantzaflaris, Harald Weiner, J. Vogl
*/

#pragma once

#include <sstream>
#include <numeric>
#include <string>

#define STRINGIFY(x) #x

namespace gismo
{

/** @namespace util

    @brief This namespace gathers several utility functions for
    miscellaneous tasks
*/
namespace util
{

/// \brief Converts value to string, assuming "operator<<" defined on C
/// \ingroup Utils
template<typename C>
std::string to_string(const C & value)
{
    std::ostringstream convert;
    convert << value;
    return convert.str();
}

/// \brief Checks if a string \a haystack begins with the string \a needle
/// \ingroup Utils
inline bool starts_with( const std::string & haystack, const std::string & needle )
{
    std::string::const_iterator it1 = haystack.begin();
    std::string::const_iterator it2 = needle.begin();
    while ( it2!=needle.end() )
    {
        if ( it1 == haystack.end() || *it1 != *it2) return false;
        it1++; it2++;
    }
    return true;
}

/// \brief Checks if a string \a haystack ends with the string \a needle
/// \ingroup Utils
inline bool ends_with( const std::string & haystack, const std::string & needle )
{
    if (needle.size() > haystack.size()) return false;
    return std::equal(needle.rbegin(), needle.rend(), haystack.rbegin());
}

} // namespace util

} // namespace gismo