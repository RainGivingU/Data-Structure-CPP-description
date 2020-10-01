#include "BinTree.h"

template <typename T>
template <typename VST>
void BinNode<T>::travLevel(VST &visit) //二叉树层次遍历
{
    Queue<BinNodePosi(T)> Q; //引入辅助队列
    Q.enqueue(this);         //根节点入队
    while (!Q.empty())       //在节点变空之前反复迭代
    {
        BinNodePosi(T) x = Q.dequeue; //取出队首节点，并随即
        visit(x->data);               //访问之
        if (HasLChild(*x))
            Q.enqueue(x->lChild); //左孩子入队
        if (HasRChild(*x))
            Q.enqueue(x->rChild); //右孩子入队
    }
}