/** @file Debug.h

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
 *  gsDebug and gsDebugVar(.) are for debugging messages and are enabled in debug
 *  mode only.
 *
 *  Note that gsDebug cannot be given as a parameter to another function.
 */
#ifndef  NDEBUG

    #define gsDebug std::cout<<"GISMO_DEBUG: "

    #define gsDebugVar(variable) gsDebug << (strrchr(__FILE__, '/') ?          \
                             strrchr(__FILE__, '/') + 1 : __FILE__) <<":"<<    \
    __LINE__<< ", "#variable": \n"<<(variable)<<std::endl
#define gsDebugIf(cond,variable) if (cond) gsDebug <<"[ "#cond" ] -- "<<       \
              (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) \
               <<":"<<__LINE__<< ", "#variable": \n"<<(variable)<<std::endl
#else
    #define gsDebug if (0) std::cout
    #define gsDebugVar(variable)
    #define gsDebugIf(cond,variable)
#endif

/**
 *  Used for optional inclusion of .hpp header files in the .h files.
 *  Allows to drop dependencies on the .hpp files when using
 *  GISMO_BUILD_LIB for compiling a library instance.
 *  When compiling as a pure template library CMake will detect
 *  dependency on .hpp files.
 */
#define GISMO_HPP_HEADER(x) #x

/**
 *  Runtime assertions which display a message
 *
 */
#ifndef NDEBUG
#   define GISMO_ASSERT(cond, message) do if(!(cond)) {std::cerr          \
       <<"Assert `"<<#cond<<"` "<<message<<"\n"<<__FILE__<<", line "\
       <<__LINE__<<" ("<<__FUNCTION__<<")"<<std::endl;                    \
       throw std::logic_error("GISMO_ASSERT"); } while(false)
#else
#   define GISMO_ASSERT(condition, message)
#endif

/**
 *  Runtime check and display error message. This command is the same as
 *  GISMO_ASSERT but it is executed in release builds as well.
 *
 */
#define GISMO_ENSURE(cond, message) do if(!(cond)) {std::cerr             \
    <<"Ensure `"<<#cond<<"` "<<message<<"\n"<<__FILE__<<", line "   \
    <<__LINE__<<" ("<< __FUNCTION__<< ")"<<std::endl;                     \
    throw std::runtime_error("GISMO_ENSURE");} while(false)

/**
 *  Denote a variable as unused, used to silence warnings in release
 *  mode builds.
 *
 */
#define GISMO_UNUSED(x)  static_cast<void>(x)

/**
 *  Runtime error message
 *
 */
#define GISMO_ERROR(message) do {std::cerr <<"Error " <<message<<"\n"\
    <<__FILE__<<", line " <<__LINE__<<" ("<<__FUNCTION__<<")"<<std::endl;  \
    throw std::runtime_error("GISMO_ERROR");} while(false)

/**
 *  Runtime "no implementation" error happens when the user calls a
 *  virtual member function without a default implementation.
 */

#define GISMO_NO_IMPLEMENTATION {std::cerr                                       \
     <<"Virtual member function `"<<__FUNCTION__<<"` has not been implemented\n" \
     <<__FILE__<<", line "<<__LINE__<<"\n"<<typeid(*this).name()<<std::endl;     \
    throw std::runtime_error("GISMO_NO_IMPLEMENTATION");}

/**
   Check if a floating point number is different than NAN (not a number)

   See https://en.wikipedia.org/wiki/Floating_point#Special_values
   and https://en.wikipedia.org/wiki/NaN
 */
template <typename T> bool gsIsnumber(T a) {return a == a;}
template <typename T> bool gsIsnan   (T a) {return a != a;}
/**
   Check if a flaoting point number is different than INF

   See https://en.wikipedia.org/wiki/Floating_point#Special_values
 */
template <typename T> bool gsIsfinite(T a) {return  (a - a) == (a - a);}

}//namespace gismo