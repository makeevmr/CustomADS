create_executable_names_from_dirs(ALGO_DIR_NAMES ALGO_EXECUTABLE_NAMES)

foreach(exec_name dir_name IN ZIP_LISTS ALGO_EXECUTABLE_NAMES ALGO_DIR_NAMES)
  add_executable(${exec_name} ${dir_name}/${exec_name}.cpp)
endforeach()

foreach(exec_name dir_name IN ZIP_LISTS ALGO_EXECUTABLE_NAMES ALGO_DIR_NAMES)
  target_link_libraries(${exec_name} PRIVATE GTest::GTest ${dir_name}_objs)
  add_test(g${exec_name} ${exec_name})
endforeach()
