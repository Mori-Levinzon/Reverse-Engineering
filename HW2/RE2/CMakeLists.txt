cmake_minimum_required(VERSION 3.7)
project(RE2)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES goblin_safe_solver.c)
add_executable(RE2 ${SOURCE_FILES})