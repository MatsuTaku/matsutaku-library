cmake_minimum_required(VERSION 3.19)
project(matsutaku-library LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OF)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_compile_options(-Wall -Wextra -Wshadow)
add_compile_options($<$<CONFIG:Release>:-O2>)

add_library(mtl INTERFACE)
target_include_directories(mtl INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_subdirectory(test)
