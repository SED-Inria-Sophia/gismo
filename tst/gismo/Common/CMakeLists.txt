# Common Module Test CMakeLists.txt
# Tests for the Common module - foundation layer

project(Common_test
VERSION
  ${${CMAKE_PROJECT_NAME}_VERSION}
LANGUAGES
  CXX)

# Create test executable
add_executable(${PROJECT_NAME} Common_test.cpp)

# Link against Common module only
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    gismo::Common
)

# Register with ctest
add_test(
  NAME ${PROJECT_NAME}
  COMMAND ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Set test properties
set_tests_properties(${PROJECT_NAME}
  PROPERTIES
    LABELS "Common"
    TIMEOUT 30
)