cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 14)
project(HW)

set(CMAKE_CXX_STANDARD 11)
add_executable(1_1
        Module2/1_1.cpp)

add_executable(2_4
        Module2/2_4.cpp)

add_executable(3_2
        Module2/3_2.cpp)

add_executable(3_2_work_in_progress
        "Module2/3_2(work in progress).cpp")

add_executable(4_2
        Module2/4_2.cpp)

add_executable(Huffman
        Module2/Huffman/files_test.cpp
        Module2/Huffman/Huffman.cpp
        Module2/Huffman/Huffman.h)

#add_executable(ContestHuffman
#        Module2/Huffman/Contest/files_test.cpp
##        Module2/Huffman/Contest/ContestHuffman.cpp
#        Module2/Huffman/Contest/Huffman.h)