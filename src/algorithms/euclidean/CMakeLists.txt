set(OBJ_LIB_NAME euclidean)

add_library(${OBJ_LIB_NAME}_objs OBJECT euclidean.cpp euclidean.hpp)

set_lib_build_flags(${OBJ_LIB_NAME}_objs)
