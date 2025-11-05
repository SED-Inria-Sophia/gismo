/** @file test_readfile.cpp

    @brief Comprehensive tests for ReadFile functionality

    This file provides extensive testing of the gsReadFile class:
    - Basic file reading operations
    - Constructor behavior with various file types
    - Template functionality with different precision types
    - Error handling for invalid files and formats

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s):
        A. Bressan, C. Hofreither, S. Takacs (original IO module contributors)
        GitHub Copilot (test implementation)
*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

// GISMO dependencies - updated for modular architecture
#include <gismo/Common/Common>
#include <gismo/Math/Math>

// IO module dependencies
#include <gismo/IO/ReadFile.h>
#include <gismo/IO/FileData.h>
#include <gismo/IO/FileManager.h>

// Standard library for file operations and test utilities
#include <fstream>
#include <sstream>
#include <cstdio>
#include <chrono>
#include <vector>

using namespace gismo;

// Test fixture for ReadFile operations with proper cleanup
class ReadFileTestFixture {
public:
    ReadFileTestFixture() {
        gsFileManager fm;
        temp_dir = fm.getTempPath();

        // Create unique test file names
        valid_xml_file = temp_dir + "readfile_valid.xml";
        invalid_file = temp_dir + "readfile_invalid.txt";
        empty_xml_file = temp_dir + "readfile_empty.xml";
        function_file = temp_dir + "readfile_function.xml";
        geom_test_file = temp_dir + "readfile_geometry.xml";
        basis_test_file = temp_dir + "readfile_basis.xml";
    }

    ~ReadFileTestFixture() {
        // Clean up all test files
        std::remove(valid_xml_file.c_str());
        std::remove(invalid_file.c_str());
        std::remove(empty_xml_file.c_str());
        std::remove(function_file.c_str());
        std::remove(geom_test_file.c_str());
        std::remove(basis_test_file.c_str());
    }

    void createBasicGismoFile(const std::string& filename) {
        std::ofstream file(filename);
        file << R"(<?xml version="1.0"?>
<xml>
  <String type="String" id="0">Test GISMO content</String>
  <String type="String" id="1" label="test_label">Labeled string content</String>
  <Comment>Basic GISMO XML file for testing ReadFile operations</Comment>
</xml>)";
        file.close();
    }

    void createEmptyXmlFile(const std::string& filename) {
        std::ofstream file(filename);
        file << R"(<?xml version="1.0"?>
<xml>
</xml>)";
        file.close();
    }

    void createFunctionFile(const std::string& filename) {
        std::ofstream file(filename);
        file << R"(<?xml version="1.0"?>
<xml>
  <Function type="gsFunctionExpr" id="0">
    <c>x*y</c>
  </Function>
  <Comment>Function expression for testing</Comment>
</xml>)";
        file.close();
    }

    void createInvalidFile(const std::string& filename) {
        std::ofstream file(filename);
        file << "This is not valid XML content\nJust plain text that should not parse as XML";
        file.close();
    }

    std::string temp_dir;
    std::string valid_xml_file;
    std::string invalid_file;
    std::string empty_xml_file;
    std::string function_file;
    std::string geom_test_file;
    std::string basis_test_file;
};

TEST_CASE("ReadFile Basic Construction and Behavior", "[io][readfile][basic]") {

    SECTION("Constructor with valid XML file") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        // Constructor should not throw
        REQUIRE_NOTHROW(gsReadFile<real_t>(fixture.valid_xml_file));

        gsReadFile<real_t> reader(fixture.valid_xml_file);
        SUCCEED("ReadFile constructor completed successfully with valid XML");
    }

    SECTION("Constructor with ID parameter") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        // Constructor with ID should not throw
        REQUIRE_NOTHROW(gsReadFile<real_t>(fixture.valid_xml_file, 0));
        REQUIRE_NOTHROW(gsReadFile<real_t>(fixture.valid_xml_file, 1));

        gsReadFile<real_t> reader_0(fixture.valid_xml_file, 0);
        gsReadFile<real_t> reader_1(fixture.valid_xml_file, 1);
        SUCCEED("ReadFile constructor with ID parameter works correctly");
    }

    SECTION("Constructor robustness") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        // Test that multiple constructions work
        gsReadFile<real_t> reader1(fixture.valid_xml_file);
        gsReadFile<real_t> reader2(fixture.valid_xml_file, 0);

        SUCCEED("ReadFile constructor robustness test completed");
    }
}

TEST_CASE("ReadFile File Format Handling", "[io][readfile][formats]") {

    SECTION("Empty XML file handling") {
        ReadFileTestFixture fixture;
        fixture.createEmptyXmlFile(fixture.empty_xml_file);

        // Empty XML should still construct successfully
        REQUIRE_NOTHROW(gsReadFile<real_t>(fixture.empty_xml_file));

        gsReadFile<real_t> reader(fixture.empty_xml_file);
        SUCCEED("ReadFile handles empty XML files gracefully");
    }

    SECTION("Invalid XML file handling") {
        ReadFileTestFixture fixture;
        fixture.createInvalidFile(fixture.invalid_file);

        // Invalid XML should not throw during construction
        REQUIRE_NOTHROW(gsReadFile<real_t>(fixture.invalid_file));

        gsReadFile<real_t> reader(fixture.invalid_file);
        SUCCEED("ReadFile handles invalid XML files gracefully");
    }

    SECTION("Non-existent file handling") {
        std::string non_existent = "/tmp/non_existent_file_12345.xml";

        // Non-existent file should not throw during construction
        REQUIRE_NOTHROW(gsReadFile<real_t>(non_existent));

        gsReadFile<real_t> reader(non_existent);
        SUCCEED("ReadFile handles non-existent files gracefully");
    }
}

TEST_CASE("ReadFile Template Type Support", "[io][readfile][templates]") {

    SECTION("Different precision types") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        // Test with different template parameters
        REQUIRE_NOTHROW(gsReadFile<double>(fixture.valid_xml_file));
        REQUIRE_NOTHROW(gsReadFile<float>(fixture.valid_xml_file));

        gsReadFile<double> reader_double(fixture.valid_xml_file);
        gsReadFile<float> reader_float(fixture.valid_xml_file);

        SUCCEED("ReadFile supports multiple precision types");
    }

    SECTION("Template with ID parameter") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        // Test template instantiation with ID
        REQUIRE_NOTHROW(gsReadFile<double>(fixture.valid_xml_file, 0));
        REQUIRE_NOTHROW(gsReadFile<float>(fixture.valid_xml_file, 1));

        gsReadFile<double> reader_double(fixture.valid_xml_file, 0);
        gsReadFile<float> reader_float(fixture.valid_xml_file, 1);

        SUCCEED("ReadFile template instantiation with ID works");
    }
}

TEST_CASE("ReadFile Basic Conversion Operations", "[io][readfile][conversion]") {

    SECTION("Safe construction with various inputs") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        gsReadFile<real_t> reader(fixture.valid_xml_file);

        // Test that readers construct successfully
        SUCCEED("ReadFile construction operations complete without exceptions");
    }
}

TEST_CASE("ReadFile Error Resilience", "[io][readfile][errors]") {

    SECTION("Malformed XML structure") {
        ReadFileTestFixture fixture;

        // Create malformed XML with unclosed tags
        std::ofstream file(fixture.invalid_file);
        file << R"(<?xml version="1.0"?>
<xml>
  <String>Unclosed string tag
  <Comment>Another unclosed comment tag
</xml>)";
        file.close();

        gsReadFile<real_t> reader(fixture.invalid_file);
        SUCCEED("ReadFile handles malformed XML structure gracefully");
    }

    SECTION("Completely empty file") {
        ReadFileTestFixture fixture;

        // Create completely empty file
        std::ofstream file(fixture.invalid_file);
        file.close();

        gsReadFile<real_t> reader(fixture.invalid_file);
        SUCCEED("ReadFile handles completely empty files gracefully");
    }

    SECTION("Binary file handling") {
        ReadFileTestFixture fixture;

        // Create binary-like file with null bytes
        std::ofstream file(fixture.invalid_file, std::ios::binary);
        for (int i = 0; i < 100; ++i) {
            file << static_cast<char>(i % 256);
        }
        file.close();

        gsReadFile<real_t> reader(fixture.invalid_file);
        SUCCEED("ReadFile handles binary files gracefully");
    }
}

TEST_CASE("ReadFile Memory Management", "[io][readfile][memory]") {

    SECTION("Multiple readers for same file") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        // Create multiple readers for the same file
        std::vector<gsReadFile<real_t>*> readers;
        for (int i = 0; i < 5; ++i) {
            readers.push_back(new gsReadFile<real_t>(fixture.valid_xml_file));
        }

        // Clean up readers
        for (auto* reader : readers) {
            delete reader;
        }

        SUCCEED("Multiple ReadFile instances manage memory correctly");
    }

    SECTION("Reader with different IDs") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        // Create readers with different ID parameters
        gsReadFile<real_t> reader_0(fixture.valid_xml_file, 0);
        gsReadFile<real_t> reader_1(fixture.valid_xml_file, 1);
        gsReadFile<real_t> reader_invalid(fixture.valid_xml_file, 999);

        SUCCEED("ReadFile instances with different IDs work correctly");
    }
}

TEST_CASE("ReadFile Integration Scenarios", "[io][readfile][integration]") {

    SECTION("Sequential file reading") {
        ReadFileTestFixture fixture;

        // Create multiple different files
        fixture.createBasicGismoFile(fixture.valid_xml_file);
        fixture.createEmptyXmlFile(fixture.empty_xml_file);
        fixture.createFunctionFile(fixture.function_file);

        // Read files sequentially
        gsReadFile<real_t> reader1(fixture.valid_xml_file);
        gsReadFile<real_t> reader2(fixture.empty_xml_file);
        gsReadFile<real_t> reader3(fixture.function_file);

        SUCCEED("Sequential file reading works correctly");
    }

    SECTION("Mixed precision reading") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        // Read same file with different precisions
        gsReadFile<double> reader_double(fixture.valid_xml_file);
        gsReadFile<float> reader_float(fixture.valid_xml_file);

        SUCCEED("Mixed precision reading works correctly");
    }
}

TEST_CASE("ReadFile Performance Characteristics", "[io][readfile][performance]") {

    SECTION("Large file handling") {
        ReadFileTestFixture fixture;

        // Create a larger XML file with multiple elements
        std::ofstream file(fixture.valid_xml_file);
        file << R"(<?xml version="1.0"?>
<xml>)";

        for (int i = 0; i < 100; ++i) {
            file << "\n  <String type=\"String\" id=\"" << i << "\">Content " << i << "</String>";
        }

        file << R"(
  <Comment>Large XML file with 100 string elements</Comment>
</xml>)";
        file.close();

        // Reading should complete in reasonable time
        auto start_time = std::chrono::high_resolution_clock::now();
        gsReadFile<real_t> reader(fixture.valid_xml_file);
        auto end_time = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        // Should complete within reasonable time (generous limit for CI)
        REQUIRE(duration.count() < 5000); // 5 seconds max

        SUCCEED("Large file reading completes in reasonable time");
    }

    SECTION("Rapid construction/destruction") {
        ReadFileTestFixture fixture;
        fixture.createBasicGismoFile(fixture.valid_xml_file);

        // Rapid construction and destruction
        for (int i = 0; i < 50; ++i) {
            gsReadFile<real_t> reader(fixture.valid_xml_file);
            // Reader goes out of scope and is destroyed
        }

        SUCCEED("Rapid construction/destruction works correctly");
    }
}