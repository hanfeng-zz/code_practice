
cmake_minimum_required(VERSION 3.25)
project(linux_practice)

set(CMAKE_CXX_STANDARD 20)

add_executable(byteOrder byteOrder.cpp)
add_executable(union union.cpp)

add_executable(raw raw.cpp raw.h)