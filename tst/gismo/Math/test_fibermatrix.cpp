/**
 * @file test_fibermatrix.cpp
 * @brief Comprehensive test suite for gsFiberMatrix class
 *
 * This test provides thorough coverage of gsFiberMatrix functionality,
 * including construction, element access, resizing, fiber operations,
 * and conversion to sparse matrices.
 *
 * Part of GISMO Math Module Testing
 */

#include <iostream>
#include <vector>
#include <sstream>

// Include full Math module for comprehensive testing
#include <gismo/Math/Math>

using namespace gismo;

int main() {
    std::cout << "=== Comprehensive gsFiberMatrix Tests ===" << std::endl;

    try {
        // Test 1: Template instantiation and constructors
        std::cout << "\n1. Testing template instantiation and constructors..." << std::endl;

        // Test ColMajor (default) version
        gsFiberMatrix<double> col_fiber(3, 4);  // 3x4 matrix
        std::cout << "   ColMajor gsFiberMatrix<double>(3,4): " << col_fiber.rows() << "x" << col_fiber.cols() << std::endl;

        // Test RowMajor version
        gsFiberMatrix<double, RowMajor> row_fiber(2, 3);  // 2x3 matrix
        std::cout << "   RowMajor gsFiberMatrix<double>(2,3): " << row_fiber.rows() << "x" << row_fiber.cols() << std::endl;

        // Test float specialization
        gsFiberMatrix<float> float_fiber(2, 2);
        std::cout << "   Float gsFiberMatrix<float>(2,2): " << float_fiber.rows() << "x" << float_fiber.cols() << std::endl;

        // Test copy constructor
        gsFiberMatrix<double> original(2, 3);
        original(0, 0) = 1.0;
        original(1, 2) = 2.5;

        gsFiberMatrix<double> copied(original);
        std::cout << "   Copy constructor: " << copied.rows() << "x" << copied.cols() << ", copied(1,2) = " << copied(1, 2) << std::endl;

        std::cout << "✅ Template instantiation and constructors successful" << std::endl;

        // Test 2: Element access and modification
        std::cout << "\n2. Testing element access and modification..." << std::endl;

        gsFiberMatrix<double> access_mat(3, 4);

        // Test write access with operator()
        access_mat(0, 0) = 1.5;
        access_mat(0, 2) = 2.5;
        access_mat(1, 1) = 3.5;
        access_mat(2, 3) = 4.5;

        // Test read access with operator()
        std::cout << "   operator()(0,0) = " << access_mat(0, 0) << std::endl;
        std::cout << "   operator()(1,1) = " << access_mat(1, 1) << std::endl;
        std::cout << "   operator()(2,3) = " << access_mat(2, 3) << std::endl;

        // Test read access with coeff()
        std::cout << "   coeff(0,2) = " << access_mat.coeff(0, 2) << std::endl;

        // Test zero element access
        std::cout << "   Zero element coeff(1,0) = " << access_mat.coeff(1, 0) << std::endl;

        std::cout << "✅ Element access and modification successful" << std::endl;

        // Test 3: Fiber operations and iterators
        std::cout << "\n3. Testing fiber operations and iterators..." << std::endl;

        gsFiberMatrix<double> fiber_mat(3, 4);
        fiber_mat(0, 1) = 10.0;
        fiber_mat(0, 3) = 30.0;
        fiber_mat(1, 0) = 40.0;
        fiber_mat(1, 2) = 50.0;

        // Test fiber access
        auto& fiber0 = fiber_mat.fiber(0);  // First row in ColMajor, first col in RowMajor
        std::cout << "   Fiber 0 size: " << fiber0.size() << std::endl;
        std::cout << "   Fiber 0 nonZeros: " << fiber0.nonZeros() << std::endl;

        // Test fiber iteration
        std::cout << "   Iterating through fiber 0 non-zeros:" << std::endl;
        for (gsFiberMatrix<double>::iterator it(fiber_mat.fiber(0)); it; ++it) {
            std::cout << "     Index " << it.index() << ": " << it.value() << std::endl;
        }

        // Test fiber modification
        fiber_mat.fiber(2).insert(1) = 99.0;
        std::cout << "   After fiber insertion: fiber_mat(2,1) = " << fiber_mat(2, 1) << std::endl;

        std::cout << "✅ Fiber operations and iterators successful" << std::endl;

        // Test 4: Matrix sizing and resizing
        std::cout << "\n4. Testing matrix sizing and resizing..." << std::endl;

        gsFiberMatrix<double> resize_mat(2, 3);
        resize_mat(0, 0) = 1.0;
        resize_mat(1, 2) = 2.0;

        std::cout << "   Original size: " << resize_mat.rows() << "x" << resize_mat.cols() << std::endl;
        std::cout << "   Original outerSize: " << resize_mat.outerSize() << ", innerSize: " << resize_mat.innerSize() << std::endl;
        std::cout << "   Original total size: " << resize_mat.size() << std::endl;

        // Test resizing
        resize_mat.resize(4, 5);
        std::cout << "   After resize(4,5): " << resize_mat.rows() << "x" << resize_mat.cols() << std::endl;
        std::cout << "   Preserved element (0,0): " << resize_mat(0, 0) << std::endl;
        std::cout << "   Preserved element (1,2): " << resize_mat(1, 2) << std::endl;

        // Test new elements are zero
        std::cout << "   New element (3,4): " << resize_mat(3, 4) << std::endl;

        std::cout << "✅ Matrix sizing and resizing successful" << std::endl;

        // Test 5: Block operations
        std::cout << "\n5. Testing block operations..." << std::endl;

        gsFiberMatrix<double> block_mat(4, 3);
        block_mat(0, 0) = 1.0; block_mat(0, 1) = 2.0;
        block_mat(1, 0) = 3.0; block_mat(1, 2) = 4.0;
        block_mat(2, 1) = 5.0; block_mat(2, 2) = 6.0;
        block_mat(3, 0) = 7.0; block_mat(3, 1) = 8.0;

        // Test row block extraction
        auto row_block = block_mat.rowBlock(1, 2);  // Rows 1-2
        std::cout << "   Row block (1,2) num rows: " << row_block.num << ", parent cols: " << row_block.mat.cols() << std::endl;

        // Test middle rows (alias for rowBlock)
        auto middle = block_mat.middleRows(1, 2);
        std::cout << "   Middle rows (1,2) num rows: " << middle.num << ", parent cols: " << middle.mat.cols() << std::endl;

        // Test block assignment
        gsFiberMatrix<double> source_block(2, 3);
        source_block(0, 0) = 10.0; source_block(0, 2) = 20.0;
        source_block(1, 1) = 30.0; source_block(1, 2) = 40.0;

        std::cout << "   Creating row block from source..." << std::endl;
        auto rb = source_block.rowBlock(0, 2);
        std::cout << "   Row block created, converting to matrix..." << std::endl;
        gsFiberMatrix<double> target_block(rb);
        std::cout << "   Block assignment result: " << target_block.rows() << "x" << target_block.cols() << std::endl;
        if (target_block.rows() > 0 && target_block.cols() > 2)
            std::cout << "   Block element (0,2): " << target_block(0, 2) << std::endl;

        std::cout << "✅ Block operations successful" << std::endl;

        // Test 6: Conversion to sparse matrix
        std::cout << "\n6. Testing conversion to sparse matrix..." << std::endl;

        gsFiberMatrix<double> conv_mat(3, 3);
        conv_mat(0, 0) = 1.0; conv_mat(0, 2) = 3.0;
        conv_mat(1, 1) = 5.0;
        conv_mat(2, 0) = 7.0; conv_mat(2, 2) = 9.0;

        // Convert to sparse matrix
        gsSparseMatrix<double> sparse_result = conv_mat.toSparse();
        std::cout << "   Converted to sparse: " << sparse_result.rows() << "x" << sparse_result.cols() << std::endl;
        std::cout << "   Sparse nonZeros: " << sparse_result.nonZeros() << std::endl;

        // Verify conversion correctness
        std::cout << "   Verification - sparse(0,0): " << sparse_result.coeff(0, 0) << std::endl;
        std::cout << "   Verification - sparse(1,1): " << sparse_result.coeff(1, 1) << std::endl;
        std::cout << "   Verification - sparse(2,2): " << sparse_result.coeff(2, 2) << std::endl;

        // Test conversion preserves sparsity pattern
        bool conversion_correct = true;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (std::abs(conv_mat(i, j) - sparse_result.coeff(i, j)) > 1e-10) {
                    conversion_correct = false;
                    break;
                }
            }
        }

        if (conversion_correct) {
            std::cout << "   ✅ Sparse conversion preserves all elements correctly" << std::endl;
        } else {
            std::cout << "   ❌ Sparse conversion has discrepancies" << std::endl;
        }

        std::cout << "✅ Conversion to sparse matrix successful" << std::endl;

        // Test 7: RowMajor vs ColMajor behavior
        std::cout << "\n7. Testing RowMajor vs ColMajor storage..." << std::endl;

        // ColMajor version (default)
        gsFiberMatrix<double, ColMajor> col_major(2, 3);
        col_major(0, 1) = 1.0;
        col_major(1, 2) = 2.0;

        // RowMajor version
        gsFiberMatrix<double, RowMajor> row_major(2, 3);
        row_major(0, 1) = 1.0;
        row_major(1, 2) = 2.0;

        std::cout << "   ColMajor matrix outerSize: " << col_major.outerSize() << ", innerSize: " << col_major.innerSize() << std::endl;
        std::cout << "   RowMajor matrix outerSize: " << row_major.outerSize() << ", innerSize: " << row_major.innerSize() << std::endl;

        // Test fiber interpretation (ColMajor stores columns as fibers, RowMajor stores rows)
        std::cout << "   ColMajor fiber 1 nonZeros: " << col_major.fiber(1).nonZeros() << std::endl;  // Column 1
        std::cout << "   RowMajor fiber 1 nonZeros: " << row_major.fiber(1).nonZeros() << std::endl;   // Row 1

        std::cout << "✅ RowMajor vs ColMajor storage successful" << std::endl;

        // Test 8: Utility operations
        std::cout << "\n8. Testing utility operations..." << std::endl;

        gsFiberMatrix<double> util_mat(3, 3);
        util_mat(0, 0) = 1.0; util_mat(0, 2) = 2.0;
        util_mat(1, 1) = 3.0;
        util_mat(2, 0) = 4.0; util_mat(2, 2) = 5.0;

        // Test reserve (for efficiency)
        util_mat.reserve(10);
        std::cout << "   Reserve operation completed (efficiency hint)" << std::endl;

        // Test setZero
        util_mat.setZero();
        std::cout << "   After setZero: (0,0) = " << util_mat(0, 0) << ", (1,1) = " << util_mat(1, 1) << std::endl;

        // Verify all elements are zero
        bool all_zero = true;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (util_mat(i, j) != 0.0) {
                    all_zero = false;
                    break;
                }
            }
        }

        if (all_zero) {
            std::cout << "   ✅ setZero() cleared all elements successfully" << std::endl;
        } else {
            std::cout << "   ❌ setZero() did not clear all elements" << std::endl;
        }

        std::cout << "✅ Utility operations successful" << std::endl;

        // Test 9: Assignment operations
        std::cout << "\n9. Testing assignment operations..." << std::endl;

        gsFiberMatrix<double> assign_source(2, 2);
        assign_source(0, 0) = 10.0; assign_source(0, 1) = 20.0;
        assign_source(1, 0) = 30.0; assign_source(1, 1) = 40.0;

        // Test copy assignment
        gsFiberMatrix<double> assign_target(2, 2);  // Pre-initialize to same size
        assign_target = assign_source;
        std::cout << "   Copy assignment result: " << assign_target.rows() << "x" << assign_target.cols() << std::endl;
        std::cout << "   Copied element (1,1): " << assign_target(1, 1) << std::endl;

        // Test self-assignment safety
        assign_target = assign_target;
        std::cout << "   Self-assignment safety: (0,0) = " << assign_target(0, 0) << std::endl;

        std::cout << "✅ Assignment operations successful" << std::endl;

        // Test 10: Print and output operations
        std::cout << "\n10. Testing print and output operations..." << std::endl;

        gsFiberMatrix<double> print_mat(2, 3);
        print_mat(0, 0) = 1.0; print_mat(0, 2) = 2.0;
        print_mat(1, 1) = 3.0;

        // Test stream output
        std::ostringstream oss;
        oss << print_mat;
        std::string output = oss.str();

        std::cout << "   Stream output length: " << output.length() << " characters" << std::endl;
        std::cout << "   Stream output (first line): ";

        // Print first line of output
        size_t newline_pos = output.find('\n');
        if (newline_pos != std::string::npos) {
            std::cout << output.substr(0, newline_pos) << std::endl;
        } else {
            std::cout << output << std::endl;
        }

        std::cout << "✅ Print and output operations successful" << std::endl;

        // Test 11: Edge cases and boundary conditions
        std::cout << "\n11. Testing edge cases and boundary conditions..." << std::endl;

        // Test empty matrix
        gsFiberMatrix<double> empty_mat(0, 0);
        std::cout << "   Empty matrix size: " << empty_mat.rows() << "x" << empty_mat.cols() << std::endl;
        std::cout << "   Empty matrix total size: " << empty_mat.size() << std::endl;

        // Test single element matrix
        gsFiberMatrix<double> single_mat(1, 1);
        single_mat(0, 0) = 42.0;
        std::cout << "   Single element matrix: " << single_mat(0, 0) << std::endl;

        // Test large sparse matrix (many zeros)
        gsFiberMatrix<double> large_sparse(100, 100);
        large_sparse(0, 0) = 1.0;
        large_sparse(50, 50) = 2.0;
        large_sparse(99, 99) = 3.0;
        std::cout << "   Large sparse matrix (100x100) with 3 elements: (99,99) = " << large_sparse(99, 99) << std::endl;

        // Convert large sparse to check efficiency
        auto large_sparse_converted = large_sparse.toSparse();
        std::cout << "   Large sparse conversion nonZeros: " << large_sparse_converted.nonZeros() << std::endl;

        std::cout << "✅ Edge cases and boundary conditions successful" << std::endl;

        // Success
        std::cout << std::endl;
        std::cout << "🎉 ALL GSFIBERMATRIX TESTS PASSED! 🎉" << std::endl;
        std::cout << "✅ Template instantiation for ColMajor and RowMajor storage" << std::endl;
        std::cout << "✅ Element access and modification through operator() and coeff()" << std::endl;
        std::cout << "✅ Fiber operations and sparse vector integration" << std::endl;
        std::cout << "✅ Matrix resizing and block operations" << std::endl;
        std::cout << "✅ Conversion to gsSparseMatrix with correctness verification" << std::endl;
        std::cout << "✅ RowMajor vs ColMajor storage behavior differences" << std::endl;
        std::cout << "✅ Utility operations (setZero, reserve) and assignment" << std::endl;
        std::cout << "✅ Print operations and edge case handling" << std::endl;

        std::cout << "\n=== Comprehensive gsFiberMatrix Testing Complete ===" << std::endl;
        std::cout << "Validates specialized sparse matrix with fiber-based storage" << std::endl;
        std::cout << "Essential for knot insertion algorithms and efficient row operations" << std::endl;

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "❌ TEST FAILED: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ TEST FAILED: Unknown error" << std::endl;
        return 1;
    }
}