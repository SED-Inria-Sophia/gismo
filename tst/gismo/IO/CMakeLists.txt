# IO Module Test CMakeLists.txt
# Tests for the IO module - Input/Output functionality

# Find Catch2
find_package(Catch2 3 REQUIRED)

# Helper macro for creating IO tests
macro(add_io_test test_name)
  add_executable(${test_name} ${test_name}.cpp)
  target_include_directories(${test_name}
    PRIVATE
      ${CMAKE_SOURCE_DIR}/src
  )
  target_link_libraries(${test_name}
    PRIVATE
      gismo::IO
      Catch2::Catch2WithMain
  )
  add_test(NAME ${test_name} COMMAND ${test_name})
  set_tests_properties(${test_name}
    PROPERTIES
      LABELS "IO"
      TIMEOUT 30
  )
endmacro()

# Focused component tests - organized by functionality
add_io_test(test_optionlist)
set_tests_properties(test_optionlist PROPERTIES LABELS "IO;OptionList")

add_io_test(test_filemanager)
set_tests_properties(test_filemanager PROPERTIES LABELS "IO;FileManager")

add_io_test(test_commandline)
set_tests_properties(test_commandline PROPERTIES LABELS "IO;CommandLine")

add_io_test(test_csv)
set_tests_properties(test_csv PROPERTIES LABELS "IO;CSV")

# Specialized functionality tests (maintained)
add_io_test(test_gzstreamer)
set_tests_properties(test_gzstreamer PROPERTIES LABELS "IO;GZStreamer;Compression")