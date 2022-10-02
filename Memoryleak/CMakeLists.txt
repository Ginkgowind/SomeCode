cmake_minimum_required(VERSION 3.23)
project(Memoryleak)

set(CMAKE_CXX_STANDARD 11)

add_executable(Memoryleak main.cpp LeakDetector.cpp LeakDetector.h)
