#include "BinTree.h"

//递归实现
template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travPost_R(x->lChild, visit);
    travPost_R(x->rChild, visit);
    visit(x->data);
}