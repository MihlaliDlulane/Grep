cmake_minimum_required(VERSION 3.31)
project(Grep C)

set(CMAKE_C_STANDARD 99)

# Main executable
add_executable(Grep main.c Lib/Library.c)
target_include_directories(Grep PRIVATE Lib)

# Test executable
add_executable(test_Library tests/test_Library.c Lib/Library.c)
target_include_directories(test_Library PRIVATE Lib)

enable_testing()
add_test(NAME LibraryTest COMMAND test_Library)