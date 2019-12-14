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

add_executable(Graph_Implementations
        Module3/Graph/test.cpp
        Module3/Graph/IGraph.h
        Module3/Graph/ArcGraph.cpp
        Module3/Graph/ArcGraph.h
        Module3/Graph/ListGraph.cpp
        Module3/Graph/ListGraph.h
        Module3/Graph/MatrixGraph.cpp
        Module3/Graph/MatrixGraph.h
        Module3/Graph/SetGraph.cpp
        Module3/Graph/SetGraph.h)

add_executable(ShortestPaths
        Module3/ShortestPaths.cpp)

add_executable(Cities
        Module3/Cities.cpp)

add_executable(SpanningTree
        Module3/SpanningTree.cpp)

add_executable(Komm
        Module3/Komm.cpp)

add_executable(Graph_Seminar
        Seminars/graphs.cpp)


#add_executable(ContestHuffman
#        Module2/Huffman/Contest/files_test.cpp
##        Module2/Huffman/Contest/ContestHuffman.cpp
#        Module2/Huffman/Contest/Huffman.h)