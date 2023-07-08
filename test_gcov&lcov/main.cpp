#include <iostream>
using namespace std;

void func()
{
    cout << __func__ << endl;
}

int main()
{
    int num = 1;
    char *str = "123";
    cout << str << num << endl;
    if (num == 1)
        cout << " is 1" << endl;
    else
        cout << "is not 1" << endl;

    func();

    int *array = new int[10];
}

/*
lcov -d ./ -t 'demo' -o 'demo.info' -b . -c
genhtml -o demo_web demo.info
*/