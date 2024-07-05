cmake_minimum_required(VERSION 3.28)

project(StatCollector)

set(CMAKE_CXX_STANDARD 17)

# Add the source directory
add_subdirectory(src)

# Add the tests directory
add_subdirectory(test)
