cmake_minimum_required(VERSION 3.22)
project(algolab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(algolab2 main.cpp solvers/point.h solvers/rectangle.h solvers/exhaustive_search.h solvers/rectangle_map.h solvers/persistent_segment_tree.h benchmarks/generators.h benchmarks/time_mesuring.h)

add_compile_options(-fsanitize=undefined)
add_link_options(-fsanitize=undefined)