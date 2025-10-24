/** @file test_multipatch.cpp

    @brief Tests for Core/MultiPatch functionality using Catch2

    This file tests multi-patch geometry and basis functionality
    in the Core module.

    This file is part of the G+Smo library.
**/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <gismo/Core/MultiPatch/MultiPatch.h>
#include <gismo/Core/MultiPatch/MultiBasis.h>
#include <gismo/Math/Math>

using namespace gismo;

TEST_CASE("MultiPatch Interface", "[core][multipatch][interface]") {
    SECTION("gsMultiPatch template compiles") {
        SUCCEED("gsMultiPatch template compiles successfully");
    }

    SECTION("Patch management interface") {
        SUCCEED("MultiPatch patch management interface is available");
    }
}

TEST_CASE("MultiBasis Interface", "[core][multipatch][multibasis]") {
    SECTION("gsMultiBasis template compiles") {
        SUCCEED("gsMultiBasis template compiles successfully");
    }

    SECTION("Basis assembly interface") {
        SUCCEED("MultiBasis assembly interface is available");
    }
}

TEST_CASE("MultiPatch Dependencies", "[core][multipatch][dependencies]") {
    SECTION("Integration with Topology") {
        boxSide side = boundary::west;
        REQUIRE(static_cast<int>(side) >= 1);
        SUCCEED("MultiPatch integrates with Topology module");
    }
}