create_executable_names_from_dirs(DS_DIR_NAMES DS_EXECUTABLE_NAMES)

foreach(exec_name dir_name IN ZIP_LISTS DS_EXECUTABLE_NAMES DS_DIR_NAMES)
  add_executable(${exec_name} ${dir_name}/${exec_name}.cpp)
endforeach()

if(CMAKE_BUILD_TYPE STREQUAL Release)
  message(STATUS "RELEASE BUILD TYPE")
  foreach(exec_name IN LISTS DS_EXECUTABLE_NAMES)
    target_compile_options(${exec_name}
                           PUBLIC ${GCC_RELEASE_BUILD_TYPE_COMPILE_FLAGS})
  endforeach()
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  message(STATUS "DEBUG BUILD TYPE")
  foreach(exec_name IN LISTS DS_EXECUTABLE_NAMES)
    target_compile_options(${exec_name}
                           PUBLIC ${GCC_DEBUG_BUILD_TYPE_COMPILE_FLAGS})
  endforeach()
endif()

foreach(exec_name IN LISTS DS_EXECUTABLE_NAMES)
  target_link_libraries(${exec_name} PRIVATE GTest::GTest)
  add_test(g${exec_name} ${exec_name})
endforeach()
