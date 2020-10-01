#include "list.h"

//对起始于位置p的n个元素排序
template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n)
{
    for (int r = 0; r < n; r++) //为各节点逐一
    {
        //在p的r个前驱中(由右至左)查找不大于p->data的位置，在该位置后插入p->data
        insertAfter(search(p->data, r, p), p->data);
        p = p->succ;     //转向下一节点
        remove(p->pred); //此时p已经指向原p的后继，删除p->pred就是删除原先完成插入的p
    }
}