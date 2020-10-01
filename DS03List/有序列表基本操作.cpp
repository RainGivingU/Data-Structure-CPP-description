#include "list.h"

//唯一化
template <typename T>
int List<T>::uniquify()
{
    if (_size < 2)       //平凡情况
        return 0;        //自然无重复
    int oldSize = _size; //记录原规模
    ListNodePosi(T) p = first();
    ListNodePosi(T) q;
    while (trailer != (q = p->succ)) //反复比对紧邻的节点对(p, q)
        if (p->data != q->data)      //若互异
            p = q;                   //则转向下一对
        else                         //若雷同
            remove(q);               //删除后者
    return oldSize - _size;          //返回规模变化量，即被删除元素的总数
}

//查找
template <typename T>
ListNodePosi(T) List<T>::search(T const &e, int n, ListNodePosi(T) p) const //在p的n个前驱中(由右向左)查找e
{
    while (0 <= n--)                    //对于p的最近的n个前驱，从右向左逐个比较
        if (((p = p->pred)->data) <= e) //若未命中
            break;                      //终止本次循环开始下一次
    return p;                           //直到命中(此时的p应为相同大小元素中位置最靠后的)
}