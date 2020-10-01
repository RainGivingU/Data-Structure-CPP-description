#include "List.h"
template <typename T>
T &List<T>::operator[](Rank r) const
{
    ListNodePosi(T) p = first(); //从首节点出发
    while (0 < r--)
        p = p->succ; //顺数第r个节点即是
    return p->data;  //返回目标节点所存元素
}