# Simplified Core Tests CMakeLists.txt
# Tests for the Core module components that are working

# Find Catch2
find_package(Catch2 3 REQUIRED)

# Simple topology tests
add_executable(test_topology_simple test_topology_simple.cpp)
target_link_libraries(test_topology_simple
  PRIVATE
    gismo::Common
    gismo::Math
    Catch2::Catch2WithMain
)

# Register test with ctest
add_test(NAME test_topology_simple COMMAND test_topology_simple)

# Set test properties
set_tests_properties(test_topology_simple
  PROPERTIES
    LABELS "Core"
    TIMEOUT 30
)