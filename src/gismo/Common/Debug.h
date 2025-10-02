/** @file Debug.h

    @brief This file contains the debugging and messaging system of G+Smo.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/


// Start DEBUG_GROUP of Doxygen
/** @{ */

#pragma once

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