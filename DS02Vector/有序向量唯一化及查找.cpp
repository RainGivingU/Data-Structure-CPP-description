#include "vector.h"
#include "Fib.h"

//有序性甄别
template <typename T>
int Vector<T>::disordered() const //返回向量中逆序相邻元素对的总数
{
    int n = 0;
    for (int i = 1; i < _size; i++)  //逐一检查_size - 1对相邻元素
        if (_elem[i - 1] > _elem[i]) //如果发现逆序
            n++;                     //则计数
    return n;                        //返回逆序数，n=0时向量才可能有序
}

//有序向量重复元素剔除 低效版
// template <typename T>
// int Vector<T>::uniquify()
// {
//     int oldSize = _size;
//     int i = 1;
//     while (i < _size)
//         _elem[i - 1] == _elem[i] ? remove(i) : i++; //雷同则删除，不雷同就下一个
//     return oldSize - _size;                         //向量规模变化量，即删除元素个数
// }

//有序向量重复元素剔除 高效版
template <typename T>
int Vector<T>::uniquify()
{
    Rank i = 0, j = 0;             //各对互异相邻元素的秩
    while (++j < _size)            //逐一扫描，直到末元素
        if (_elem[i] != _elem[j])  //跳过雷同
            _elem[++i] = _elem[j]; //发现不同元素，向前移至紧邻前者右侧
    _size = i + 1;                 //截除尾部多余元素
    shrink();
    return j - i; //j是原规模-1，i是现规模-1，返回删除元素个数
}

//查找接口
template <typename T>
Rank Vector<T>::search(T const &e, Rank lo, Rank hi) const
{
    return (rand() % 2) ? binSearch(_elem, e, lo, hi)  //二分查找
                        : fibSearch(_elem, e, lo, hi); //或Fibonacci查找
}

//二分查找算法A
template <typename T>
static Rank binSearch(T *A, T const &e, Rank lo, Rank hi)
{
    while (lo < hi) //每次迭代有可能做两次比较判断，有三个分支
    {
        Rank mi = (lo - hi) >> 1; //以中心为轴点
        if (e < A[mi])            // 深入查找前半段
            hi = mi;
        else if (e > A[mi]) // 深入查找后半段
            lo = mi + 1;
        else // 已命中直接返回
            return mi;
    }
    return -1;
    //有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置
}

//Fibonacci查找算法
template <typename T>
static Rank fibSearch(T *A, T const &e, Rank lo, Rank hi)
{
    Fib fib(hi - lo); //获取不小于查找规模的最小项Fibonacci数
    while (lo < hi)
    {
        while (hi - lo < fib.get()) //同理，获取不小于当前查找规模的最小项Fibonacci数
            fib.prev();
        Rank mi = lo + fib.get() - 1; //确定形如Fib(k) - 1的轴点

        if (e < A[mi]) // 深入查找前半段
            hi = mi;
        else if (e > A[mi]) // 深入查找后半段
            lo = mi + 1;
        else // 已命中直接返回
            return mi;
    }
    return -1； //查找失败
}

//二分查找算法B
template <typename T>
static Rank binSearch(T *A, T const &e, Rank lo, Rank hi)
{
    while (1 < hi - lo) //直至有效查找区间的宽度缩短至1时算法终止
    {
        Rank mi = (lo - hi) >> 1;      // 取中点为轴点
        e < A[mi] ? hi = mi : lo = mi; // 子区间为[lo, mi)或[mi, hi)
    }
    return e == A[lo] ? lo : -1;
    //缩短至一个元素时，如果相等，返回该元素Rank；不等，说明查找失败，返回-1
}

//二分查找算法C
template <typename T>
static Rank binSearch(T *A, T const &e, Rank lo, Rank hi)
{
    while (lo < hi)
    {
        Rank mi = (lo + hi) >> 1;
        e < A[mi] ? hi = mi : lo = mi + 1; //子区间为[lo, mi)或(mi, hi)，注意和B版本的区别
    }                                      //出口处必有A[lo = hi] = M
    return --lo;                           //  A[lo - 1]
}