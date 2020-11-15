#include "BST.h"
#define Balanced(x) \
    (stature((x).lChild) == stature((x).rChild)) //理想平衡
#define BalFac(x) \
    (stature((x).lChild) - stature((x).rChild)) //平衡因子
#define AvlBalanced(x) \
    ((-2 < BalFax(x)) && (BalFac(x) < 2)) //AVL平衡条件
#define tallerChild(x) (                                                                                                                                                                                        \
    stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild : (                                                                                                        /*左高*/                             \
                                                                 stature((x)->lChild) < stature((x)->rChild) ? (x)->rChild : (                                           /*右高*/                             \
                                                                                                                              IsLChild(*(x)) ? (x)->lChild : (x)->rChild /*等高：与父亲x同侧者优先*/ \
                                                                                                                              )))

template <typename T>
class AVL : public BST<T> //BST派生
{
public:
    //search()等接口直接沿用
    BinNodePosi(T) insert(const T &); //重写插入
    bool remove(const T &);           //重写删除
};

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if (x)        //若要插入的节点已经存在
        return x; //直接返回已存在的节点
    BinNodePosi(T) xx = x = new BinNode<T>(e, _hot);
    _size++; //创建新节点
    //此时，若x的父亲_hot增高，则其祖父有可能失衡
    for (BinNodePosi(T) g = _hot; g; g = g->parent) //从x之父向上检查各代祖先g
    {
        if (!AvlBalanced(*g)) //一旦发现g失衡，采用3+4算法使之复衡
        {
            FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g))); //重新接入原树
            break;                                                    //g复衡后局部子树高度复原，则整棵树到达平衡
        }
        else                 //g依然平衡
            updateHeight(g); //更新其高度
    }
    return xx; //返回新节点位置
} //无论e是否存在于原树，AVL::insert(e)->data == e

template <typename T>
bool AVL<T>::remove(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if (!x)            //目标不存在
        return false;  //返回删除失败
    removeAt(x, _hot); //物理删除x
    _size--;
    //从_hot出发逐层向上，依次检查各代祖先g
    for (BinNodePosi(T) g = _hot; g; g = g->parent)
    {
        if (!AvlBalanced(*g)) //一旦发现g失衡，就通过调整恢复平衡
            g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
        updateHeight(g); //更新高度
    }                    //可能需要O(logn)次调整，全树高度有可能下降
    return true;         //删除成功
}
