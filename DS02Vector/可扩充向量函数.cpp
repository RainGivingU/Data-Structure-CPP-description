#include "vector.h"

template <typename T>
//以数组区间A[lo, hi)为蓝本复制向量
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)]; //分配空间，为何是两倍？
    _size = 0;                                //规模清零
    while (lo < hi)                           //A[lo, hi)中的元素逐一
        _elem[_size++] = A[lo++];             //复制到_elem[0, hi - lo)
}

template <typename T>
void Vector<T>::expand() //向量空间不足时扩容
{
    if (_size < _capacity) //尚未满员时，不必扩容
        return;
    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY; //不低于最小容量
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1]; //容量加倍
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i]; //复制原向量内容（T为基本类型或者‘=’是已重载赋值操作符）
    delete[] oldElem;          //释放原空间
}

template <typename T>
void Vector<T>::shrink() //装填因子过小时压缩向量所占空间
{
    if (_capacity < DEFAULT_CAPACITY << 1) //不致收缩到默认容量下
        return;
    if (_size << 2 > _capacity) //以25%为界
        return;

    T *oldElem = _elem;
    _elem = new T[_capacity >> 1];  //容量减半
    for (int i = 0; i < _size; i++) //复制原向量内容
        _elem[i] = oldElem[i];
    delete[] oldElem; //释放原向量空间
}