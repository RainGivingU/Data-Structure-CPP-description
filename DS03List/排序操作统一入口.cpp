#include "list.h"
template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n)
{
    switch (rand() % 3)
    case 1:
        insertionSort(p, n); //插入排序
    break;
case 2:
    selectionSort(p, n); //选择排序
    break;
default:
    mergeSort(p, n); //归并排序
    break;
}