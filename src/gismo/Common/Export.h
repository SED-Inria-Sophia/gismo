/** @file Export.h

    @brief Handles shared library creation and other class attributes

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include <gismo/Common/Config.h>

/*
  Symbol exporting and importing

  By default no symbols are exported to the shared library. Symbols
  that have to be exported must be marked with GISMO_EXPORT. Symbols
  that should not be exported can be marked with GISMO_LOCAL.
  Also:

  - When building the shared library, gismo_EXPORTS must be defined,
  but NOT when consuming G+Smo as a shared library

  - When building the static library, gismo_STATIC must be defined

  - If GISMO_BUILD_LIB is defined when building or consuming the
  shared or static library then function definitions in .hpp files are
  not included after class/function declarations. Otherwise
  (GISMO_BUILD_LIB not defined) a pure template build will take place.
  The macro GISMO_BUILD_LIB is controlled in Config.h.

  info: https://gcc.gnu.org/wiki/Visibility
*/
#if defined(gismo_STATIC) // using/building static library

#  define GISMO_EXPORT
#  define GISMO_IMPORT
//#  define GISMO_LOCAL
#  define GISMO_DEFAULT_VIS //GCC warns when base/derived class differ in visibility, this macro allows to add attribute to the templated base

#elif defined(gismo_EXPORTS) // building shared library: export, don't import

#  if defined(_WIN32) || defined(__CYGWIN__)
#    define GISMO_EXPORT __declspec(dllexport) //equiv. __attribute__ ((dllexport))
#    define GISMO_IMPORT __declspec(dllimport) //equiv. __attribute__ ((dllimport))
#    define GISMO_LOCAL
#    define GISMO_DEFAULT_VIS
#  else
#    define GISMO_EXPORT __attribute__ ((visibility ("default")))
#    define GISMO_IMPORT __attribute__ ((visibility ("default")))
#    define GISMO_LOCAL  __attribute__ ((visibility ("hidden")))
#    define GISMO_DEFAULT_VIS __attribute__ ((visibility ("default")))
#  endif

#else // consuming shared library: import

#  if defined(_WIN32) || defined(__CYGWIN__)
#    define GISMO_EXPORT __declspec(dllimport) //equiv. __attribute__ ((dllimport))
#    define GISMO_IMPORT __declspec(dllimport) //equiv. __attribute__ ((dllimport))
#    define GISMO_LOCAL
#    define GISMO_DEFAULT_VIS
#  else
#    define GISMO_EXPORT __attribute__ ((visibility ("default")))
#    define GISMO_IMPORT __attribute__ ((visibility ("default")))
#    define GISMO_LOCAL  __attribute__ ((visibility ("hidden")))
#    define GISMO_DEFAULT_VIS __attribute__ ((visibility ("default")))
#  endif

#endif