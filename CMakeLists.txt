cmake_minimum_required(VERSION 3.22.3)
project(Memoryleak)

set(CMAKE_CXX_STANDARD 11)

add_executable(Memoryleak main.cpp LeakDetector.cpp LeakDetector.h)
