# Math Module Test CMakeLists.txt
# Tests for the Math module - linear algebra and mathematical utilities

# AsMatrix Test
add_executable(test_asmatrix test_asmatrix.cpp)
target_link_libraries(test_asmatrix
  PRIVATE
    gismo::Common
    gismo::Math
)
set_target_properties(test_asmatrix PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
add_test(
  NAME test_asmatrix
  COMMAND test_asmatrix
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(test_asmatrix
  PROPERTIES
    LABELS "Math;Comprehensive"
    TIMEOUT 60
)

# Task 2.3.1: Comprehensive Compilation Testing
# ==============================================================================
# Add comprehensive test suite for validating Type Alias Strategy implementation

# Comprehensive Matrix Test
add_executable(test_matrix test_matrix.cpp)
target_link_libraries(test_matrix
  PRIVATE
    gismo::Common
    gismo::Math
)
set_target_properties(test_matrix PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
add_test(
  NAME test_matrix
  COMMAND test_matrix
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(test_matrix
  PROPERTIES
    LABELS "Math;Comprehensive;Methods"
    TIMEOUT 60
)

# Vector Comprehensive Test
add_executable(test_vector test_vector.cpp)
target_link_libraries(test_vector
  PRIVATE
    gismo::Common
    gismo::Math
)
set_target_properties(test_vector PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
add_test(
  NAME test_vector
  COMMAND test_vector
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(test_vector
  PROPERTIES
    LABELS "Math;Comprehensive;Methods"
    TIMEOUT 60
)

# Comprehensive Sparse Matrix Test
add_executable(test_sparsematrix test_sparsematrix.cpp)
target_link_libraries(test_sparsematrix
  PRIVATE
    gismo::Common
    gismo::Math
)
set_target_properties(test_sparsematrix PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
add_test(
  NAME test_sparsematrix
  COMMAND test_sparsematrix
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(test_sparsematrix
  PROPERTIES
    LABELS "Math;Comprehensive;Methods"
    TIMEOUT 60
)

# Sparse Vector Test
add_executable(test_sparsevector test_sparsevector.cpp)
target_link_libraries(test_sparsevector
  PRIVATE
    gismo::Common
    gismo::Math
)
set_target_properties(test_sparsevector PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
add_test(
  NAME test_sparsevector
  COMMAND test_sparsevector
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(test_sparsevector
  PROPERTIES
    LABELS "Math;Comprehensive;Methods"
    TIMEOUT 60
)

# Fiber Matrix Test
add_executable(test_fibermatrix test_fibermatrix.cpp)
target_link_libraries(test_fibermatrix
  PRIVATE
    gismo::Common
    gismo::Math
)
set_target_properties(test_fibermatrix PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
add_test(
  NAME test_fibermatrix
  COMMAND test_fibermatrix
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(test_fibermatrix
  PROPERTIES
    LABELS "Math;Comprehensive;Methods"
    TIMEOUT 60
)

# VecAsSymmMatrix Test
add_executable(test_vecassymmmatrix test_vecassymmmatrix.cpp)
target_link_libraries(test_vecassymmmatrix
  PRIVATE
    gismo::Common
    gismo::Math
)
set_target_properties(test_vecassymmmatrix PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
add_test(
  NAME test_vecassymmmatrix
  COMMAND test_vecassymmmatrix
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(test_vecassymmmatrix
  PROPERTIES
    LABELS "Math;Comprehensive;Methods"
    TIMEOUT 60
)

# RowSelection Test
add_executable(test_rowselection test_rowselection.cpp)
target_link_libraries(test_rowselection
  PRIVATE
    gismo::Common
    gismo::Math
)
set_target_properties(test_rowselection PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
add_test(
  NAME test_rowselection
  COMMAND test_rowselection
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
set_tests_properties(test_rowselection
  PROPERTIES
    LABELS "Math;Comprehensive;Methods"
    TIMEOUT 60
)

# Comprehensive Test Suite Target
# Create custom target to run all comprehensive tests together
add_custom_target(run_comprehensive_tests
  COMMAND ${CMAKE_CTEST_COMMAND} -L "Comprehensive" --output-on-failure
  DEPENDS
    test_matrix
    test_vector
    test_sparsematrix
    test_sparsevector
    test_fibermatrix
    test_vecassymmmatrix
    test_rowselection
  COMMENT "Running Task 2.3.1 Comprehensive Compilation Test Suite"
)

# Add to main test suite
add_custom_target(run_all_math_tests
  COMMAND ${CMAKE_CTEST_COMMAND} -L "Math" --output-on-failure
  DEPENDS
    test_matrix
    test_vector
    test_sparsematrix
    test_sparsevector
    test_fibermatrix
    test_vecassymmmatrix
    test_rowselection
    test_asmatrix
  COMMENT "Running all Math module tests including comprehensive suite"
)