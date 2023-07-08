#!/bin/bash
g++ -Wall -g -fprofile-arcs -ftest-coverage main.cpp -o test.out # -c是只编译不链接
./test.out
lcov -d ./ -t 'testlcov' -o 'lcov.info' -b . -c
genhtml -o lcov_web lcov.info

# 教程博客：https://www.cnblogs.com/ChinaHook/p/5508660.html