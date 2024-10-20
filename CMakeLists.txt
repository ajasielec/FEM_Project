cmake_minimum_required(VERSION 3.29)
project(FEM_Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(FEM_Project main.cpp
        main.h
        Quadrature.cpp
        Quadrature.h
        grid.cpp
        grid.h
        Node.cpp
        Node.h
        Element.cpp
        Element.h
        ElemUniv.cpp
        ElemUniv.h
        Jakobian.cpp
        Jakobian.h
        includes.h)
