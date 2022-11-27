cmake_minimum_required(VERSION 3.19)

project(matsutaku-library CXX)

set(CMAKE_CXX_STANDARD 14) # Support C++ version of TopCoder at 11/27/2022
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

add_library(mtl INTERFACE)
target_include_directories(mtl INTERFACE include)

enable_testing()
include_directories(include/mtl)
add_subdirectory(test)
