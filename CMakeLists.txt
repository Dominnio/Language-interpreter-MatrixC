cmake_minimum_required(VERSION 3.10)
project(Language-interpreter-MatrixC)

set(CMAKE_CXX_STANDARD 11)

add_executable(Language-interpreter-MatrixC main.cpp source.cpp source.h scanner.cpp scanner.h parser.cpp parser.h executor.cpp executor.h)