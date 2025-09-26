set(OBJ_LIB_NAME sieve_of_eratosthenes)

add_library(${OBJ_LIB_NAME}_objs OBJECT sieve_of_eratosthenes.cpp
                                        sieve_of_eratosthenes.hpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
  message(STATUS "RELEASE BUILD TYPE")
  target_compile_options(${OBJ_LIB_NAME}_objs
                         PUBLIC ${GCC_RELEASE_BUILD_TYPE_COMPILE_FLAGS})
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  message(STATUS "DEBUG BUILD TYPE")
  target_compile_options(${OBJ_LIB_NAME}_objs
                         PUBLIC ${GCC_DEBUG_BUILD_TYPE_COMPILE_FLAGS})
endif()
