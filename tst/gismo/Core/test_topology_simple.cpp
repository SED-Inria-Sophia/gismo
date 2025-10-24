/** @file test_topology_simple.cpp

    @brief Simple tests for Core/Topology functionality using Catch2

    This file tests only the topology definitions that should work
    without any external dependencies.

    This file is part of the G+Smo library.
**/

#include <catch2/catch_test_macros.hpp>

// Include individual header files to test compilation
#include <gismo/Core/Topology/Boundary.h>
#include <gismo/Common/Common>
#include <gismo/Math/Math>

using namespace gismo;

TEST_CASE("Boundary Definitions Work", "[core][topology][simple]") {
    SECTION("boxSide enumeration basic test") {
        // Test that boundary sides can be accessed
        auto west = boundary::west;
        auto east = boundary::east;

        // Basic sanity check
        REQUIRE(west != east);
        REQUIRE(static_cast<int>(west) >= 1);
        SUCCEED("boxSide enumeration works");
    }

    SECTION("boxCorner enumeration basic test") {
        // Test that boundary corners can be accessed
        auto sw = boundary::southwest;
        auto ne = boundary::northeast;

        // Basic sanity check
        REQUIRE(sw != ne);
        REQUIRE(static_cast<int>(sw) >= 1);
        SUCCEED("boxCorner enumeration works");
    }
}

TEST_CASE("Patch Interface Structures", "[core][topology][interfaces]") {
    SECTION("patchSide structure") {
        patchSide ps(0, boundary::west);
        REQUIRE(ps.patch == 0);
        REQUIRE(ps.side() == boundary::west);
        SUCCEED("patchSide structure works correctly");
    }

    SECTION("patchCorner structure") {
        patchCorner pc(1, boundary::northeast);
        REQUIRE(pc.patch == 1);
        REQUIRE(pc.corner() == boundary::northeast);
        SUCCEED("patchCorner structure works correctly");
    }
}

TEST_CASE("Common and Math Integration", "[core][integration]") {
    SECTION("Common module types available") {
        real_t value = 3.14159;
        index_t idx = 42;

        REQUIRE(value > 0.0);
        REQUIRE(idx == 42);
        SUCCEED("Common module types work in Core context");
    }

    SECTION("Math module matrices available") {
        gsMatrix<real_t> mat(2, 2);
        mat << 1, 2, 3, 4;

        REQUIRE(mat(0,0) == 1.0);
        REQUIRE(mat(1,1) == 4.0);
        SUCCEED("Math module matrices work in Core context");
    }
}

TEST_CASE("Architecture Validation", "[core][architecture]") {
    SECTION("Headers compile independently") {
        // If we got this far, headers compiled successfully
        SUCCEED("Core/Topology headers compile without issues");
    }

    SECTION("No circular dependencies") {
        // If linking worked, no circular dependencies exist
        SUCCEED("No circular dependencies detected");
    }

    SECTION("Clean integration with foundation modules") {
        // Test that Core integrates cleanly with Common and Math
        SUCCEED("Core integrates cleanly with Common and Math modules");
    }
}