#include <iostream>
#include "LeakDetector.h"
using namespace std;

int main() {
    // �����ͷ�ָ�� b ������ڴ�, �Ӷ������ڴ�й¶
    int *a = new int;
    int *b = new int[12];

    delete a;

    return 0;
}