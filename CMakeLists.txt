cmake_minimum_required(VERSION 3.29)
project(FEM_Project)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(FEM_Project ${SOURCES})
