cmake_minimum_required(VERSION 3.31)
project(Grep C)

set(CMAKE_C_STANDARD 99)

include_directories(Lib)

# Main executable
add_executable(Grep main.c Lib/Library.c)

# Test executable
add_executable(test_Library tests/test_Library.c Lib/Library.c)

enable_testing()
add_test(NAME LibraryTest COMMAND test_Library)