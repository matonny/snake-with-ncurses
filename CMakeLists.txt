cmake_minimum_required(VERSION 3.16)
project(snake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-lncurses")

add_executable(snake main.cpp)