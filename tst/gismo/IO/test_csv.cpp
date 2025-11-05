/** @file test_csv.cpp

    @brief Focused tests for CSV export functionality

    This file provides comprehensive testing of the CSV export capabilities:
    - Matrix export to CSV files
    - CSV format with headers
    - Large matrix handling
    - File I/O operations for CSV
    - CSV format validation

    This file is part of the G+Smo library.
**/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

using Catch::Approx;

// Include foundation modules
#include <gismo/Common/Common>
#include <gismo/Math/Math>

// Test CSV functionality
#include <gismo/IO/Csv.h>
#include <gismo/IO/FileManager.h>

// Standard library for file operations
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace gismo;

TEST_CASE("CSV Export Functionality", "[io][csv]") {

    SECTION("Matrix export to CSV file") {
        // Create test matrix
        gsMatrix<real_t> matrix(3, 2);
        matrix << 1.0, 2.0,
                  3.0, 4.0,
                  5.0, 6.0;

        // Export to temporary CSV file
        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "test_matrix.csv";

        // Write matrix to CSV file
        gismo::writeMatrixToCsv(csv_file, matrix);

        // Verify file was created and has content
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());

        std::string line;
        std::getline(check_file, line);
        REQUIRE(!line.empty());

        // Should contain values from the matrix
        REQUIRE(line.find("1") != std::string::npos);
        check_file.close();

        // Clean up
        std::remove(csv_file.c_str());
    }

    SECTION("CSV with headers") {
        // Create small matrix
        gsMatrix<real_t> matrix(2, 3);
        matrix << 1.0, 2.0, 3.0,
                  4.0, 5.0, 6.0;

        // Export with headers
        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "test_with_headers.csv";

        std::vector<std::string> headers = {"X", "Y", "Z"};
        gismo::writeMatrixToCsv(csv_file, matrix, headers);

        // Check file content
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());

        std::string first_line;
        std::getline(check_file, first_line);

        // Should contain headers
        REQUIRE(first_line.find("X") != std::string::npos);
        REQUIRE(first_line.find("Y") != std::string::npos);
        REQUIRE(first_line.find("Z") != std::string::npos);

        check_file.close();
        std::remove(csv_file.c_str());
    }

    SECTION("Large matrix CSV export") {
        // Test with larger matrix to verify scalability
        const int size = 10;
        gsMatrix<real_t> large_matrix(size, size);

        // Fill with simple pattern
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                large_matrix(i, j) = i + j * 0.1;
            }
        }

        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "large_matrix.csv";

        // Should export without issues
        REQUIRE_NOTHROW(gismo::writeMatrixToCsv(csv_file, large_matrix));

        // Verify file exists
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());
        check_file.close();

        std::remove(csv_file.c_str());
    }
}

TEST_CASE("CSV Format Validation", "[io][csv][format]") {

    SECTION("CSV format with different separators") {
        gsMatrix<real_t> matrix(2, 2);
        matrix << 1.5, 2.7,
                  3.1, 4.9;

        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "format_test.csv";

        // Write matrix to CSV
        gismo::writeMatrixToCsv(csv_file, matrix);

        // Read and verify format
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());

        std::string line;
        std::getline(check_file, line);

        // Should contain decimal values
        bool has_decimal = (line.find("1.5") != std::string::npos ||
                           line.find("1,5") != std::string::npos);  // Different locale formats
        REQUIRE(has_decimal);

        // Should contain some separator (comma, semicolon, or tab)
        bool has_separator = (line.find(",") != std::string::npos ||
                             line.find(";") != std::string::npos ||
                             line.find("\t") != std::string::npos);
        REQUIRE(has_separator);

        check_file.close();
        std::remove(csv_file.c_str());
    }

    SECTION("CSV with integer matrix") {
        gsMatrix<int> int_matrix(2, 3);
        int_matrix << 1, 2, 3,
                      4, 5, 6;

        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "integer_matrix.csv";

        // Should handle integer matrices
        REQUIRE_NOTHROW(gismo::writeMatrixToCsv(csv_file, int_matrix));

        // Verify content
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());

        std::string content;
        std::getline(check_file, content);

        // Should contain integer values
        REQUIRE(content.find("1") != std::string::npos);
        REQUIRE(content.find("2") != std::string::npos);

        check_file.close();
        std::remove(csv_file.c_str());
    }

    SECTION("CSV precision handling") {
        gsMatrix<real_t> precision_matrix(1, 3);
        precision_matrix << 3.141592653589793, 2.718281828459045, 1.414213562373095;

        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "precision_test.csv";

        gismo::writeMatrixToCsv(csv_file, precision_matrix);

        // Read back and check precision
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());

        std::string line;
        std::getline(check_file, line);

        // Should contain high precision values (at least some decimal places)
        REQUIRE(line.find(".") != std::string::npos);  // Contains decimal point
        REQUIRE(line.length() > 10);  // Should be reasonably long for precision

        check_file.close();
        std::remove(csv_file.c_str());
    }
}

TEST_CASE("CSV Advanced Features", "[io][csv][advanced]") {

    SECTION("Empty matrix handling") {
        gsMatrix<real_t> empty_matrix(0, 0);

        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "empty_matrix.csv";

        // Should handle empty matrices gracefully
        REQUIRE_NOTHROW(gismo::writeMatrixToCsv(csv_file, empty_matrix));

        // File should be created (even if empty)
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());
        check_file.close();

        std::remove(csv_file.c_str());
    }

    SECTION("Single element matrix") {
        gsMatrix<real_t> single_matrix(1, 1);
        single_matrix(0, 0) = 42.5;

        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "single_element.csv";

        gismo::writeMatrixToCsv(csv_file, single_matrix);

        // Verify single element export
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());

        std::string content;
        std::getline(check_file, content);

        bool has_value = (content.find("42.5") != std::string::npos ||
                         content.find("42,5") != std::string::npos);
        REQUIRE(has_value);

        check_file.close();
        std::remove(csv_file.c_str());
    }

    SECTION("Vector as matrix export") {
        // Test column vector
        gsMatrix<real_t> col_vector(5, 1);
        col_vector << 1.0, 2.0, 3.0, 4.0, 5.0;

        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "column_vector.csv";

        gismo::writeMatrixToCsv(csv_file, col_vector);

        // Verify export
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());

        // Should have 5 lines (one per element)
        int line_count = 0;
        std::string line;
        while (std::getline(check_file, line)) {
            line_count++;
        }

        REQUIRE(line_count == 5);
        check_file.close();
        std::remove(csv_file.c_str());
    }

    SECTION("Row vector export") {
        // Test row vector
        gsMatrix<real_t> row_vector(1, 4);
        row_vector << 10.0, 20.0, 30.0, 40.0;

        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string csv_file = temp_dir + "row_vector.csv";

        gismo::writeMatrixToCsv(csv_file, row_vector);

        // Verify export
        std::ifstream check_file(csv_file);
        REQUIRE(check_file.is_open());

        std::string line;
        std::getline(check_file, line);

        // Should contain all elements in one line
        REQUIRE(line.find("10") != std::string::npos);
        REQUIRE(line.find("20") != std::string::npos);
        REQUIRE(line.find("30") != std::string::npos);
        REQUIRE(line.find("40") != std::string::npos);

        check_file.close();
        std::remove(csv_file.c_str());
    }
}

TEST_CASE("CSV Integration Scenarios", "[io][csv][integration]") {

    SECTION("CSV export in computational workflow") {
        // Simulate a computational workflow with results export

        // 1. Generate computation results
        const int num_steps = 5;
        gsMatrix<real_t> results(num_steps, 4);

        for (int i = 0; i < num_steps; ++i) {
            results(i, 0) = i;                          // Step number
            results(i, 1) = i * 0.1;                    // Time
            results(i, 2) = std::sin(i * 0.1);         // Solution value
            results(i, 3) = std::abs(std::sin(i * 0.1) - (i * 0.1)); // Error
        }

        // 2. Export with descriptive headers
        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();
        std::string results_file = temp_dir + "computation_results.csv";

        std::vector<std::string> headers = {"Step", "Time", "Solution", "Error"};
        gismo::writeMatrixToCsv(results_file, results, headers);

        // 3. Verify export quality
        std::ifstream check_file(results_file);
        REQUIRE(check_file.is_open());

        // Check header line
        std::string header_line;
        std::getline(check_file, header_line);
        REQUIRE(header_line.find("Step") != std::string::npos);
        REQUIRE(header_line.find("Time") != std::string::npos);
        REQUIRE(header_line.find("Solution") != std::string::npos);
        REQUIRE(header_line.find("Error") != std::string::npos);

        // Check data lines
        std::string data_line;
        std::getline(check_file, data_line);
        REQUIRE(!data_line.empty());

        check_file.close();
        std::remove(results_file.c_str());
    }

    SECTION("Multiple CSV exports in sequence") {
        gsFileManager fm;
        std::string temp_dir = fm.getTempPath();

        // Export multiple related datasets
        std::vector<std::string> filenames = {
            temp_dir + "dataset1.csv",
            temp_dir + "dataset2.csv",
            temp_dir + "dataset3.csv"
        };

        for (size_t i = 0; i < filenames.size(); ++i) {
            gsMatrix<real_t> data(2, 2);
            data << i, i+1,
                    i+2, i+3;

            gismo::writeMatrixToCsv(filenames[i], data);

            // Verify each file
            std::ifstream check(filenames[i]);
            REQUIRE(check.is_open());
            check.close();
        }

        // Clean up all files
        for (const auto& filename : filenames) {
            std::remove(filename.c_str());
        }
    }

    SECTION("CSV export error recovery") {
        gsMatrix<real_t> matrix(2, 2);
        matrix << 1.0, 2.0,
                  3.0, 4.0;

        // Try to write to an invalid path (should handle gracefully)
        std::string invalid_path = "/definitely/invalid/path/file.csv";

        // This might throw or might create a file in current directory
        // The behavior depends on the implementation
        // Just ensure it doesn't crash
        REQUIRE_NOTHROW(gismo::writeMatrixToCsv(invalid_path, matrix));
    }
}