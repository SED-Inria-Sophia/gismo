/** @file gsDebug.h

    @brief This file contains the debugging and messaging system of G+Smo.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <typeinfo>

// Include config for debug-specific definitions
#include "gismo/Core/Config.h"

// See also about memory leak detection:
// http://msdn.microsoft.com/en-us/library/e5ewb1h3%28v=vs.90%29.aspx
#if _MSC_VER //>= 1400
#include <crtdbg.h>
//#include <errno.h>
#endif

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
 *  gsDebug is for debug messages, available only when GISMO_DEBUG is defined
 */
#ifdef GISMO_DEBUG
    #define gsDebug std::cout<<"Debug: "
    #define gsDebugVar(variable) std::cout<<"Debug: "#variable" = "<<variable<<std::endl
#else
    #define gsDebug if(false) std::cout
    #define gsDebugVar(variable)
#endif

/** Error handling:
 *  GISMO_ERROR throws an exception with the given message
 */
#define GISMO_ERROR(message) \
    do { \
        std::ostringstream oss; \
        oss << "Error in " << __FILE__ << ":" << __LINE__ << ": " << message; \
        throw std::runtime_error(oss.str()); \
    } while(0)

/** Assert macro:
 *  GISMO_ASSERT throws an exception if condition is false (only in debug builds)
 */
#ifdef GISMO_DEBUG
    #define GISMO_ASSERT(condition, message) \
        do { \
            if(!(condition)) { \
                GISMO_ERROR("Assertion failed: " << #condition << ". " << message); \
            } \
        } while(0)
#else
    #define GISMO_ASSERT(condition, message) ((void)0)
#endif

/** Ensure macro:
 *  GISMO_ENSURE throws an exception if condition is false (always checked)
 */
#define GISMO_ENSURE(condition, message) \
    do { \
        if(!(condition)) { \
            GISMO_ERROR("Condition failed: " << #condition << ". " << message); \
        } \
    } while(0)

/** Unused variable helper to avoid compiler warnings */
#define GISMO_UNUSED(x) ((void)(x))

/** Stringify macro for creating string literals */
#define GISMO_STRINGIFY(x) #x
#define GISMO_TOSTRING(x) GISMO_STRINGIFY(x)

} // namespace gismo