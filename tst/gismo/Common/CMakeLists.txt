# Common Module Test CMakeLists.txt
# Tests for the Common module - foundation layer

# Create test executable (no need to declare project, we're part of main gismo project)
add_executable(Common_test Common_test.cpp)

# Link against Common module only
target_link_libraries(Common_test
  PRIVATE
    gismo::Common
)

# Register with ctest
add_test(
  NAME Common_test
  COMMAND Common_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Set test properties
set_tests_properties(Common_test
  PROPERTIES
    LABELS "Common"
    TIMEOUT 30
)