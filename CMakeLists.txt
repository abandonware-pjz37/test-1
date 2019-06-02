cmake_minimum_required(VERSION 3.2)
project(foo)

set(CMAKE_CXX_STANDARD 11 CACHE STRING "...")

add_executable(foo foo.cpp)

enable_testing()
add_test(NAME foo COMMAND foo)
