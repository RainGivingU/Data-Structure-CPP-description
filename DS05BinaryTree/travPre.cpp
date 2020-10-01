#include "BinTree.h"

//先序遍历

//递归实现
template <typename T, typename VST>
void traverse(BinNodePosi(T) x, VST &visit)
{
    if (!x) //递归基
        return;
    visit(x->data);             //1.树根节点
    traverse(x->lChild, visit); //2.左子树
    traverse(x->rChild, visit); //3.右子树
} //T(n) = O(1) + T(a) + T(n-a-1) = O(n)

//迭代实现1
template <typename T, typename VST>
void travPre_I1(BinNodePosi(T) x, VST &visit)
{
    Stack<BinNodePosi(T)> S; //辅助栈
    if (x)
        S.push(x);     //根节点入栈
    while (!S.empty()) //在栈变空之前反复循环
    {
        x = S.pop();    //弹出当前节点
        visit(x->data); //访问当前节点
        if (HasRChild(*x))
            S.push(x->rChild); //右孩子先入后出
        if (HasLChild(*x))
            S.push(x->lChild); //左孩子后进先出
    }
}

template <typename T, typename VST> //由根节点x起沿藤下行
static void visitAlongVine(BinNodePosi(T) x, VST &visit, Stack<BinNodePosi(T)> &S)
{
    while (x) //若栈顶为NULL,则立即返回主算法
    {
        visit(x->data);    //访问当前节点
        S.push(x->rChild); //右孩子(右子树)入栈(将来逆序出栈)
        x = x->lChild;     //沿藤下行
    }
}

//迭代实现2
template <typename T, typename VST>
void travPre_I2(BinNodePosi(T) x, VST &visit)
{
    Stack<BinNodePosi(T)> S; //辅助栈
    while (true)             //以右子树为单位，逐批访问节点
    {
        visitAlongVine(x, visit, S); //访问子树x的藤，各右子树(根)入栈缓冲
        if (S.empty())
            break;   //栈空即退出
        x = S.pop(); //弹出下一右子树(根)
    }
}