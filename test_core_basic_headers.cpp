/** @file test_core_basic_headers.cpp

    @brief Test for basic gismo/Core module headers (excluding LinearAlgebra.h).

    This test verifies that the basic Core module headers can be included
    correctly without requiring full Matrix module dependencies.

    Usage:
    From build directory:
    g++ -I../src -I./src -std=c++17 ../test_core_basic_headers.cpp -o test_core_basic_headers

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Generated test for Core module basic header verification
**/

#include <iostream>

// Test basic Core module headers (without LinearAlgebra.h which needs Matrix module)

// Step 1: Include Config.h first (generated during build)
#include <gismo/Core/Config.h>

// Step 2: Include Export.h (depends on Config.h)
#include <gismo/Core/Export.h>

// Step 3: Include Debug.h (standalone)
#include <gismo/Core/Debug.h>

// Step 4: Include Memory.h (standalone)
#include <gismo/Core/Memory.h>

// Test multiple inclusion safety
#include <gismo/Core/Export.h>
#include <gismo/Core/Debug.h>
#include <gismo/Core/Memory.h>

int main() {
    std::cout << "=== G+Smo Core Module Basic Header Inclusion Test ===" << std::endl;
    std::cout << std::endl;

    std::cout << "✓ Config.h included successfully" << std::endl;
    std::cout << "✓ Export.h included successfully" << std::endl;
    std::cout << "✓ Debug.h included successfully" << std::endl;
    std::cout << "✓ Memory.h included successfully" << std::endl;
    std::cout << "✓ Multiple inclusions work (header guards effective)" << std::endl;

    std::cout << std::endl;
    std::cout << "🎉 BASIC COMPILATION TEST PASSED!" << std::endl;
    std::cout << "Basic Core module headers compile correctly." << std::endl;
    std::cout << std::endl;

    // Basic functionality test - can we use some definitions from the headers?
    std::cout << "Basic functionality test:" << std::endl;
    std::cout << "GISMO version: " << GISMO_VERSION << std::endl;

    #ifdef GISMO_PLATFORM_MACOS
    std::cout << "Platform: macOS detected" << std::endl;
    #endif

    #ifdef GISMO_COMPILER_CLANG
    std::cout << "Compiler: Clang detected" << std::endl;
    #endif

    std::cout << std::endl;
    std::cout << "📝 Note: LinearAlgebra.h and ForwardDeclarations.h require Matrix module" << std::endl;
    std::cout << "   and full project build for complete header testing." << std::endl;

    return 0;
}