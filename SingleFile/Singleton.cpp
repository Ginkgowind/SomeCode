//
// Created by Ginkgowind on 2022/10/3.
//

#include<iostream>

using namespace std;

class CSingleton {
private:
    CSingleton() { cout << "create" << endl; };

    CSingleton(const CSingleton &);

    CSingleton &operator=(const CSingleton &);

    ~CSingleton() { cout << "destory" << endl; };

    static CSingleton myInstance; // 懒汉式，第一次使用时才初始化（饿汉式，定义时就直接初始化）
public:
    static CSingleton *getInstance() {
        return &myInstance;
    }
};

CSingleton CSingleton::myInstance;
// CSingleton CSingleton::myInstance;

int main(int argc, char *argv[]) {
    // CSingleton::myInstance;
    CSingleton *ct1 = CSingleton::getInstance();
    CSingleton *ct2 = CSingleton::getInstance();
    CSingleton *ct3 = CSingleton::getInstance();

    return 0;
}