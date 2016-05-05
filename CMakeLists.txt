cmake_minimum_required(VERSION 2.6)
project(dot.c)

set(CMAKE_C_STANDARD 99)
include_directories(include)
add_library(dot src/dot.c)
target_link_libraries(dot tcc)

add_executable(example_composed example/composed.c)
target_link_libraries(example_composed dot)
