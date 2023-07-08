// main.cpp文件

#include <iostream>
#include <string>

#ifdef H
#include "pair.h"
#elif defined(HPP)
#include "pair.hpp"
#elif defined(HPPH)
#include "pairhpp.h"
#endif

#if VAL == 1
#include "pair.h"
#elif VAL == 2
#include "pair.hpp"
#elif VAL == 3
#include "pairhpp.h"
#endif

int main()
{
    Pair<std::string, int> student("kaizenly", 19); // 实例化出一个类 Pair<std::string, int>
    std::cout << "key: " << student.key << " | "
              << "value: " << student.value;
    return 0;
}