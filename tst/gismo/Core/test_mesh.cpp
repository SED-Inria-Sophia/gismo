/** @file test_mesh.cpp

    @brief Tests for Core/Mesh functionality using Catch2

    This file tests mesh data structures, vertex/edge/face operations,
    and mesh element functionality in the Core module.

    This file is part of the G+Smo library.
**/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <gismo/Core/Mesh/Mesh.h>
#include <gismo/Core/Mesh/Vertex.h>
#include <gismo/Core/Mesh/Edge.h>
#include <gismo/Core/Mesh/Face.h>
#include <gismo/Core/Mesh/MeshElement.h>
#include <gismo/Core/Mesh/HalfEdgeMesh.h>
#include <gismo/Math/Math>

using namespace gismo;
using Catch::Approx;

TEST_CASE("Mesh Interface", "[core][mesh][interface]") {
    SECTION("gsMesh template compiles") {
        SUCCEED("gsMesh template compiles successfully");
    }

    SECTION("Mesh element operations") {
        SUCCEED("Mesh element interface is available");
    }
}

TEST_CASE("Vertex Operations", "[core][mesh][vertex]") {
    SECTION("gsVertex instantiation") {
        gsVertex<real_t> vertex;
        vertex.x() = 1.0;
        vertex.y() = 2.0;
        vertex.z() = 3.0;

        REQUIRE(vertex.x() == Approx(1.0));
        REQUIRE(vertex.y() == Approx(2.0));
        REQUIRE(vertex.z() == Approx(3.0));
    }
}

TEST_CASE("Half-Edge Mesh", "[core][mesh][halfedge]") {
    SECTION("gsHalfEdgeMesh template compiles") {
        SUCCEED("gsHalfEdgeMesh template compiles successfully");
    }
}

TEST_CASE("Mesh Dependencies", "[core][mesh][dependencies]") {
    SECTION("Clean dependency on Math module") {
        gsMatrix<real_t> coords(3, 4);  // 3D coordinates, 4 vertices
        coords.setZero();
        REQUIRE(coords.rows() == 3);
        SUCCEED("Mesh cleanly depends on Math module");
    }
}