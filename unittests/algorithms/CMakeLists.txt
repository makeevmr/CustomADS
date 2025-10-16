list(APPEND DIR_NAMES kmp sieve_of_eratosthenes)

set(EXECUTABLE_NAMES "")
foreach(dir IN LISTS DIR_NAMES)
  string(PREPEND dir "test_")
  list(APPEND EXECUTABLE_NAMES "${dir}")
endforeach()

foreach(exec_name dir_name IN ZIP_LISTS EXECUTABLE_NAMES DIR_NAMES)
  add_executable(${exec_name} ${dir_name}/${exec_name}.cpp)
endforeach()

foreach(exec_name dir_name IN ZIP_LISTS EXECUTABLE_NAMES DIR_NAMES)
  target_link_libraries(${exec_name} PRIVATE GTest::GTest ${dir_name}_objs)
  add_test(g${exec_name} ${exec_name})
endforeach()
