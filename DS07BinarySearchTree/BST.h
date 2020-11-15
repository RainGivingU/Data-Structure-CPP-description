#include "../DS05BinaryTree/BinTree.h"

template <typename T>
class BST : public BinTree<T> //由BinTree派生
{
public:
    //使用virtual修饰，以便派生类(BST的变种)重写
    virtual BinNodePosi(T) & search(const T &); //查找
    virtual BinNodePosi(T) insert(const T &);   //插入
    virtual bool remove(const T &);             //删除
protected:
    BinNodePosi(T) _hot; //命中节点的父亲
    BinNodePosi(T) connect34(
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
    BinNodePosi(T) rotateAt(BinNodePosi(T)); //旋转调整
};

template <typename T>
    BinNodePosi(T) & BST<T>::search(const T &e)
{
    return searchIn(_root, e, _hot = NULL); //从根节点启动查找
}

template <typename T>
    static BinNodePosi(T) & searchIn(
                                BinNodePosi(T) & v,    //当前(子)树根
                                const T &e,            //目标关键码
                                BinNodePosi(T) & _hot) //记忆热点
{
    if (!v || e == v->data) //尾递归，确认找不到 or 已找到
        return v;
    _hot = v; //记下当前(一定是非空)节点
    return searchIn(((e < v->data) ? v->lChild : v->rChild), e, _hot);
}

template <typename T>
BinNodePosi(T) BST<T>::insert(const T &e)
{
    BinNodePosi(T) &x = search(e); //查找目标(注意_hot的初值设置)
    if (!x)                        //查找失败时才执行插入操作
    {
        x = new BinNode<T>(e, _hot); //在x处创建新节点，也_hot为父亲
        _size++;                     //更新全树规模
        updateHeightAbove(x);        //更新x即其历代祖先高度
    }
    return x; //无论e是否存在于原树中，此时总有x->data == e
} //边界情况可以顺利处理

template <typename T>
bool BST<T>::remove(const T &e)
{
    BinNodePosi(T) &x = search(e); //定位目标节点
    if (!x)                        //确认目标存在，若不存在
        return false;              //直接返回
    removeAt(x, _hot);             //分两大类情况实施删除
    _size--;                       //更新全树规模
    updateHeightAbove(_hot);       //更新_hot及其历代祖先的高度
    return true;                   //返回值指示删除成功与否
}

template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) & x, BinNodePosi(T) & hot)
{
    BinNodePosi(T) w = x;       //实际被摘除的节点，初值同x
    BinNodePosi(T) succ = NULL; //实际被删除节点的接替者
    if (!HasLChild(*x))         //左子树为空(左右子树都为空的情况也在此处理了)
        succ = x = x->rChild;   //接替者为右子树
    else if (!HasRChild(*x))    //右子树为空
        succ = x = x->lChild;   //接替者为左子树
    else
    {
        //左右子树并存的情况
        w = w->succ();
        swap(x->data, w->data); //*x与其后继*w交换数据
        BinNodePosi(T) u = w->parent;
        //问题转化为摘除单分支的节点w，它的父亲是u
        //w作为直接后继必然没有左孩子，w的继任者必定是它的右孩子
        succ = w->rChild;
        //u是w的父亲必是w->rChild的祖父，但w->rChild继任后要将其设定为u的孩子
        if (u == x)
            u->rChild = succ;
        else
            u->lChild = succ;
    }
    hot = w->parent; //记录实际被删除节点的父亲
    if (succ)
        succ->parent = hot; //被删除节点的接替者与hot相联
    release(w->data);
    release(w);  //释放被摘除节点
    return succ; //返回接替者
}

template <typename T>
BinNodePosi(T) BST<T>::connect34(
    BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
    BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3)
{
    a->lChild = T0;
    if (T0)
        T0->parent = a;
    a->rChild = T1;
    if (T1)
        T1->parent = a;
    c->lChild = T2;
    if (T2)
        T2->parent = c;
    c->rChild = T3;
    if (T3)
        T3->parent = c;
    b->lChild = a;
    a->parent = b;
    b->rChild = c;
    c->parent = b;
    updateHeight(a);
    updateHeight(c);
    updateHeight(b);
    return b;
}

template <typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)
{
    BinNodePosi(T) p = v->parent, g = p->parent;
    if (IsLChild(*p))     //zig
        if (IsLChild(*v)) //zig-zig
        {
            p->parent = g->parent; //向上联接
            return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
        }
        else //zag-zig(先zag再zig)注意，双旋时v成为该局部的根(即b)
        {
            v->parent = g->parent;
            return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
        }
    else                  //zag
        if (IsRChild(*p)) //zag-zag
    {
        p->parent = g->parent;
        return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
    }
    else //zig-zag(先zig再zag)
    {
        v->parent = g->parent;
        return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
    }
}