file(GLOB_RECURSE
  test_files *test.cpp
)
foreach(test_file ${test_files})
  get_Filename_component(base_name ${test_file} NAME_WE)
  add_executable(${base_name} ${test_file})
endforeach()

add_custom_target(verify)
file(GLOB_RECURSE
  verify_files *.test.cpp
)
foreach(verify_file ${verify_files})
  get_Filename_component(base_name ${verify_file} NAME_WE)
  add_dependencies(verify ${base_name})
endforeach()

add_custom_target(build_ctest)
file(GLOB_RECURSE
  ctest_files *_test.cpp
)
foreach(ctest_file ${ctest_files})
  get_Filename_component(base_name ${ctest_file} NAME_WE)
  add_test(${base_name} ${base_name})
  add_dependencies(build_ctest ${base_name})
endforeach()
