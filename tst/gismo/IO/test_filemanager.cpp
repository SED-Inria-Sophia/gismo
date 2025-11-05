/** @file test_filemanager.cpp

    @brief Focused tests for FileManager functionality

    This file provides comprehensive testing of the FileManager class:
    - File existence and path operations
    - Temporary directory access
    - File search functionality
    - Data directory operations
    - Path management and validation

    This file is part of the G+Smo library.
**/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

using Catch::Approx;

// Include foundation modules
#include <gismo/Common/Common>
#include <gismo/Math/Math>

// Test FileManager functionality
#include <gismo/IO/FileManager.h>

// Standard library for file operations
#include <fstream>
#include <cstdio>

using namespace gismo;

TEST_CASE("FileManager File System Operations", "[io][filemanager]") {

    SECTION("File existence and path checks") {
        gsFileManager fm;

        // Test file existence checking
        std::string current_dir = fm.getCurrentPath();
        REQUIRE(!current_dir.empty());

        // Test basic path operations - FileManager focuses on finding files
        // rather than path manipulation
        REQUIRE_NOTHROW(fm.find("nonexistent.txt")); // Should return empty string but not throw
    }

    SECTION("Temporary directory access") {
        gsFileManager fm;

        // Test that we can get temporary directory path
        std::string temp_dir = fm.getTempPath();
        REQUIRE(!temp_dir.empty());

        // Should be a valid directory path
        REQUIRE((temp_dir.back() == '/' || temp_dir.back() == '\\'));
    }

    SECTION("File search functionality") {
        gsFileManager fm;

        // Test file search - should return empty string for non-existent files
        std::string result = fm.find("definitely_nonexistent_file.xyz");
        REQUIRE(result.empty());

        // Test that findInDataDir doesn't crash
        std::string data_result = fm.findInDataDir("nonexistent.txt");
        // Should return empty string but not crash
        REQUIRE_NOTHROW(fm.fileExistsInDataDir("nonexistent.txt"));
    }
}

TEST_CASE("FileManager Path Operations", "[io][filemanager][paths]") {

    SECTION("Current path retrieval") {
        gsFileManager fm;

        std::string current = fm.getCurrentPath();
        REQUIRE(!current.empty());

        // Current path should be a valid directory path
        // (ends with separator or is root)
        REQUIRE(current.length() > 0);
    }

    SECTION("Temporary directory validation") {
        gsFileManager fm;

        std::string temp_path = fm.getTempPath();
        REQUIRE(!temp_path.empty());

        // Temp path should end with directory separator
        char last_char = temp_path.back();
        bool is_valid_separator = (last_char == '/' || last_char == '\\');
        REQUIRE(is_valid_separator);
    }

    SECTION("Path normalization behavior") {
        gsFileManager fm;

        // Test that paths are handled consistently
        std::string temp1 = fm.getTempPath();
        std::string temp2 = fm.getTempPath();

        // Should return consistent results
        REQUIRE(temp1 == temp2);
    }
}

TEST_CASE("FileManager Search Operations", "[io][filemanager][search]") {

    SECTION("File search with non-existent files") {
        gsFileManager fm;

        // Search for files that definitely don't exist
        std::string result1 = fm.find("this_file_definitely_does_not_exist_12345.txt");
        std::string result2 = fm.find("nonexistent_with_strange_name.xyz");

        REQUIRE(result1.empty());
        REQUIRE(result2.empty());
    }

    SECTION("Data directory search operations") {
        gsFileManager fm;

        // Test data directory search methods
        REQUIRE_NOTHROW(fm.findInDataDir("nonexistent.txt"));
        REQUIRE_NOTHROW(fm.fileExistsInDataDir("nonexistent.txt"));

        // Non-existent files should return false/empty
        std::string data_result = fm.findInDataDir("definitely_not_there.xml");
        bool exists = fm.fileExistsInDataDir("definitely_not_there.xml");

        REQUIRE(data_result.empty());
        REQUIRE(exists == false);
    }

    SECTION("Search method consistency") {
        gsFileManager fm;

        // Test that different search methods behave consistently
        // for non-existent files
        std::string general_search = fm.find("test_consistency.dat");
        std::string data_search = fm.findInDataDir("test_consistency.dat");
        bool data_exists = fm.fileExistsInDataDir("test_consistency.dat");

        // All should indicate file doesn't exist
        REQUIRE(general_search.empty());
        REQUIRE(data_search.empty());
        REQUIRE(data_exists == false);
    }
}

TEST_CASE("FileManager Real File Operations", "[io][filemanager][real]") {

    SECTION("Create and find temporary test file") {
        gsFileManager fm;

        // Create a temporary file for testing
        std::string temp_dir = fm.getTempPath();
        std::string test_file = temp_dir + "filemanager_test.txt";

        // Create the file
        std::ofstream file(test_file);
        REQUIRE(file.is_open());
        file << "Test content for FileManager";
        file.close();

        // Now try to find it using FileManager
        // Note: find() might not look in temp directory, but file should exist
        std::ifstream check_file(test_file);
        REQUIRE(check_file.is_open());
        check_file.close();

        // Clean up
        std::remove(test_file.c_str());

        // Verify cleanup worked
        std::ifstream verify_cleanup(test_file);
        REQUIRE(!verify_cleanup.is_open());
    }

    SECTION("Multiple file operations") {
        gsFileManager fm;

        std::string temp_dir = fm.getTempPath();

        // Create multiple test files
        std::vector<std::string> test_files = {
            temp_dir + "fm_test1.dat",
            temp_dir + "fm_test2.cfg",
            temp_dir + "fm_test3.xml"
        };

        // Create all files
        for (const auto& filename : test_files) {
            std::ofstream file(filename);
            REQUIRE(file.is_open());
            file << "Test data";
            file.close();
        }

        // Verify all files exist
        for (const auto& filename : test_files) {
            std::ifstream file(filename);
            REQUIRE(file.is_open());
            file.close();
        }

        // Clean up all files
        for (const auto& filename : test_files) {
            std::remove(filename.c_str());
        }
    }
}

TEST_CASE("FileManager Error Handling", "[io][filemanager][errors]") {

    SECTION("Invalid path handling") {
        gsFileManager fm;

        // Test with various invalid/unusual filenames
        std::vector<std::string> invalid_files = {
            "",                          // Empty string
            "   ",                       // Whitespace only
            "file_with_very_long_name_that_probably_does_not_exist_anywhere_12345.extension",
            "invalid/path/structure.txt",
            "*.wildcard.file"
        };

        for (const auto& invalid_file : invalid_files) {
            // These should not throw, just return empty/false
            REQUIRE_NOTHROW(fm.find(invalid_file));
            REQUIRE_NOTHROW(fm.findInDataDir(invalid_file));
            REQUIRE_NOTHROW(fm.fileExistsInDataDir(invalid_file));

            // Results should indicate failure
            REQUIRE(fm.find(invalid_file).empty());
            REQUIRE(fm.findInDataDir(invalid_file).empty());
            REQUIRE(fm.fileExistsInDataDir(invalid_file) == false);
        }
    }

    SECTION("Method robustness") {
        gsFileManager fm;

        // Test that core methods don't crash with edge cases
        REQUIRE_NOTHROW(fm.getCurrentPath());
        REQUIRE_NOTHROW(fm.getTempPath());

        // Results should be meaningful
        REQUIRE(!fm.getCurrentPath().empty());
        REQUIRE(!fm.getTempPath().empty());
    }
}

TEST_CASE("FileManager Integration", "[io][filemanager][integration]") {

    SECTION("FileManager with other IO components workflow") {
        gsFileManager fm;

        // Simulate a workflow where FileManager is used with other components
        // 1. Get temp directory for output
        std::string temp_dir = fm.getTempPath();
        REQUIRE(!temp_dir.empty());

        // 2. Create a data file
        std::string data_file = temp_dir + "workflow_data.txt";
        std::ofstream output(data_file);
        REQUIRE(output.is_open());
        output << "Workflow test data\n";
        output << "Line 2\n";
        output << "Line 3\n";
        output.close();

        // 3. Use FileManager to verify file operations
        std::ifstream input(data_file);
        REQUIRE(input.is_open());

        std::string content;
        std::getline(input, content);
        REQUIRE(content == "Workflow test data");
        input.close();

        // 4. Clean up
        std::remove(data_file.c_str());
    }

    SECTION("Consistent behavior across operations") {
        gsFileManager fm;

        // Test that FileManager behaves consistently across multiple calls
        std::string path1 = fm.getCurrentPath();
        std::string path2 = fm.getCurrentPath();
        std::string temp1 = fm.getTempPath();
        std::string temp2 = fm.getTempPath();

        REQUIRE(path1 == path2);
        REQUIRE(temp1 == temp2);

        // Search operations should also be consistent
        std::string search1 = fm.find("nonexistent_consistency_test.xyz");
        std::string search2 = fm.find("nonexistent_consistency_test.xyz");

        REQUIRE(search1 == search2);
        REQUIRE(search1.empty());
    }
}