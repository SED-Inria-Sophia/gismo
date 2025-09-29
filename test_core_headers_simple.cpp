/** @file test_core_headers_simple.cpp

    @brief Simple compilation test for gismo/Core module headers.

    This test verifies that the Core module headers can be included in the correct
    order with proper dependencies. It's designed to work with the built project.

    Usage:
    From build directory:
    g++ -I../src -I./src -std=c++17 ../test_core_headers_simple.cpp -o test_core_headers_simple

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Generated test for Core module header verification
**/

#include <iostream>

// Test Core module header inclusions in proper dependency order
// Following the correct order based on type dependencies

// Step 1: Include Config.h first (generated during build)
#include <gismo/Core/Config.h>

// Step 2: Include Export.h (depends on Config.h)
#include <gismo/Core/Export.h>

// Step 3: Include LinearAlgebra.h (defines basic types like short_t)
// Note: This may require Eigen, but we'll test compilation
#include <gismo/Core/LinearAlgebra.h>

// Step 4: Include ForwardDeclarations.h (depends on short_t from LinearAlgebra.h)
#include <gismo/Core/ForwardDeclarations.h>

// Step 5: Include Debug.h and Memory.h (standalone)
#include <gismo/Core/Debug.h>
#include <gismo/Core/Memory.h>

// Test multiple inclusion safety
#include <gismo/Core/Export.h>
#include <gismo/Core/Debug.h>
#include <gismo/Core/Memory.h>

int main() {
    std::cout << "=== G+Smo Core Module Header Inclusion Test (Simple) ===" << std::endl;
    std::cout << std::endl;

    std::cout << "✓ Config.h included successfully" << std::endl;
    std::cout << "✓ Export.h included successfully" << std::endl;
    std::cout << "✓ LinearAlgebra.h included successfully" << std::endl;
    std::cout << "✓ ForwardDeclarations.h included successfully" << std::endl;
    std::cout << "✓ Debug.h included successfully" << std::endl;
    std::cout << "✓ Memory.h included successfully" << std::endl;
    std::cout << "✓ Multiple inclusions work (header guards effective)" << std::endl;

    std::cout << std::endl;
    std::cout << "🎉 COMPILATION TEST PASSED!" << std::endl;
    std::cout << "All Core module headers compile correctly" << std::endl;
    std::cout << "when included in proper dependency order." << std::endl;

    // Basic functionality test - can we use some types/macros from the headers?
    std::cout << std::endl;
    std::cout << "Basic functionality test:" << std::endl;
    std::cout << "GISMO version: " << GISMO_VERSION << std::endl;

    return 0;
}