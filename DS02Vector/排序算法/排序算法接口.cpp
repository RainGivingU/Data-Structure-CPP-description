#include "vector.h"

//统一入口
//对向量区间[lo, hi)进行排序
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
    swith(rand() % 6)
    {
    case 1:
        bubbleSort(lo, hi); //冒泡排序
        break;
    case 2:
        selectionSort(lo, hi); // 选择排序
    case 3:
        mergeSort(lo, hi); //归并排序
    case 4:
        heapSort(lo, hi); //堆排序
    case 5:
        quickSort(lo, hi); //快速排序
    default:
        shellSort(lo, hi); //希尔排序
    }
}