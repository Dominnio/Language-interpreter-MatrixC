cmake_minimum_required(VERSION 3.10)
project(Language-interpreter-MatrixC)

set(CMAKE_CXX_STANDARD 11)

add_executable(Language-interpreter-MatrixC main.cpp src.cpp src.h scanner.cpp scanner.h)