cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "src/")
set(UNITTESTS_DIR "unittests/")

project(custom_ads LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_DEBUG "")

include_directories(${SOURCE_DIR})

# tests
enable_testing()
find_package(GTest REQUIRED)
add_subdirectory(${UNITTESTS_DIR})
