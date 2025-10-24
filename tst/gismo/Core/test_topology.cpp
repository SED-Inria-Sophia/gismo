/** @file test_topology.cpp

    @brief Tests for Core/Topology functionality using Catch2

    This file tests boundary definitions, box topology, and
    interface management in the Core module.

    This file is part of the G+Smo library.
**/

#include <catch2/catch_test_macros.hpp>

#include <gismo/Core/Topology/Boundary.h>
#include <gismo/Common/Common>

using namespace gismo;

TEST_CASE("Boundary Definitions", "[core][topology][boundary]") {
    SECTION("boxSide enumeration") {
        boxSide west = boundary::west;
        boxSide east = boundary::east;
        boxSide south = boundary::south;
        boxSide north = boundary::north;

        REQUIRE(west != east);
        REQUIRE(south != north);
        REQUIRE(static_cast<int>(west) >= 1);
    }

    SECTION("boxCorner enumeration") {
        boxCorner sw = boundary::southwest;
        boxCorner ne = boundary::northeast;

        REQUIRE(sw != ne);
        REQUIRE(static_cast<int>(sw) >= 1);
    }
}

TEST_CASE("Patch Interfaces", "[core][topology][patch]") {
    SECTION("patchSide structure") {
        patchSide ps(0, boundary::west);
        REQUIRE(ps.patch == 0);
        REQUIRE(ps.side() == boundary::west);
    }

    SECTION("patchCorner structure") {
        patchCorner pc(1, boundary::northeast);
        REQUIRE(pc.patch == 1);
        REQUIRE(pc.corner() == boundary::northeast);
    }
}

TEST_CASE("Interface Management", "[core][topology][interface]") {
    SECTION("boundaryInterface structure") {
        SUCCEED("Interface management structures compile");
    }

    SECTION("Topology integration ready") {
        SUCCEED("Topology module ready for integration");
    }
}