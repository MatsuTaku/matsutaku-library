cmake_minimum_required(VERSION 3.19)

project(cpl)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

include_directories(cpl)

file(GLOB test_files test/*test.cpp test/*/*test.cpp)
foreach(test_file ${test_files})
  get_Filename_component(base_name ${test_file} NAME_WE)
  add_executable(${base_name} ${test_file})
endforeach()

enable_testing()
file(GLOB ctest_files test/*_test.cpp test/*/*_test.cpp)
foreach(ctest_file ${ctest_files})
  get_Filename_component(base_name ${ctest_file} NAME_WE)
  add_test(${base_name} ${base_name})
endforeach()
