function(set_lib_build_flags OBJ_LIB_NAME)
  if(CMAKE_BUILD_TYPE STREQUAL Release)
    message(STATUS "RELEASE BUILD TYPE")
    target_compile_options(${OBJ_LIB_NAME}
                           PUBLIC ${GCC_RELEASE_BUILD_TYPE_COMPILE_FLAGS})
  endif()

  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    message(STATUS "DEBUG BUILD TYPE")
    target_compile_options(${OBJ_LIB_NAME}
                           PUBLIC ${GCC_DEBUG_BUILD_TYPE_COMPILE_FLAGS})
  endif()
endfunction()

foreach(algo_dir_name IN LISTS ALGO_DIR_NAMES)
  add_subdirectory(${algo_dir_name})
endforeach()
