# Math Module Test # # Set C++ standard
# set_target_properties(${PROJECT_NAME} PROPERTIES
#   CXX_STANDARD 17
#   CXX_STANDARD_REQUIRED ON
# )

# # Register with ctest
# add_test(
#   NAME ${PROJECT_NAME}
#   COMMAND ${PROJECT_NAME}
#   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )xt
# Tests for the Math module - linear algebra and mathematical utilities

# Temporarily disabled until Math module is implemented
# project(Math_test
# VERSION
#   ${${CMAKE_PROJECT_NAME}_VERSION}
# LANGUAGES
#   CXX)

# # Create test executable
# add_executable(${PROJECT_NAME} Math_test.cpp)

# # Link against Math module only
# target_link_libraries(${PROJECT_NAME}
#   PRIVATE
#     gismo::Math
# )

# # Set C++ standard
# set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

# Register with ctest
add_test(
  NAME ${PROJECT_NAME}
  COMMAND ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)