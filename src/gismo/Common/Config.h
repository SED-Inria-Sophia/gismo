/** @file Config.h

    @brief Provides preprocessor directives configuration of G+Smo

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

/** Define the G+Smo version number. */
#define GISMO_VERSION    "24.08.0"
#define GISMO_MAJOR      24
#define GISMO_MINOR      8
#define GISMO_PATCH      0

/** Define default coefficient type. */
#define GISMO_COEFF_TYPE double
#ifndef real_t
#define real_t           GISMO_COEFF_TYPE
#endif

/** Define default index type. */
#define index_t          int

/** Define default dimension type. */
#define short_t          short

/** Define the file data directory. */
#define GISMO_DATA_DIR "/Users/tkloczko/Development/gismo/gismo/filedata"

/** Define the search paths. */
#define GISMO_SEARCH_PATHS ""

/** Define the file configuration directory. */
#define GISMO_CONFIG_DIR "/Users/tkloczko/Development/gismo/gismo/cmake"

// Basic template library configuration
// These values are set to safe defaults for Common module
#ifdef GISMO_WITH_CODIPACK
#undef GISMO_WITH_CODIPACK
#endif

#ifdef GISMO_WITH_MPI
#undef GISMO_WITH_MPI
#endif

// Standard includes expected by Common module
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Library build configuration
#ifndef GISMO_BUILD_LIB
#define GISMO_BUILD_LIB
#endif