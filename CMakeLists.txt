cmake_minimum_required(VERSION 3.22)
project(algolab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(algolab2 main.cpp point.h rectangle.h exhaustive_search.h rectangle_map.h persistent_segment_tree.h)

add_compile_options(-fsanitize=undefined)
add_link_options(-fsanitize=undefined)