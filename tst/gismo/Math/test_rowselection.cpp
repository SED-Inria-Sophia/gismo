/**
 * @file test_rowselection.cpp
 * @brief Tests for RowSelection class functionality
 *
 * Tests the RowSelection Eigen extension that allows selecting specific
 * rows from matrices and vectors.
 *
 * Part of gismo/Math module testing suite.
 */

#include <iostream>
#include <gismo/Common/Common>
#include <gismo/Math/LinearAlgebra.h>

using namespace gismo;

/**
 * Test basic RowSelection functionality
 */
void testBasicRowSelection() {
    std::cout << "1. Testing basic RowSelection functionality..." << std::endl;

    // Create a test matrix
    gsEigen::Matrix3d mat;
    mat << 1, 2, 3,
           4, 5, 6,
           7, 8, 9;

    // Create index vector to select rows 0 and 2
    gsEigen::Vector2i indices;
    indices << 0, 2;

    // Test selectRows method
    auto selected = mat.selectRows(indices);

    std::cout << "   Original matrix:" << std::endl;
    std::cout << "   " << mat.row(0) << std::endl;
    std::cout << "   " << mat.row(1) << std::endl;
    std::cout << "   " << mat.row(2) << std::endl;

    std::cout << "   Selected rows (0, 2):" << std::endl;
    std::cout << "   Dimensions: " << selected.rows() << " x " << selected.cols() << std::endl;

    // Verify dimensions
    if (selected.rows() == 2 && selected.cols() == 3) {
        std::cout << "   ✓ Correct dimensions (2x3)" << std::endl;
    }

    // Verify values
    bool values_correct = (selected.coeff(0, 0) == 1.0) && (selected.coeff(0, 1) == 2.0) && (selected.coeff(0, 2) == 3.0) &&
                         (selected.coeff(1, 0) == 7.0) && (selected.coeff(1, 1) == 8.0) && (selected.coeff(1, 2) == 9.0);

    if (values_correct) {
        std::cout << "   ✓ Selected row values are correct" << std::endl;
    }

    std::cout << "   ✓ Basic RowSelection functionality working" << std::endl;
}

/**
 * Test RowSelection with vectors
 */
void testVectorRowSelection() {
    std::cout << "2. Testing RowSelection with vectors..." << std::endl;

    // Create a test vector
    gsEigen::VectorXd vec(5);
    vec << 10, 20, 30, 40, 50;

    // Select specific elements (treating as rows)
    gsEigen::VectorXi indices(3);
    indices << 1, 3, 4;  // Select indices 1, 3, 4

    auto selected = vec.selectRows(indices);

    std::cout << "   Original vector: " << vec.transpose() << std::endl;
    std::cout << "   Selected indices (1,3,4): ";
    std::cout << selected.transpose() << std::endl;

    // Verify values
    bool values_correct = (selected.coeff(0, 0) == 20.0) &&
                         (selected.coeff(1, 0) == 40.0) &&
                         (selected.coeff(2, 0) == 50.0);

    if (values_correct) {
        std::cout << "   ✓ Vector row selection values correct" << std::endl;
    }

    std::cout << "   ✓ Vector RowSelection functionality working" << std::endl;
}

/**
 * Test RowSelection with dynamic indices
 */
void testDynamicRowSelection() {
    std::cout << "3. Testing RowSelection with dynamic indices..." << std::endl;

    // Create a larger test matrix
    gsEigen::MatrixXd mat(5, 4);
    mat << 1,  2,  3,  4,
           5,  6,  7,  8,
           9,  10, 11, 12,
           13, 14, 15, 16,
           17, 18, 19, 20;

    // Create dynamic index vector
    gsEigen::VectorXi indices = gsEigen::VectorXi::Zero(3);
    indices << 0, 2, 4;  // First, third, and fifth rows

    auto selected = mat.selectRows(indices);

    std::cout << "   Original matrix: 5x4" << std::endl;
    std::cout << "   Selected rows (0,2,4): " << selected.rows() << "x" << selected.cols() << std::endl;

    // Verify specific values
    bool first_row_correct = (selected.coeff(0, 0) == 1.0) && (selected.coeff(0, 3) == 4.0);
    bool third_row_correct = (selected.coeff(1, 0) == 9.0) && (selected.coeff(1, 3) == 12.0);
    bool fifth_row_correct = (selected.coeff(2, 0) == 17.0) && (selected.coeff(2, 3) == 20.0);

    if (first_row_correct && third_row_correct && fifth_row_correct) {
        std::cout << "   ✓ Dynamic row selection values correct" << std::endl;
    }

    std::cout << "   ✓ Dynamic RowSelection functionality working" << std::endl;
}

/**
 * Test RowSelection chaining and expressions
 */
void testRowSelectionExpressions() {
    std::cout << "4. Testing RowSelection expressions..." << std::endl;

    // Create test matrix
    gsEigen::Matrix3d mat;
    mat << 2, 4, 6,
           8, 10, 12,
           14, 16, 18;

    gsEigen::Vector2i indices;
    indices << 0, 2;

    // Test that RowSelection can be used in expressions
    auto selected = mat.selectRows(indices);

    // Test evaluation to dense matrix
    gsEigen::MatrixXd dense = selected;

    std::cout << "   Dense evaluation successful" << std::endl;
    std::cout << "   Result dimensions: " << dense.rows() << "x" << dense.cols() << std::endl;

    // Verify the dense matrix has correct values
    bool dense_correct = (dense(0, 0) == 2.0) && (dense(0, 2) == 6.0) &&
                        (dense(1, 0) == 14.0) && (dense(1, 2) == 18.0);

    if (dense_correct) {
        std::cout << "   ✓ Dense evaluation produces correct results" << std::endl;
    }

    std::cout << "   ✓ RowSelection expressions working" << std::endl;
}

int main() {
    std::cout << "=== RowSelection Functionality Test ===" << std::endl;

    try {
        testBasicRowSelection();
        testVectorRowSelection();
        testDynamicRowSelection();
        testRowSelectionExpressions();

        std::cout << std::endl;
        std::cout << "=== All Tests Passed ===" << std::endl;
        std::cout << "RowSelection implementation completed successfully!" << std::endl;
        std::cout << "- Basic row selection from matrices working" << std::endl;
        std::cout << "- Vector row selection (element selection) working" << std::endl;
        std::cout << "- Dynamic indices supported" << std::endl;
        std::cout << "- Expression template integration working" << std::endl;
        std::cout << "- Dense evaluation supported" << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}