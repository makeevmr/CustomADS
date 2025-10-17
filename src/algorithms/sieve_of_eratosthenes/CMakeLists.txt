set(OBJ_LIB_NAME sieve_of_eratosthenes)

add_library(${OBJ_LIB_NAME}_objs OBJECT sieve_of_eratosthenes.cpp
                                        sieve_of_eratosthenes.hpp)

set_lib_build_flags(${OBJ_LIB_NAME}_objs)
