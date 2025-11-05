/** @file test_gzstreamer.cpp

    @brief Focused tests for gzstreamer compression functionality

    This file provides targeted testing of:
    - Compressed stream I/O operations (read/write)
    - Stream state management and error handling
    - Integration with standard C++ iostream interface
    - Temporary file operations for safe testing

    This file is part of the G+Smo library.
**/

#include <catch2/catch_test_macros.hpp>

// Include foundation modules
#include <gismo/Common/Common>

// Test the gzstreamer functionality
#include <gismo/IO/gzstreamer/gzstream.h>

// Include FileManager for temporary file operations
#include <gismo/IO/FileManager.h>

#include <fstream>
#include <sstream>
#include <string>

using namespace gismo;

// Test fixture for safe temporary file operations
class GZStreamTestFixture {
public:
    GZStreamTestFixture() {
        // Use FileManager for safe temporary file management
        gsFileManager fm;
        temp_dir = fm.getTempPath();

        // Create unique test file names
        test_file_gz = temp_dir + "/gzstream_test.gz";
        test_file_txt = temp_dir + "/gzstream_test.txt";
    }

    ~GZStreamTestFixture() {
        // Clean up test files
        std::remove(test_file_gz.c_str());
        std::remove(test_file_txt.c_str());
    }

    std::string temp_dir;
    std::string test_file_gz;
    std::string test_file_txt;

    // Standard test data
    const std::string test_data = "GISMO Library\nCompression Test Data\n12345\n3.14159\nEnd of test data.\n";
};

TEST_CASE("GZStream Basic I/O Operations", "[io][gzstream]") {

    SECTION("Write and read compressed data") {
        GZStreamTestFixture fixture;

        // Write data to compressed file
        {
            ogzstream out_gz(fixture.test_file_gz.c_str());
            REQUIRE(out_gz.rdbuf()->is_open());

            out_gz << fixture.test_data;
            REQUIRE(out_gz.good());

            // Explicit close to ensure data is written
            out_gz.close();
        }

        // Read data back from compressed file
        {
            igzstream in_gz(fixture.test_file_gz.c_str());
            REQUIRE(in_gz.rdbuf()->is_open());

            std::string read_data;
            std::string line;
            while (std::getline(in_gz, line)) {
                read_data += line + "\n";
            }

            REQUIRE(read_data == fixture.test_data);
            REQUIRE(in_gz.eof());  // Should reach EOF naturally
        }
    }

    SECTION("Stream interface compatibility") {
        GZStreamTestFixture fixture;

        // Test that gzstream works with standard stream operations
        ogzstream out_gz(fixture.test_file_gz.c_str());
        REQUIRE(out_gz.rdbuf()->is_open());

        // Test various stream operations
        out_gz << "Integer: " << 42 << std::endl;
        out_gz << "Float: " << 3.14159 << std::endl;
        out_gz << "String: " << "GISMO" << std::endl;

        REQUIRE(out_gz.good());
        out_gz.close();

        // Read back and validate
        igzstream in_gz(fixture.test_file_gz.c_str());
        REQUIRE(in_gz.rdbuf()->is_open());

        std::string content;
        char buffer[256];
        while (in_gz.getline(buffer, sizeof(buffer))) {
            content += std::string(buffer) + "\n";
        }

        REQUIRE(content.find("Integer: 42") != std::string::npos);
        REQUIRE(content.find("Float: 3.14159") != std::string::npos);
        REQUIRE(content.find("String: GISMO") != std::string::npos);
    }
}

TEST_CASE("GZStream State Management", "[io][gzstream][state]") {

    SECTION("Stream state validation") {
        GZStreamTestFixture fixture;

        // Test output stream states
        ogzstream out_gz;
        REQUIRE_FALSE(out_gz.rdbuf()->is_open());  // Should not be open initially

        out_gz.open(fixture.test_file_gz.c_str());
        REQUIRE(out_gz.rdbuf()->is_open());        // Should be open after open()
        REQUIRE(out_gz.good());           // Should be in good state

        out_gz.close();
        REQUIRE_FALSE(out_gz.rdbuf()->is_open()); // Should be closed after close()
    }

    SECTION("Error handling for invalid files") {
        // Test behavior with invalid file paths
        igzstream in_gz("/invalid/path/nonexistent.gz");

        // Should handle invalid paths gracefully - file won't open
        REQUIRE_FALSE(in_gz.rdbuf()->is_open());

        // Try to read from unopened stream - should not work
        std::string content;
        std::getline(in_gz, content);
        REQUIRE(content.empty()); // Should not read anything from invalid file
    }

    SECTION("Stream operations after close") {
        GZStreamTestFixture fixture;

        ogzstream out_gz(fixture.test_file_gz.c_str());
        REQUIRE(out_gz.rdbuf()->is_open());

        // Write some data and close
        out_gz << "test data";
        out_gz.close();
        REQUIRE_FALSE(out_gz.rdbuf()->is_open()); // Should be closed

        // After close, stream should handle additional operations gracefully
        // (behavior may vary, but should not crash)
        out_gz << "more data"; // This should be handled gracefully

        // Verify the file contains original data
        igzstream in_gz(fixture.test_file_gz.c_str());
        std::string content;
        std::getline(in_gz, content);
        REQUIRE(content == "test data");
    }
}

TEST_CASE("GZStream Compression Effectiveness", "[io][gzstream][compression]") {

    SECTION("Compression actually reduces file size") {
        GZStreamTestFixture fixture;

        // Create large, repetitive test data (compresses well)
        std::string large_data;
        for (int i = 0; i < 1000; ++i) {
            large_data += "This is repetitive test data for compression testing. ";
        }

        // Write uncompressed version
        {
            std::ofstream uncompressed(fixture.test_file_txt);
            uncompressed << large_data;
        }

        // Write compressed version
        {
            ogzstream compressed(fixture.test_file_gz.c_str());
            compressed << large_data;
        }

        // Compare file sizes - compressed should be smaller
        std::ifstream uncompressed(fixture.test_file_txt, std::ios::binary | std::ios::ate);
        std::ifstream compressed(fixture.test_file_gz, std::ios::binary | std::ios::ate);

        auto uncompressed_size = uncompressed.tellg();
        auto compressed_size = compressed.tellg();

        REQUIRE(compressed_size < uncompressed_size);
        REQUIRE(compressed_size > 0); // Sanity check

        // Verify we can read back the same data
        igzstream in_gz(fixture.test_file_gz.c_str());
        std::string read_back;
        std::string line;
        while (std::getline(in_gz, line)) {
            read_back += line + " "; // getline removes newlines, spaces separate lines
        }

        // Should contain the same content (allowing for whitespace differences)
        REQUIRE(read_back.find("This is repetitive test data") != std::string::npos);
    }
}

TEST_CASE("GZStream Integration with FileManager", "[io][gzstream][integration]") {

    SECTION("Temporary file creation with FileManager") {
        gsFileManager fm;
        std::string temp_path = fm.getTempPath();

        // FileManager and gzstream should work together
        REQUIRE_FALSE(temp_path.empty());

        // Create compressed file in temp directory
        std::string gz_file = temp_path + "/integration_test.gz";

        ogzstream out(gz_file.c_str());
        REQUIRE(out.rdbuf()->is_open());

        out << "Integration test between FileManager and gzstreamer";
        out.close();

        // Verify file exists and can be read
        igzstream in(gz_file.c_str());
        REQUIRE(in.rdbuf()->is_open());

        std::string content;
        std::getline(in, content);
        REQUIRE(content == "Integration test between FileManager and gzstreamer");

        // Clean up
        std::remove(gz_file.c_str());
    }
}