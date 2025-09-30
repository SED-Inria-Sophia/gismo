/** @file Math_test.cpp

    @brief Tests for the Math module - linear algebra and mathematical utilities

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Test Suite for Math Module
**/

// Simple test without UnitTest++ dependency
#include <iostream>
#include <limits>

// Test the Math module headers
#include <gismo/Math/Math>

int main()
{
    std::cout << "Testing Math module headers..." << std::endl;

    // Test that all Math headers can be included without errors
    std::cout << "✓ All headers included successfully" << std::endl;

    // Test mathematical constants
    if (std::abs(gismo::math::PI - 3.141592653589793) < 1e-15 &&
        std::abs(gismo::math::E - 2.718281828459045) < 1e-15 &&
        std::abs(gismo::math::PI_2 - 1.5707963267948966) < 1e-15) {
        std::cout << "✓ Mathematical constants work" << std::endl;
    } else {
        std::cout << "✗ Mathematical constants failed" << std::endl;
        return 1;
    }

    // Test matrix operations
    gismo::gsMatrixd mat(3, 3);
    mat << 1, 2, 3,
           4, 5, 6,
           7, 8, 9;

    if (mat.rows() == 3 && mat.cols() == 3 && mat(0,0) == 1 && mat(2,2) == 9) {
        std::cout << "✓ Dense matrix operations work" << std::endl;
    } else {
        std::cout << "✗ Dense matrix operations failed" << std::endl;
        return 1;
    }

    // Test vector operations
    gismo::gsVector<real_t> vec(3);
    vec << 1, 2, 3;

    if (vec.size() == 3 && vec[0] == 1 && vec[2] == 3) {
        std::cout << "✓ Vector operations work" << std::endl;
    } else {
        std::cout << "✗ Vector operations failed" << std::endl;
        return 1;
    }

    // Test sparse matrix operations
    gismo::gsSparseEntries<real_t> entries;
    entries.add(0, 0, 1.0);
    entries.add(1, 1, 2.0);
    entries.add(2, 2, 3.0);

    gismo::gsSparseMatrixd sparse(3, 3);
    sparse.setFrom(entries);

    if (sparse.rows() == 3 && sparse.cols() == 3 && sparse.nonZeros() == 3) {
        std::cout << "✓ Sparse matrix operations work" << std::endl;
    } else {
        std::cout << "✗ Sparse matrix operations failed" << std::endl;
        return 1;
    }

    // Test matrix view operations (basic functionality)
    gismo::gsMatrixd bigMat(6, 6);
    bigMat.setIdentity();

    // Test point grid generation
    gismo::gsVector<real_t> lower(2);
    lower << 0, 0;
    gismo::gsVector<real_t> upper(2);
    upper << 1, 1;
    gismo::gsVector<unsigned> np(2);
    np << 3, 3;

    gismo::gsMatrixd grid = gismo::uniformGrid(lower, upper, np);

    if (grid.rows() == 2 && grid.cols() == 9) {
        std::cout << "✓ Point grid generation works" << std::endl;
    } else {
        std::cout << "✗ Point grid generation failed" << std::endl;
        return 1;
    }

    // Test linear algebra functions
    gismo::gsMatrixd testMat(2, 2);
    testMat << 1, 2, 3, 4;

    if (gismo::isnumber(testMat) && gismo::isfinite(testMat)) {
        std::cout << "✓ Linear algebra utility functions work" << std::endl;
    } else {
        std::cout << "✗ Linear algebra utility functions failed" << std::endl;
        return 1;
    }

    std::cout << "All Math module tests passed! ✓" << std::endl;
    return 0;
}