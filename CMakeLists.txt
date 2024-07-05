cmake_minimum_required(VERSION 3.28)

project(StatCollector)

set(CMAKE_CXX_STANDARD 17)

add_executable(StatCollectorTest tests.cpp)

add_executable(StatCollector main.cpp
        ../stats.hpp)