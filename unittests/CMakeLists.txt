set(DS_DIR "data_structures/")

add_executable(test_segment_tree ${DS_DIR}/segment_tree/test_segment_tree.cpp)

list(APPEND GCC_RELEASE_BUILD_TYPE_COMPILE_FLAGS -DNDEBUG -O2)

list(
  APPEND
  GCC_DEBUG_BUILD_TYPE_COMPILE_FLAGS
  -g
  -Werror
  -Wall
  -Wextra
  -Wpedantic
  -Wcast-align
  -Wcast-qual
  -Wconversion
  -Wctor-dtor-privacy
  -Wenum-compare
  -Wfloat-equal
  -Wnon-virtual-dtor
  -Wold-style-cast
  -Woverloaded-virtual
  -Wredundant-decls
  -Wsign-conversion
  -Wsign-promo)

if(CMAKE_BUILD_TYPE STREQUAL Release)
  message(STATUS "RELEASE BUILD TYPE")
  target_compile_options(test_segment_tree
                         PUBLIC ${GCC_RELEASE_BUILD_TYPE_COMPILE_FLAGS})
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  message(STATUS "DEBUG BUILD TYPE")
  target_compile_options(test_segment_tree
                         PUBLIC ${GCC_DEBUG_BUILD_TYPE_COMPILE_FLAGS})
endif()

target_link_libraries(test_segment_tree PRIVATE GTest::GTest)
add_test(gtests_segment_tree test_segment_tree)
