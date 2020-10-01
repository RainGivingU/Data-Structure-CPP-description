#include "list.h"
//对起始位置为p的n个元素排序
template <typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n)
{
    ListNodePosi(T) head = p->pred;
    ListNodePosi(T) tail = p;
    for (int i = 0; i < n; i++)
        tail = tail->succ; //将tail位置设置为p+n
    while (1 < n)
    {
        ListNodePosi(T) max = selectMax(head->succ, n); //在[p, n)中找最大者
        insertBefore(tail, remove(max));                //将最大者删除的同时将最大者插入到tail前
        tail = tail->pred;
        n--;
    }
}

template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n)
{
    ListNodePosi(T) max = p;                  //暂定首节点为max
    for (ListNodePosi(T) cur = p; 1 < n; n--) //从首节点p出发将后续节点依次与max比较
        //!lt = not less than
        if (!lt((cur = cur->succ)->data, max->data)) //若前者不小于后者
            max = cur;                               //更新最大元素位置记录
    return max;                                      //返回最大节点位置
}