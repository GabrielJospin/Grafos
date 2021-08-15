cmake_minimum_required(VERSION 3.17)
project(Grafos)

set(CMAKE_CXX_STANDARD 14)
set(GRAPHVIZ_INCLUDE_DIR "C:\\Program Files\\Graphviz\\include\\graphviz")

include_directories("${GRAPHVIZ_INCLUDE_DIR}")
add_executable(Grafos grafos.cpp)