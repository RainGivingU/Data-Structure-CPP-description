#include "vector.h"

//重载赋值运算符
template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> const &V)
{
    if (_elem)
        delete[] _elem;             //释放原内容
    copyFrom(V._elem, 0, V.size()); //整体复制
    return *this;                   //返回当前对象的引用，以便链式赋值
}

//重载下标运算符,仅作右值
template <typename T>
const T &Vector<T>::operator[](Rank r) const //重载下标运算符
{
    return _elem[r];
    // assert: 0 <= r < _size
}

//重载下标运算符,可作左值
template <typename T>
T &Vector<T>::operator[](Rank r) //重载下标运算符
{
    return _elem[r];
    // assert: 0 <= r < _size
}