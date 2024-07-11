cmake_minimum_required(VERSION 3.19)

project(matsutaku-library CXX)

# set(CMAKE_CXX_STANDARD 14) # Support C++ version of TopCoder at 11/27/2022
set(CMAKE_CXX_STANDARD 17) # Same as competitive-verifier compile option
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

add_library(mtl INTERFACE)
target_include_directories(mtl INTERFACE include)

include_directories(.)

enable_testing()
add_subdirectory(test)
