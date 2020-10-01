#include "vector.h"

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    while (!bubble(lo, hi--))
        ;
}

template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
    bool sorted = true; //假设整体有序
    while (++lo < hi)
        if (_elem[lo - 1] > _elem[lo]) //若发现逆序
        {
            sorted = false;                 //  则整体未达到有序
            swap(_elem[lo - 1], _elem[lo]); // 交换逆序对使之有序
        }
    return sorted; //返回有序标志
}

//再改进
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    while (lo < (hi = bubble2(lo, hi)))
        ;
}

template <typename T>
Rank Vector<T>::bubble2(Rank lo, Rank hi)
{
    Rank last = lo; //最右侧逆序对初始化为[lo - 1, lo]
    while (++lo < hi)
        if (_elem[lo - 1] > _elem[lo]) //若发现逆序
        {
            last = lo; //更新最右侧逆序位置记录
            swap(_elem[lo - 1], _elem[lo])
        }
    return last;
}