cmake_minimum_required(VERSION 3.22)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 11)

add_executable(ThreadPool example.cpp ThreadPool.h)