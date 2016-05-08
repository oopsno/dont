cmake_minimum_required(VERSION 2.6)
project(tt)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_library(tt SHARED tt.cpp tables.cpp)
set_target_properties(tt PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(tt PRIVATE ${CMAKE_JS_INC})
target_link_libraries(tt ${CMAKE_JS_LIB})