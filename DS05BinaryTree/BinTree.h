#include <iostream>
#include "../DS04Stack-Queue/Stack.h"
#include "../DS04Stack-Queue/Queue.h"
#include "BinNode.h"
template <typename T>
class BinTree //二叉树模板类
{
protected:
    int _size;                                  //规模
    BinNodePosi(T) _root;                       //根节点
    virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);   //更新节点x及其祖先的高度
public:
    BinTree() : _size(0), _root(NULL) {} //构造函数
    ~BinTree()
    {
        if (0 < _size) //析构函数
            remove(_root);
    }
    int size() const //规模
    {
        return _size;
    }
    bool empty() const //判空
    {
        return !_root;
    }
    BinNodePosi(T) root() const //树根
    {
        return _root;
    }
    BinNodePosi(T) insertAsRoot(T const &e);                     //插入根节点
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const &e);     //e作为左孩子插入
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const &e);     //e作为右孩子插入
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T> *&T); //T作为x的左子树接入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T> *&T); //T作为x的右子树接入
    int remove(BinNodePosi(T) x);                                //删除以位置x处节点为根的子树，返回该子树的规模
    BinTree<T> *secede(BinNodePosi(T) x);                        //将子树x从当前树中摘除，将其转换为一棵独立的子树
    template <typename VST>
    void travLevel(VST &visit) //层次遍历
    {
        if (_root)
            _root->travLevel(visit);
    }
    template <typename VST> //先序遍历
    void travPre(VST &visit)
    {
        if (_root)
            _root->travPre(visit);
    }
    template <typename VST> //中序遍历
    void travIn(VST &visit)
    {
        if (_root)
            _root->travIn(visit);
    }
    template <typename VST> //后序遍历
    void travPost(VST &visit)
    {
        if (_root)
            _root->travPost(visit);
    }
    //比较器、判等器
    bool operator<(BinTree<T> const &t)
    {
        return _root && t._root && lt(_root, t._root);
    }
    bool operator==(BinTree<T> const &t)
    {
        return _root && t._root && (_root == t._root);
    }
};

//高度更新
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //更新节点高度
{
    return x->height = 1 + max(stature(x->lChild), stature(x->rChild);)
} //O(1)

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) //更新v及祖先的高度
{
    while (x)
    {
        if (x->height == updateHeight(x))
            break;
        x = x->parent;
    } //O(n = depth(x))
}

//节点插入
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const &e)
{
    _size = 1;
    return _root = new BinNode<T>(e); //将e当作根节点插入空的二叉树
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const &e) //e插入为x的左孩子
{
    _size++;              //规模必+1
    x->insertAsLC(e);     //这里调用了节点类中的insertAsLC
    updateHeightAbove(x); //更新x及其祖先的高度
    return x->lChild;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const &e) //e插入为x的右孩子
{
    _size++;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rChild;
}

//子树接入
template <typename T> //将S当作节点x的左子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T> *&S)
{
    if (x->lChild = S->_root)
        x->lChild->parent = x; //接入
    _size += S->_size;         //更新规模
    updateHeightAbove(x);      //更新高度
    S->_root = NULL;
    S->_size = 0;
    S = NULL; //释放原树
    return x; //返回接入位置
}

template <typename T> //将S当作节点x的右子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T> *&S)
{
    if (x->rChild = S->_root)
        x->rChild->parent = x; //接入
    _size += S->_size;         //更新规模
    updateHeightAbove(x);      //更新高度
    S->_root = NULL;
    S->_size = 0;
    S = NULL; //释放原树
    return x; //返回接入位置
}

//子树删除
template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
int BinTree<T>::remove(BinNodePosi(T) x)
{
    FromParentTo(*x) = NULL;      //切断来自父节点的指针
    updateHeightAbove(x->parent); //更新祖先高度
    int n = removeAt(x);          //删除子树x
    _size -= n;                   //更新规模
    return n;                     //返回删除节点总数
}
template <typename T> //删除二叉树位置x处的节点及其后代，返回被删除节点的数值
static int removeAt(BinNodePosi(T) x)
{
    if (!x)
        return 0;                                          //递归基：空树
    int n = 1 + removeAt(x->lChild) + removeAt(x->rChild); //递归释放左、右子树
    release(x->data);                                      //释放被删除节点
    release(x);
    return n; //返回删除节点总数
}

//子树分离
template <typename T> //将子树x从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T> *BinTree<T>::secede(BinNodePosi(T) x)
{
    FromParentTo(*x) = NULL;        //切断来自父节点的指针
    updateHeightAbove(x->parent);   //更新原树中所有祖先的高度
    BinTree<T> *S = new BinTree<T>; //创建新树
    S->_root = x;                   //新树以x为根；
    x->parent = NULL;
    S->_size = x->size(); //更新新树规模
    _size -= S->_size;    //更新当前树规模
    return S;             //返回新树
}