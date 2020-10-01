#include "vector.h"

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (hi - lo < 2) // 单元素区间自然有序，否则继续向下执行
        return;

    int mi = (lo + hi) >> 1; //以中点为界
    mergeSort(lo, mi);       // 对前半段排序
    mergeSort(mi, hi);       //对后半段排序
    merge(lo, mi, hi);       //归并
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
    T *A = _elem + lo; //合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo;
    T *B = new T[lb]; //前子向量B[0, lb) = _elem[lo, mi)
    for (Rank i = 0; i < lb; B[i] = A[i++])
        ; //复制前子向量B
    int lc = hi - mi;
    T *C = _elem + mi; //后子向量C[0, lc) = _elem[mi, hi)

    //归并：反复从B和C首元素中取出更小者归入A
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
    {
        //每次接纳一个元素后，A通过自加指向下一个位置
        if ((j < lb) && (lc <= k || (B[j] <= C[k])))
            A[i++] = B[j++];
        if ((k < lc) && (lb <= j || (C[k] < B[j])))
            A[i++] = C[k++];
    }

    delete[] B; //释放临时空间B
}