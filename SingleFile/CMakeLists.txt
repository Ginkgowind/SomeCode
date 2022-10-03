cmake_minimum_required(VERSION 3.22)
project(Sortalgorithm)

set(CMAKE_CXX_STANDARD 11)

#add_executable(heapsort heapsort.cpp quicksort.cpp PrintFunc/printfunc.cpp PrintFunc/printfunc.h)
add_executable(quicksort quicksort.cpp)
add_executable(heapsort heapsort.cpp)
add_executable(mergesort mergesort.cpp)
add_executable(shared_ptr shared_ptr.cpp)
add_executable(Singleton Singleton.cpp)