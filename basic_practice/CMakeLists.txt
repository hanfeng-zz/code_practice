cmake_minimum_required(VERSION 3.25)
project(basic_practice)

set(CMAKE_CXX_STANDARD 17)

add_executable(basic_practice main.cpp)


add_executable(const_pointer  constPointer.h constPointer.cpp)

add_executable(INLINE  INLINE.h INLINE.cpp)

add_executable(overloadFun overloadFun.cpp)

add_executable(shared_ptr shared_ptr.cpp)

add_executable(friend friend.cpp)
add_executable(vector vector.cpp)

add_executable(dispatch dispatch.cpp)