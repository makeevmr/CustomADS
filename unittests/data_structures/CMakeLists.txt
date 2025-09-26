set(SEGMENT_TREE_DIR "segment_tree/")

add_executable(test_segment_tree ${SEGMENT_TREE_DIR}/test_segment_tree.cpp)

list(APPEND EXECUTABLES test_segment_tree)

if(CMAKE_BUILD_TYPE STREQUAL Release)
  message(STATUS "RELEASE BUILD TYPE")
  foreach(executable IN LISTS EXECUTABLES)
    target_compile_options(${executable}
                           PUBLIC ${GCC_RELEASE_BUILD_TYPE_COMPILE_FLAGS})
  endforeach()
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  message(STATUS "DEBUG BUILD TYPE")
  foreach(executable IN LISTS EXECUTABLES)
    target_compile_options(${executable}
                           PUBLIC ${GCC_DEBUG_BUILD_TYPE_COMPILE_FLAGS})
  endforeach()
endif()

target_link_libraries(test_segment_tree PRIVATE GTest::GTest)
add_test(gtests_segment_tree test_segment_tree)
