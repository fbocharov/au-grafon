cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -Wall -Werror -pedantic")

add_subdirectory(fractal)
add_subdirectory(glpp)
