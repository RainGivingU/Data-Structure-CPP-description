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

//迭代实现
template <typename T, typename VST>
void travPost_I(BinNodePosi(T) x, VST &visit)
{
    Stack<BinNodePosi(T)> S; //辅助栈
    if (x)
        S.push(); //根节点入栈
    while (!S.empty())
    {
        if (S.top() != x->parent) //如果栈顶不是当前节点的父亲(则必是其右兄)
            gotoHLVFL(S);         //再以其右兄为根的子树中找到HLVFL
        x = S.pop();              //弹出栈顶
        visit(x->data);           //并访问之
    }
}
template <typename T> //在以S栈顶节点为根的子树中找到最高左侧可见叶节点
static void gotoHLVFL(Stack<BinNodePosi(T)> &S)
{
    while (BinNodePosi(T) x = S.top()) //自顶而下，反复检查当前节点
        if (HasLChild(*x))             //尽可能向左
        {
            if (HasRChild(*x))     //若同时有左右孩子
                S.push(x->rChild); //右孩子优先入栈
            S.push(x->lChild);     //左孩子再入栈
        }
        else //没有左孩子
        {
            S.push(x->rChild); //右孩子入栈
        }
    S.pop(); //返回之前，弹出栈顶的空节点
}
