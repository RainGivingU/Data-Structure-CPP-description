#include "BinTree.h"

//递归实现
template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST &visit)
{
    if (!x)
        return;
    travIn_R(x->lChild, visit);
    visit(x->data);
    travIn_R(x->rChild, visit);
}

//迭代实现
template <typename T>
static void goAlongVine(BinNodePosi(T) x, Stack<BinNodePosi(T)> &S)
{
    while (x)
    {
        S.push(x);
        x = x->lChild;
    } //逐层深入，沿藤曼各节点一次入栈
}

template <typename T, typename V>
void travIn(BinNodePosi(T) x, V &visit)
{
    Stack<BinNodePosi(T)> S; //辅助栈
    while (true)             //反复地
    {
        goAlongVine(x, S); //从当前节点出发，逐批入栈
        if (S.empty())
            break;      //直至所有节点处理完毕
        x = S.pop();    //x的左子树或为空或已遍历(等效为空)
        visit(x->data); //所以立即访问之
        x = x->rChild;  //转向其右子树(可能为NULL，此时不会有任何动作)
    }
}