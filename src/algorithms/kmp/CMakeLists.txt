set(OBJ_LIB_NAME kmp)

add_library(${OBJ_LIB_NAME}_objs OBJECT kmp.cpp kmp.hpp)

set_lib_build_flags(${OBJ_LIB_NAME}_objs)
