cmake_minimum_required(VERSION 3.17)
project(OURSTLV2)

set(CMAKE_CXX_STANDARD 20)

add_executable(OURSTLV2 main.cpp Containers/Sequence_Containers/Array.h Containers/Sequence_Containers/Vector.h Containers/Sequence_Containers/Deque.h Containers/Sequence_Containers/List.h Containers/Sequence_Containers/Forward_List.h OurSTL.h)