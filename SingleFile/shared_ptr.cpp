//
// Created by Ginkgowind on 2022/10/3.
//

#include <iostream>
using namespace std;

template<typename T>
class SharedPtr {
private:
    int *m_count_;
    T *m_ptr_;
public:
    //构造函数
    SharedPtr() : m_ptr_(nullptr), m_count_(new int) {}

    SharedPtr(T *ptr) : m_ptr_(ptr), m_count_(new int) { *m_count_ = 1; }

    //析构函数
    ~SharedPtr() {
        --(*m_count_);
        if (*m_count_ == 0) {
            delete m_ptr_;
            delete m_count_;
            m_ptr_ = nullptr;
            m_count_ = nullptr;
        }
    }

    //拷⻉构造函数
    SharedPtr(const SharedPtr &ptr) {
        m_count_ = ptr.m_count_;
        m_ptr_ = ptr.m_ptr_;
        ++(*m_count_);
    }

    //拷⻉赋值运算
    void operator=(const SharedPtr &ptr) { SharedPtr(std::move(ptr)); }

    //移动构造函数
    SharedPtr(SharedPtr &&ptr) : m_ptr_(ptr.m_ptr_), m_count_(ptr.m_count_) { ++(*m_count_); }

    //移动赋值运算
    void operator=(SharedPtr &&ptr) { SharedPtr(std::move(ptr)); }

    //解引⽤
    T &operator*() { return *m_ptr_; }

    //箭头运算
    T *operator->() { return m_ptr_; }

    //重载bool操作符
    operator bool() { return m_ptr_ == nullptr; }

    T *get() { return m_ptr_; }

    size_t use_count() { return *m_count_; }

    bool unique() { return *m_count_ == 1; }

    void swap(SharedPtr &ptr) { std::swap(*this, ptr); }
};

int main() {
    int a = 5;
    int b = 4;
    SharedPtr<int> p1 = SharedPtr<int>(&a);
    SharedPtr<int> p2 = SharedPtr<int>(&a);
    cout << p1.use_count() << endl;
    cout << p2.use_count() << endl;
    p2 = p1;
    cout << p1.use_count() << endl;
    cout << p2.use_count() << endl;
}