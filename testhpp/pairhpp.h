// pairhpp.h文件

#pragma once

template <class T1, class T2>
class Pair
{
public:
    T1 key;   // 关键字
    T2 value; // 值

public:
    Pair(T1 k, T2 v);
    bool operator<(const Pair<T1, T2> &p) const;
};

template <class T1, class T2>
Pair<T1, T2>::Pair(T1 k, T2 v) : key(k), value(v)
{
}

template <class T1, class T2>
bool Pair<T1, T2>::operator<(const Pair<T1, T2> &p) const
{
    return key < p.key;
}