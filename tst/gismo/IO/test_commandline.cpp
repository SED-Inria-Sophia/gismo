/** @file test_commandline.cpp

    @brief Focused tests for CommandLine functionality

    This file provides comprehensive testing of the CommandLine class:
    - Command line setup and basic functionality
    - OptionList integration (inheritance)
    - Argument parsing capabilities
    - Configuration-driven workflows
    - Command line option management

    This file is part of the G+Smo library.
**/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

using Catch::Approx;

// Include foundation modules
#include <gismo/Common/Common>
#include <gismo/Math/Math>

// Test CommandLine functionality
#include <gismo/IO/CommandLine.h>

using namespace gismo;

TEST_CASE("CommandLine Argument Processing", "[io][commandline]") {

    SECTION("Command line setup and basic functionality") {
        // Create command line processor
        gsCmdLine cmd("Test Program");

        // Add various argument types using the correct API
        // Use addNewXXX methods which don't require lvalue references
        cmd.addNewInt("n", "number", "Number parameter", 10);

        // Test that we can retrieve options (should have defaults)
        // Note: CommandLine inherits from OptionList, so we can access options directly
        REQUIRE(cmd.askInt("number", 999) == 10);
    }

    SECTION("Option list integration") {
        gsCmdLine cmd("Test");

        // Add parameters using CommandLine API with reference variables
        index_t iter_val = 100;
        real_t eps_val = 1e-6;
        cmd.addInt("i", "iterations", "Max iterations", iter_val);
        cmd.addReal("e", "eps", "Convergence criterion", eps_val);

        // Values should be accessible through OptionList interface
        REQUIRE(cmd.askInt("iterations", 999) == 100);
        REQUIRE(cmd.askReal("eps", 999.0) == Approx(1e-6));
    }
}

TEST_CASE("CommandLine OptionList Inheritance", "[io][commandline][inheritance]") {

    SECTION("Basic OptionList functionality through CommandLine") {
        gsCmdLine cmd("Test Program");

        // Test CommandLine API with proper flag, name, description, and reference
        index_t degree_val = 3;
        real_t tol_val = 1e-6;
        std::string geom_val = "square.xml";
        bool plot_val = false;
        cmd.addInt("d", "maxDegree", "Maximum polynomial degree", degree_val);
        cmd.addReal("t", "tolerance", "Solver tolerance", tol_val);
        cmd.addString("g", "geometry", "Geometry file", geom_val);
        cmd.addSwitch("p", "plot", "Generate plots", plot_val);

        // Verify retrieval through ask methods (safer for CommandLine)
        REQUIRE(cmd.askInt("maxDegree", 999) == 3);
        REQUIRE(cmd.askReal("tolerance", 999.0) == Approx(1e-6));
        REQUIRE(cmd.askString("geometry", "default") == "square.xml");
        REQUIRE(cmd.askSwitch("plot", true) == false);

        // Test that reference variables work
        REQUIRE(degree_val == 3);
        REQUIRE(tol_val == Approx(1e-6));
        REQUIRE(geom_val == "square.xml");
        REQUIRE(plot_val == false);
    }

    SECTION("Ask methods work through inheritance") {
        gsCmdLine cmd("Test");

        real_t param_val = 2.5;
        cmd.addReal("p", "configured_param", "A configured parameter", param_val);

        // Should return configured value
        REQUIRE(cmd.askReal("configured_param", 999.0) == Approx(2.5));

        // Should return default for missing parameter
        REQUIRE(cmd.askReal("missing_param", 123.456) == Approx(123.456));
        REQUIRE(cmd.askInt("missing_int", 42) == 42);
        REQUIRE(cmd.askString("missing_string", "default_value") == "default_value");
        REQUIRE(cmd.askSwitch("missing_switch", true) == true);
    }

    SECTION("Type safety through CommandLine") {
        gsCmdLine cmd("Type Safety Test");

        index_t num_val = 100;
        std::string str_val = "hello";
        cmd.addInt("n", "number_param", "Integer parameter", num_val);
        cmd.addString("s", "string_param", "String parameter", str_val);

        // Correct type access through ask methods should work
        REQUIRE(cmd.askInt("number_param", 999) == 100);
        REQUIRE(cmd.askString("string_param", "default") == "hello");

        // Reference variables should have correct values
        REQUIRE(num_val == 100);
        REQUIRE(str_val == "hello");
    }
}

TEST_CASE("CommandLine Advanced Features", "[io][commandline][advanced]") {

    SECTION("Command line with multiple parameter types") {
        gsCmdLine cmd("Advanced Test");

        // Add parameters of all types
        index_t degree_val = 2;
        real_t h_val = 0.1;
        std::string input_val = "input.xml";
        bool verbose_val = false;
        cmd.addInt("d", "degree", "Polynomial degree", degree_val);
        cmd.addReal("m", "meshsize", "Mesh size", h_val);
        cmd.addString("i", "input", "Input file", input_val);
        cmd.addSwitch("v", "verbose", "Verbose output", verbose_val);

        // Add command line specific parameters using addNewXXX
        cmd.addNewInt("r", "refine", "Refinement levels", 3);

        // Test that all parameters work together through ask methods
        REQUIRE(cmd.askInt("degree", 999) == 2);
        REQUIRE(cmd.askReal("meshsize", 999.0) == Approx(0.1));
        REQUIRE(cmd.askString("input", "default") == "input.xml");
        REQUIRE(cmd.askSwitch("verbose", true) == false);
        REQUIRE(cmd.askInt("refine", 999) == 3);

        // Test that reference variables reflect the values
        REQUIRE(degree_val == 2);
        REQUIRE(h_val == Approx(0.1));
        REQUIRE(input_val == "input.xml");
        REQUIRE(verbose_val == false);
    }

    SECTION("CommandLine error handling") {
        gsCmdLine cmd("Error Test");

        index_t valid_val = 50;
        cmd.addInt("v", "valid_param", "Valid parameter", valid_val);

        // Ask methods should return configured value for existing parameters
        REQUIRE(cmd.askInt("valid_param", 999) == 50);

        // Ask methods should return defaults for missing parameters
        REQUIRE(cmd.askInt("invalid_param", 123) == 123);
        REQUIRE(cmd.askString("missing", "fallback") == "fallback");
    }
}

TEST_CASE("Real-world CommandLine Workflow", "[io][commandline][workflow]") {

    SECTION("Configuration-driven application workflow") {
        // Simulate a typical GISMO application workflow

        // 1. Setup command line with proper CommandLine API
        gsCmdLine cmd("GISMO Solver");
        index_t degree_val = 3;
        index_t refine_val = 2;
        real_t tol_val = 1e-6;
        std::string geom_val = "square.xml";
        bool plot_val = false;
        cmd.addInt("d", "degree", "Polynomial degree", degree_val);
        cmd.addInt("r", "refine", "Refinement levels", refine_val);
        cmd.addReal("t", "tolerance", "Solver tolerance", tol_val);
        cmd.addString("g", "geometry", "Geometry file", geom_val);
        cmd.addSwitch("p", "plot", "Generate plots", plot_val);

        // 2. Access configuration via reference variables (the CommandLine way)
        REQUIRE(degree_val == 3);
        REQUIRE(refine_val == 2);
        REQUIRE(tol_val == Approx(1e-6));
        REQUIRE(geom_val == "square.xml");
        REQUIRE(plot_val == false);

        // 3. Also test access via ask methods
        REQUIRE(cmd.askInt("degree", 999) == 3);
        REQUIRE(cmd.askInt("refine", 999) == 2);
        REQUIRE(cmd.askReal("tolerance", 999.0) == Approx(1e-6));
        REQUIRE(cmd.askString("geometry", "default") == "square.xml");
        REQUIRE(cmd.askSwitch("plot", true) == false);
    }

    SECTION("Multi-stage parameter configuration") {
        gsCmdLine cmd("Multi-Stage Config");

        // Stage 1: Basic setup
        index_t steps_val = 10;
        real_t dt_val = 0.01;
        cmd.addInt("s", "initialSteps", "Initial steps", steps_val);
        cmd.addReal("t", "timestep", "Time step", dt_val);

        // Stage 2: Add more parameters
        std::string method_val = "BDF2";
        bool adaptive_val = false;
        cmd.addString("M", "method", "Time integration method", method_val);
        cmd.addSwitch("a", "adaptiveTimeStep", "Use adaptive time stepping", adaptive_val);

        // Stage 3: Verify configuration via reference variables
        REQUIRE(steps_val == 10);
        REQUIRE(dt_val == Approx(0.01));
        REQUIRE(method_val == "BDF2");
        REQUIRE(adaptive_val == false);

        // Also verify via ask methods
        REQUIRE(cmd.askInt("initialSteps", 999) == 10);
        REQUIRE(cmd.askReal("timestep", 999.0) == Approx(0.01));
        REQUIRE(cmd.askString("method", "default") == "BDF2");
        REQUIRE(cmd.askSwitch("adaptiveTimeStep", true) == false);
    }
}

TEST_CASE("CommandLine Parameter Management", "[io][commandline][management]") {

    SECTION("Parameter validation and constraints") {
        gsCmdLine cmd("Validation Test");

        // Add parameters with specific ranges/constraints
        index_t maxiter_val = 1000;
        real_t reltol_val = 1e-8;
        std::string format_val = "VTK";
        cmd.addInt("i", "maxIter", "Maximum iterations", maxiter_val);
        cmd.addReal("t", "relTol", "Relative tolerance", reltol_val);
        cmd.addString("f", "outputFormat", "Output format", format_val);

        // Test parameter constraint validation via reference variables
        REQUIRE(maxiter_val > 0);  // Should be positive
        REQUIRE(reltol_val > 0.0);  // Should be positive
        REQUIRE(reltol_val < 1.0);  // Should be less than 1

        // Test parameter access via ask methods
        REQUIRE(cmd.askInt("maxIter", 999) == 1000);
        REQUIRE(cmd.askReal("relTol", 999.0) == Approx(1e-8));
        REQUIRE(cmd.askString("outputFormat", "default") == "VTK");
    }

    SECTION("Parameter hierarchy and defaults") {
        gsCmdLine cmd("Hierarchy Test");

        // Setup parameter hierarchy: global -> specific
        index_t global_refine_val = 2;
        index_t local_refine_val = 1;
        real_t global_tol_val = 1e-6;
        real_t local_tol_val = 1e-8;
        cmd.addInt("g", "globalRefine", "Global refinement", global_refine_val);
        cmd.addInt("l", "localRefine", "Local refinement", local_refine_val);
        cmd.addReal("G", "globalTolerance", "Global tolerance", global_tol_val);
        cmd.addReal("L", "localTolerance", "Local tolerance", local_tol_val);

        // Test parameter hierarchy via reference variables
        int totalRefine = global_refine_val + local_refine_val;
        real_t effectiveTolerance = std::min(global_tol_val, local_tol_val);

        REQUIRE(totalRefine == 3);
        REQUIRE(effectiveTolerance == Approx(1e-8));

        // Test parameter inheritance patterns
        REQUIRE(local_tol_val < global_tol_val);
    }

    SECTION("Dynamic parameter addition") {
        gsCmdLine cmd("Dynamic Test");

        // Start with basic parameters
        index_t basic_val = 1;
        cmd.addInt("b", "basicParam", "Basic parameter", basic_val);

        // Test parameter existence via reference variable
        REQUIRE(basic_val == 1);
        REQUIRE(cmd.askInt("basicParam", 999) == 1);

        // Test that non-existent parameters return defaults
        REQUIRE(cmd.askReal("advancedParam", 999.0) == Approx(999.0));

        // Now add an advanced parameter
        real_t advanced_val = 2.5;
        cmd.addReal("a", "advancedParam", "Advanced parameter", advanced_val);

        // Now the parameter should exist
        REQUIRE(advanced_val == Approx(2.5));
        REQUIRE(cmd.askReal("advancedParam", 999.0) == Approx(2.5));
    }
}

TEST_CASE("CommandLine Update Functionality", "[io][commandline][updates]") {

    SECTION("Reference variable updates after command line parsing") {
        gsCmdLine cmd("Update Test");

        // Setup parameters with initial values
        index_t degree_val = 2;
        real_t tolerance_val = 1e-6;
        std::string method_val = "CG";
        bool verbose_val = false;

        cmd.addInt("d", "degree", "Polynomial degree", degree_val);
        cmd.addReal("t", "tolerance", "Solver tolerance", tolerance_val);
        cmd.addString("m", "method", "Solution method", method_val);
        cmd.addSwitch("v", "verbose", "Verbose output", verbose_val);

        // Verify initial values via reference variables
        REQUIRE(degree_val == 2);
        REQUIRE(tolerance_val == Approx(1e-6));
        REQUIRE(method_val == "CG");
        REQUIRE(verbose_val == false);

        // Verify initial values via ask methods (immediate OptionList population)
        REQUIRE(cmd.askInt("degree", 999) == 2);
        REQUIRE(cmd.askReal("tolerance", 999.0) == Approx(1e-6));
        REQUIRE(cmd.askString("method", "default") == "CG");
        REQUIRE(cmd.askSwitch("verbose", true) == false);

        // Simulate command line arguments with different values
        const char* argv[] = {"program", "--degree", "4", "--tolerance", "1e-8", "--method", "GMRES", "--verbose"};
        int argc = sizeof(argv) / sizeof(char*);

        // Parse command line
        cmd.getValues(argc, const_cast<char**>(argv));

        // Verify that reference variables are updated
        REQUIRE(degree_val == 4);  // Updated from 2 to 4
        REQUIRE(tolerance_val == Approx(1e-8));  // Updated from 1e-6 to 1e-8
        REQUIRE(method_val == "GMRES");  // Updated from "CG" to "GMRES"
        REQUIRE(verbose_val == true);  // Updated from false to true (toggle)

        // Verify that ask methods return updated values
        REQUIRE(cmd.askInt("degree", 999) == 4);
        REQUIRE(cmd.askReal("tolerance", 999.0) == Approx(1e-8));
        REQUIRE(cmd.askString("method", "default") == "GMRES");
        REQUIRE(cmd.askSwitch("verbose", false) == true);
    }

    SECTION("OptionList synchronization via updateOptionList") {
        gsCmdLine cmd("Sync Test");

        // Add parameters using both CommandLine and OptionList interfaces
        index_t cmdline_param = 10;
        cmd.addInt("c", "cmdlineParam", "Command line parameter", cmdline_param);

        // Add OptionList-only parameter (not command line accessible)
        cmd.addInt("optionParam", "Option list only parameter", 20);

        // Before parsing: both should be accessible via ask methods
        REQUIRE(cmd.askInt("cmdlineParam", 999) == 10);
        REQUIRE(cmd.askInt("optionParam", 999) == 20);

        // Simulate command line parsing
        const char* argv[] = {"program", "--cmdlineParam", "15"};
        int argc = sizeof(argv) / sizeof(char*);

        cmd.getValues(argc, const_cast<char**>(argv));

        // After parsing: updateOptionList() should synchronize everything
        REQUIRE(cmdline_param == 15);  // Reference variable updated
        REQUIRE(cmd.askInt("cmdlineParam", 999) == 15);  // OptionList updated
        REQUIRE(cmd.askInt("optionParam", 999) == 20);  // OptionList-only unchanged
    }

    SECTION("Partial parameter updates with mixed command line usage") {
        gsCmdLine cmd("Mixed Update Test");

        // Setup multiple parameters
        index_t iter_val = 100;
        real_t tol_val = 1e-6;
        std::string file_val = "input.xml";
        bool debug_val = false;

        cmd.addInt("i", "iterations", "Max iterations", iter_val);
        cmd.addReal("t", "tolerance", "Convergence tolerance", tol_val);
        cmd.addString("f", "filename", "Input filename", file_val);
        cmd.addSwitch("d", "debug", "Enable debug output", debug_val);

        // Parse command line with only some parameters specified
        const char* argv[] = {"program", "--iterations", "200", "--debug"};
        int argc = sizeof(argv) / sizeof(char*);

        cmd.getValues(argc, const_cast<char**>(argv));

        // Updated parameters should reflect command line values
        REQUIRE(iter_val == 200);  // Updated
        REQUIRE(debug_val == true);  // Updated (toggled)

        // Non-specified parameters should retain default values
        REQUIRE(tol_val == Approx(1e-6));  // Unchanged
        REQUIRE(file_val == "input.xml");  // Unchanged

        // OptionList should reflect the same state
        REQUIRE(cmd.askInt("iterations", 999) == 200);
        REQUIRE(cmd.askReal("tolerance", 999.0) == Approx(1e-6));
        REQUIRE(cmd.askString("filename", "default") == "input.xml");
        REQUIRE(cmd.askSwitch("debug", false) == true);
    }

    SECTION("Switch toggle behavior with updates") {
        gsCmdLine cmd("Switch Toggle Test");

        // Test switch toggling behavior
        bool flag1_val = false;  // Should become true when specified
        bool flag2_val = true;   // Should become false when specified
        bool flag3_val = false;  // Should remain false when not specified

        cmd.addSwitch("a", "flagA", "Flag A", flag1_val);
        cmd.addSwitch("b", "flagB", "Flag B", flag2_val);
        cmd.addSwitch("c", "flagC", "Flag C", flag3_val);

        // Specify only flags A and B on command line
        const char* argv[] = {"program", "--flagA", "--flagB"};
        int argc = sizeof(argv) / sizeof(char*);

        cmd.getValues(argc, const_cast<char**>(argv));

        // Verify toggle behavior
        REQUIRE(flag1_val == true);   // false -> true (toggled)
        REQUIRE(flag2_val == false);  // true -> false (toggled)
        REQUIRE(flag3_val == false);  // false -> false (not specified, no toggle)

        // Verify OptionList synchronization
        REQUIRE(cmd.askSwitch("flagA", false) == true);
        REQUIRE(cmd.askSwitch("flagB", true) == false);
        REQUIRE(cmd.askSwitch("flagC", true) == false);
    }

    SECTION("Multi-value parameter updates") {
        gsCmdLine cmd("Multi-Value Test");

        // Test multi-value parameter updates
        std::vector<index_t> int_list = {1, 2, 3};  // Initial values
        std::vector<real_t> real_list = {0.1, 0.2};  // Initial values
        std::vector<std::string> str_list = {"a", "b"};  // Initial values

        cmd.addMultiInt("i", "intList", "Integer list", int_list);
        cmd.addMultiReal("r", "realList", "Real list", real_list);
        cmd.addMultiString("s", "strList", "String list", str_list);

        // Specify multiple values on command line
        const char* argv[] = {"program",
                             "--intList", "10", "--intList", "20", "--intList", "30",
                             "--realList", "1.5", "--realList", "2.5",
                             "--strList", "hello", "--strList", "world"};
        int argc = sizeof(argv) / sizeof(char*);

        cmd.getValues(argc, const_cast<char**>(argv));

        // Verify multi-value updates (should replace initial values)
        REQUIRE(int_list.size() == 3);
        REQUIRE(int_list[0] == 10);
        REQUIRE(int_list[1] == 20);
        REQUIRE(int_list[2] == 30);

        REQUIRE(real_list.size() == 2);
        REQUIRE(real_list[0] == Approx(1.5));
        REQUIRE(real_list[1] == Approx(2.5));

        REQUIRE(str_list.size() == 2);
        REQUIRE(str_list[0] == "hello");
        REQUIRE(str_list[1] == "world");
    }

    SECTION("Plain string parameter updates") {
        gsCmdLine cmd("Plain String Test");

        // Test plain string (unlabeled) parameter
        std::string input_file = "default.xml";
        cmd.addPlainString("inputFile", "Input file (unlabeled)", input_file);

        // Also add regular parameters
        index_t refine_val = 2;
        cmd.addInt("r", "refine", "Refinement level", refine_val);

        // Specify plain string and regular parameter
        const char* argv[] = {"program", "custom_input.xml", "--refine", "5"};
        int argc = sizeof(argv) / sizeof(char*);

        cmd.getValues(argc, const_cast<char**>(argv));

        // Verify updates
        REQUIRE(input_file == "custom_input.xml");  // Plain string updated
        REQUIRE(refine_val == 5);  // Regular parameter updated

        // Verify OptionList contains updated values
        REQUIRE(cmd.askString("inputFile", "default") == "custom_input.xml");
        REQUIRE(cmd.askInt("refine", 999) == 5);
    }

    SECTION("Pre- and post-parsing state consistency") {
        gsCmdLine cmd("State Consistency Test");

        // Setup parameters
        index_t param1 = 42;
        real_t param2 = 3.14;
        std::string param3 = "initial";

        cmd.addInt("a", "param1", "Parameter 1", param1);
        cmd.addReal("b", "param2", "Parameter 2", param2);
        cmd.addString("c", "param3", "Parameter 3", param3);

        // Before parsing: ask methods should return initial values
        REQUIRE(cmd.askInt("param1", 999) == 42);
        REQUIRE(cmd.askReal("param2", 999.0) == Approx(3.14));
        REQUIRE(cmd.askString("param3", "default") == "initial");

        // Parse with updated values
        const char* argv[] = {"program", "--param1", "100", "--param3", "updated"};
        int argc = sizeof(argv) / sizeof(char*);

        cmd.getValues(argc, const_cast<char**>(argv));

        // After parsing: both reference variables and ask methods should be consistent
        REQUIRE(param1 == 100);
        REQUIRE(param2 == Approx(3.14));  // Unchanged
        REQUIRE(param3 == "updated");

        REQUIRE(cmd.askInt("param1", 999) == 100);
        REQUIRE(cmd.askReal("param2", 999.0) == Approx(3.14));
        REQUIRE(cmd.askString("param3", "default") == "updated");
    }
}