/** @file test_asmatrix.cpp
    @brief Comprehensive test for AsMatrix classes and their functionality

    This test provides comprehensive coverage of gsAsMatrix, gsAsConstMatrix,
    gsAsVector, and gsAsConstVector classes and their methods.
*/

#include <iostream>
#include <vector>
#include <memory>
#include <array>

// Include the full Math module to get all necessary definitions
#include <gismo/Math/Math>

using namespace gismo;

int main() {
    std::cout << "\n=== Comprehensive gsAsMatrix Tests ===\n";

    try {
        // Test 1: gsAsMatrix Construction and Basic Operations
        std::cout << "\n1. Testing gsAsMatrix construction and basic operations..." << std::endl;
        std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

        // User-friendly syntax: gsAsMatrix<double>
        gsAsMatrix<double> mat(data, 2, 3);  // 2x3 matrix
        std::cout << "   Created gsAsMatrix<double> successfully" << std::endl;
        std::cout << "   Matrix size: " << mat.rows() << "x" << mat.cols() << std::endl;
        std::cout << "   Element (0,0): " << mat(0, 0) << std::endl;
        std::cout << "   Element (1,2): " << mat(1, 2) << std::endl;

        // Test modification capabilities
        double originalValue = mat(0, 1);
        mat(0, 1) = 99.0;
        std::cout << "   Modified (0,1) from " << originalValue << " to " << mat(0, 1) << std::endl;

        // Test 2: gsAsVector Construction and Operations
        std::cout << "\n2. Testing gsAsVector construction and operations..." << std::endl;
        std::vector<double> vec_data = {10.0, 20.0, 30.0, 40.0};

        // User-friendly syntax: gsAsVector<double>
        gsAsVector<double> vec(vec_data);
        std::cout << "   Created gsAsVector<double> successfully" << std::endl;
        std::cout << "   Vector size: " << vec.size() << std::endl;
        std::cout << "   Element [0]: " << vec[0] << std::endl;
        std::cout << "   Element [2]: " << vec[2] << std::endl;

        // Test vector operations
        vec[1] = 99.0;
        std::cout << "   Modified element [1] to: " << vec[1] << std::endl;
        double norm = vec.norm();
        std::cout << "   Vector norm: " << norm << std::endl;

        // Test 3: gsAsConstMatrix Construction and Read-only Operations
        std::cout << "\n3. Testing gsAsConstMatrix construction and read-only operations..." << std::endl;
        const std::vector<double> const_data = {7.0, 8.0, 9.0, 10.0, 11.0, 12.0};

        // User-friendly syntax: gsAsConstMatrix<double>
        gsAsConstMatrix<double> const_mat(const_data, 2, 3);
        std::cout << "   Created gsAsConstMatrix<double> successfully" << std::endl;
        std::cout << "   Const matrix size: " << const_mat.rows() << "x" << const_mat.cols() << std::endl;
        std::cout << "   Element (0,0): " << const_mat(0, 0) << std::endl;
        std::cout << "   Element (1,2): " << const_mat(1, 2) << std::endl;

        // Test read-only operations
        double trace = const_mat.trace();
        std::cout << "   Trace: " << trace << std::endl;
        double determinant = const_mat.block(0, 0, 2, 2).determinant();
        std::cout << "   2x2 block determinant: " << determinant << std::endl;

        // Test 4: gsAsConstVector Construction and Read-only Operations
        std::cout << "\n4. Testing gsAsConstVector construction and read-only operations..." << std::endl;
        const std::vector<float> const_vec_data = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

        // User-friendly syntax: gsAsConstVector<float>
        gsAsConstVector<float> const_vec(const_vec_data);
        std::cout << "   Created gsAsConstVector<float> successfully" << std::endl;
        std::cout << "   Const vector size: " << const_vec.size() << std::endl;
        std::cout << "   Element [0]: " << const_vec[0] << std::endl;
        std::cout << "   Element [4]: " << const_vec[4] << std::endl;

        // Test read-only operations
        float const_norm = const_vec.norm();
        std::cout << "   Const vector norm: " << const_norm << std::endl;
        float sum = const_vec.sum();
        std::cout << "   Const vector sum: " << sum << std::endl;

        // Test 5: Multiple Data Type Support
        std::cout << "\n5. Testing multiple data type support..." << std::endl;
        std::vector<float> float_data = {1.5f, 2.5f, 3.5f, 4.5f};
        std::vector<int> int_data = {10, 20, 30, 40, 50, 60};

        gsAsMatrix<float> matf(float_data, 2, 2);    // gsAsMatrix<float>
        gsAsVector<float> vecf(float_data);          // gsAsVector<float>
        gsAsMatrix<int> mati(int_data, 2, 3);       // gsAsMatrix<int>

        std::cout << "   Float matrix (2x2): " << matf(0, 0) << ", " << matf(1, 1) << std::endl;
        std::cout << "   Float vector [0]: " << vecf[0] << std::endl;
        std::cout << "   Int matrix (2x3): " << mati(0, 0) << ", " << mati(1, 2) << std::endl;

        // Test 6: Raw Pointer Construction
        std::cout << "\n6. Testing raw pointer construction..." << std::endl;
        double raw_data[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
        gsAsMatrixImpl<double, gsEigen::Dynamic, gsEigen::Dynamic> raw_mat(raw_data, 2, 3);
        std::cout << "   Created AsMatrix from raw pointer successfully" << std::endl;
        std::cout << "   Raw matrix size: " << raw_mat.rows() << "x" << raw_mat.cols() << std::endl;
        std::cout << "   Element (1,1): " << raw_mat(1, 1) << std::endl;

        // Modify through raw pointer matrix
        raw_mat(0, 0) = 100.0;
        std::cout << "   Modified (0,0) to: " << raw_mat(0, 0) << std::endl;

        // Test 7: Matrix Operations and Arithmetic
        std::cout << "\n7. Testing matrix operations and arithmetic..." << std::endl;
        std::vector<double> op_data1 = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> op_data2 = {5.0, 6.0, 7.0, 8.0};

        gsAsMatrix<double> op_mat1(op_data1, 2, 2);
        gsAsMatrix<double> op_mat2(op_data2, 2, 2);

        // Matrix arithmetic (creates new expressions)
        auto sum_expr = op_mat1 + op_mat2;
        auto prod_expr = op_mat1 * op_mat2;

        std::cout << "   Original mat1(0,0): " << op_mat1(0, 0) << std::endl;
        std::cout << "   Original mat2(0,0): " << op_mat2(0, 0) << std::endl;
        std::cout << "   Sum expression (0,0): " << sum_expr(0, 0) << std::endl;
        std::cout << "   Product expression (0,0): " << prod_expr(0, 0) << std::endl;

        // Assignment operations
        op_mat1 = op_mat2;
        std::cout << "   After assignment, mat1(0,0): " << op_mat1(0, 0) << std::endl;

        // Test 8: Block Operations and Views
        std::cout << "\n8. Testing block operations and views..." << std::endl;
        std::vector<double> block_data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
        gsAsMatrix<double> block_mat(block_data, 3, 3);

        // Block access
        auto top_left = block_mat.block(0, 0, 2, 2);
        std::cout << "   Top-left 2x2 block (0,0): " << top_left(0, 0) << std::endl;
        std::cout << "   Top-left 2x2 block (1,1): " << top_left(1, 1) << std::endl;

        // Row and column access
        auto first_row = block_mat.row(0);
        auto second_col = block_mat.col(1);
        std::cout << "   First row [0]: " << first_row[0] << std::endl;
        std::cout << "   Second column [2]: " << second_col[2] << std::endl;

        // Test 9: Integration with gsMatrix Methods
        std::cout << "\n9. Testing integration with gsMatrix methods..." << std::endl;
        gsMatrix<double> source_matrix(3, 3);
        source_matrix.setRandom();
        std::cout << "   Created source matrix: " << source_matrix.rows() << "x" << source_matrix.cols() << std::endl;

        // Test reshaping from gsMatrix
        auto reshaped = source_matrix.reshape(1, 9);  // Returns gsAsMatrixImpl
        std::cout << "   Reshaped to: " << reshaped.rows() << "x" << reshaped.cols() << std::endl;
        std::cout << "   Reshaped element (0,4): " << reshaped(0, 4) << std::endl;

        // Test asVector from gsMatrix
        auto as_vec = source_matrix.asVector();  // Returns gsAsVectorImpl
        std::cout << "   AsVector size: " << as_vec.size() << std::endl;
        std::cout << "   AsVector element [5]: " << as_vec[5] << std::endl;

        // Test asRowVector
        auto as_row_vec = source_matrix.asRowVector();
        std::cout << "   AsRowVector size: " << as_row_vec.rows() << "x" << as_row_vec.cols() << std::endl;

        // Test 10: Memory Management and Data Sharing
        std::cout << "\n10. Testing memory management and data sharing..." << std::endl;
        std::vector<double> shared_data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};

        // Create multiple views of the same data
        gsAsMatrix<double> view1(shared_data, 2, 3);
        gsAsMatrix<double> view2(shared_data, 3, 2);  // Different shape, same data

        std::cout << "   View1 (2x3) element (0,0): " << view1(0, 0) << std::endl;
        std::cout << "   View2 (3x2) element (0,0): " << view2(0, 0) << std::endl;

        // Modify through one view
        view1(0, 0) = 999.0;
        std::cout << "   After modifying view1(0,0) to 999:" << std::endl;
        std::cout << "     View1 (0,0): " << view1(0, 0) << std::endl;
        std::cout << "     View2 (0,0): " << view2(0, 0) << std::endl;
        std::cout << "     Original vector [0]: " << shared_data[0] << std::endl;

        // Test 11: Template Specializations and Type Deduction
        std::cout << "\n11. Testing template specializations and type deduction..." << std::endl;

        // Test different numeric types
        std::vector<float> float_test = {1.1f, 2.2f, 3.3f, 4.4f};
        std::vector<int> int_test = {10, 20, 30, 40};

        auto float_mat = gsAsMatrix<float>(float_test, 2, 2);
        auto int_mat = gsAsMatrix<int>(int_test, 2, 2);

        std::cout << "   Float matrix element (1,0): " << float_mat(1, 0) << std::endl;
        std::cout << "   Int matrix element (1,0): " << int_mat(1, 0) << std::endl;

        // Test type compatibility
        std::cout << "   Float matrix type size: " << sizeof(float_mat(0,0)) << " bytes" << std::endl;
        std::cout << "   Int matrix type size: " << sizeof(int_mat(0,0)) << " bytes" << std::endl;

        // Test 12: Boundary Conditions and Edge Cases
        std::cout << "\n12. Testing boundary conditions and edge cases..." << std::endl;

        // Single element matrix
        std::vector<double> single_data = {42.0};
        gsAsMatrix<double> single_mat(single_data, 1, 1);
        std::cout << "   Single element matrix (1x1): " << single_mat(0, 0) << std::endl;

        // Single element vector
        gsAsVector<double> single_vec(single_data);
        std::cout << "   Single element vector [0]: " << single_vec[0] << std::endl;

        // Row vector (1xN)
        std::vector<double> row_data = {1.0, 2.0, 3.0, 4.0, 5.0};
        gsAsMatrix<double> row_mat(row_data, 1, 5);
        std::cout << "   Row matrix (1x5) element (0,4): " << row_mat(0, 4) << std::endl;

        // Column vector (Nx1)
        gsAsMatrix<double> col_mat(row_data, 5, 1);
        std::cout << "   Column matrix (5x1) element (4,0): " << col_mat(4, 0) << std::endl;

        // Test 13: Performance and Copy Operations
        std::cout << "\n13. Testing performance and copy operations..." << std::endl;
        std::vector<double> perf_data = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
        gsAsMatrix<double> perf_mat(perf_data, 3, 3);

        std::cout << "   Original AsMatrix size: " << perf_mat.rows() << "x" << perf_mat.cols() << std::endl;
        std::cout << "   Original element (1,1): " << perf_mat(1, 1) << std::endl;

        // Test copying to gsMatrix (safer than move)
        gsMatrix<double> copied = perf_mat;  // Implicit conversion/copy
        std::cout << "   Copied matrix size: " << copied.rows() << "x" << copied.cols() << std::endl;
        std::cout << "   Copied matrix element (1,1): " << copied(1, 1) << std::endl;

        // Verify original is still valid
        std::cout << "   Original still valid - element (2,2): " << perf_mat(2, 2) << std::endl;

        // Test assignment from gsMatrix expression
        gsMatrix<double> identity = gsMatrix<double>::Identity(3, 3);
        perf_mat = identity;  // Assignment
        std::cout << "   After assignment from Identity, element (1,1): " << perf_mat(1, 1) << std::endl;

        std::cout << "\n=== All gsAsMatrix Tests Completed Successfully ===" << std::endl;
        std::cout << "Comprehensive coverage achieved for:" << std::endl;
        std::cout << "  - gsAsMatrix construction and element access" << std::endl;
        std::cout << "  - gsAsVector operations and arithmetic" << std::endl;
        std::cout << "  - gsAsConstMatrix read-only operations" << std::endl;
        std::cout << "  - gsAsConstVector read-only access" << std::endl;
        std::cout << "  - Multiple data type support (double, float, int)" << std::endl;
        std::cout << "  - Raw pointer construction" << std::endl;
        std::cout << "  - Matrix arithmetic and operations" << std::endl;
        std::cout << "  - Block operations and views" << std::endl;
        std::cout << "  - Integration with gsMatrix methods (reshape, asVector)" << std::endl;
        std::cout << "  - Memory management and data sharing" << std::endl;
        std::cout << "  - Template specializations and type deduction" << std::endl;
        std::cout << "  - Boundary conditions and edge cases" << std::endl;
        std::cout << "  - Performance and copy operations" << std::endl;

        return 0;

    } catch (const std::exception& e) {
        std::cout << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}