/** @file Stopwatch.h

    @brief Timing functions.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): C. Hofreither, J. Vogl
*/

#pragma once

#include <ostream>

// includes for wall clocks
#if __cplusplus >= 201103L || _MSC_VER >= 1600
#  include <chrono>
#elif defined(__linux__)
#  include <sys/time.h>
#elif defined(_MSC_VER) || defined(__MINGW32__)
#  include <sys/timeb.h>
#else
#  include <ctime>
#endif

// includes for CPU clocks
#if defined(__linux__)
#  include <sys/resource.h>
#else
#  include <ctime>
#endif

namespace gismo
{

inline std::ostream& formatTime(std::ostream& os, double sec)
{
    int flo = (int)(sec);
    sec -= flo;
    int hh = flo / 3600;
    flo = flo  % 3600;
    int mm = flo / 60;
    double ss = (flo % 60) + sec;
    if (hh > 0) os << hh << "h ";
    if (mm > 0) os << mm << "m ";
    std::streamsize prec = os.precision();
    os.precision(2);
    os << ss << "s";
    os.precision(prec);
    return os;
}

} // namespace gismo