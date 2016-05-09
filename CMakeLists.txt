cmake_minimum_required(VERSION 2.6)
project(dont)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
include_directories(include)

add_subdirectory(codegen/type_traits)

file(GLOB dont_src "src/*.c")
add_library(dont ${dont_src})
target_link_libraries(dont tcc)

add_executable(example_composed example/composed.c)
target_link_libraries(example_composed dont)

file(GLOB test_src "test/*.cpp")
add_executable(unittest ${test_src})
target_link_libraries(unittest dont gtest)
