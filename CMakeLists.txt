cmake_minimum_required(VERSION 3.20)
project(custom_ads LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR src)
set(UNITTESTS_DIR unittests)

set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_DEBUG "")

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

include_directories(${SOURCE_DIR})
add_subdirectory(${SOURCE_DIR})

# tests
enable_testing()
find_package(GTest REQUIRED)
add_subdirectory(${UNITTESTS_DIR})
