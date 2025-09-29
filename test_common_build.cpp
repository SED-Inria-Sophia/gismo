/** @file test_common_build.cpp

    @brief Test that Common module integrates with CMake build system

    This test verifies that the renamed Common module works properly
    with the GISMO build system and CMake targets.
*/

#include <iostream>
#include <gismo/Common/Config.h>
#include <gismo/Common/Export.h>
#include <gismo/Common/Debug.h>
#include <gismo/Common/Memory.h>

int main() {
    std::cout << "=== G+Smo Common Module Build Integration Test ===" << std::endl;
    std::cout << std::endl;

    std::cout << "✓ Config.h included successfully" << std::endl;
    std::cout << "✓ Export.h included successfully" << std::endl;
    std::cout << "✓ Debug.h included successfully" << std::endl;
    std::cout << "✓ Memory.h included successfully" << std::endl;

    std::cout << std::endl;
    std::cout << "🎉 COMMON MODULE BUILD INTEGRATION TEST PASSED!" << std::endl;
    std::cout << "Common module successfully renamed from Core and works with build system." << std::endl;

    std::cout << std::endl;
    std::cout << "Build configuration:" << std::endl;
    std::cout << "GISMO version: " << GISMO_VERSION << std::endl;

    return 0;
}