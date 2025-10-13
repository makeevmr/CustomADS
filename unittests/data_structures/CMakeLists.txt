list(APPEND DIR_NAMES aho_corasick_automata segment_tree)
list(APPEND EXECUTABLE_NAMES test_aho_corasick_automata test_segment_tree)

foreach(exec_name dir_name IN ZIP_LISTS EXECUTABLE_NAMES DIR_NAMES)
  add_executable(${exec_name} ${dir_name}/${exec_name}.cpp)
endforeach()

if(CMAKE_BUILD_TYPE STREQUAL Release)
  message(STATUS "RELEASE BUILD TYPE")
  foreach(exec_name IN LISTS EXECUTABLE_NAMES)
    target_compile_options(${exec_name}
                           PUBLIC ${GCC_RELEASE_BUILD_TYPE_COMPILE_FLAGS})
  endforeach()
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  message(STATUS "DEBUG BUILD TYPE")
  foreach(exec_name IN LISTS EXECUTABLE_NAMES)
    target_compile_options(${exec_name}
                           PUBLIC ${GCC_DEBUG_BUILD_TYPE_COMPILE_FLAGS})
  endforeach()
endif()

foreach(exec_name IN LISTS EXECUTABLE_NAMES)
  target_link_libraries(${exec_name} PRIVATE GTest::GTest)
  add_test(g${exec_name} ${exec_name})
endforeach()
