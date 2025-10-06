# Math Module Test CMakeLists.txt
# Tests for the Math module - linear algebra and mathematical utilities

# Create test executable (no need to declare project, we're part of main gismo project)
add_executable(Math_test Math_test.cpp)

# Link against Math module
target_link_libraries(Math_test
  PRIVATE
    gismo::Common
    gismo::Math
)

# Set C++ standard
set_target_properties(Math_test PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

# Register with ctest
add_test(
  NAME Math_test
  COMMAND Math_test
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Set test properties
set_tests_properties(Math_test
  PROPERTIES
    LABELS "Math"
    TIMEOUT 60
)