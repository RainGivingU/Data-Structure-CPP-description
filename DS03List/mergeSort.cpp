#include "list.h"

template <typename T> //有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
void List<T>::merge(ListNodePosi(T) & p, int n, List<T> &L, ListNodePosi(T) q, int m)
{
    // assert: this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
    // L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
    // 注意：在归并排序之类的场合，有可能 this == L && rank(p) + n = rank(q)
    ListNodePosi(T) pp = p->pred;            //借助前驱（可能是header），以便返回前 ...
    while (0 < m)                            //在q尚未移出区间之前
        if ((0 < n) && (p->data <= q->data)) //若p仍在区间内且v(p) <= v(q)，则
        {
            if (q == (p = p->succ))
                break;
            n--;
        }    //将p替换为其直接后继（等效于将p归入合并的列表）
        else //若p已超出右界或v(q) < v(p)，则
        {
            insertBefore(p, L.remove((q = q->succ)->pred));
            m--;
        }         //将q转移至p之前
    p = pp->succ; //确定归并后区间的（新）起点
}

template <typename T> //列表癿弻幵排序算法：对起始亍位置p癿n个元素排序
void List<T>::mergeSort(ListNodePosi(T) & p, int n)
{ //valid(p) && rank(p) + n <= size
    if (n < 2)
        return;     //若待排序范围已足够小，则直接返回；否则...
    int m = n >> 1; //以中点为界
    ListNodePosi(T) q = p;
    for (int i = 0; i < m; i++)
        q = q->succ; //均分列表
    mergeSort(p, m);
    mergeSort(q, n - m);          //对前、后子列表分别排序
    merge(p, m, *this, q, n - m); //归并
} //注意：排序后，p依然指向归并后区间的（新）起点