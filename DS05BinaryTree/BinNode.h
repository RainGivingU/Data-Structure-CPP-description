#include <stddef.h>
#define BinNodePosi(T) BinNode<T> *         //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度，与"空树高度为-1"的约定相统一
typedef enum
{
    RB_RED,
    RB_BLACK
} RBColor; //节点颜色

template <typename T>
struct BinNode //二叉树节点模板类
{
    //成员，这里默认全开放
    T data;                //数值
    BinNodePosi(T) parent; //父节点
    BinNodePosi(T) lChild; //左孩子
    BinNodePosi(T) rChild; //右孩子
    int height;            //高度
    int npl;               //Null Path Length(左式堆，也可以直接用height代替)
    RBColor color;         //颜色(红黑树)
    //构造函数
    BinNode() : parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED) {}
    BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
            int h = 0, int l = 1, RBColor c = RB_RED)
        : data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(1), color(c) {}
    //操作接口
    int size();                           //统计当前节点后代总数，即以其为根的子树规模
    BinNodePosi(T) insertAsLC(T const &); //作为当前节点的左孩子插入新节点
    BinNodePosi(T) insertAsRC(T const &); //作为当前节点的右孩子插入新节点
    BinNodePosi(T) succ();                //取出当前节点的直接后继
    template <typename VST>
    void travLevel(VST &); //子树层次遍历
    template <typename VST>
    void travPre(VST &); //子树先序遍历
    template <typename VST>
    void travIn(VST &); //子树中序遍历
    template <typename VST>
    void travPost(VST &); //子树后序遍历
    //比较器、判等器
    bool operator<(BinNode const &bn) //小于
    {
        return data < bn.data;
    }
    bool operator==(BinNode const &bn) //等于
    {
        return data == bn.data;
    }
};

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const &e)
{
    //创建一个新的BinNode,data = e,parent = this
    //再将当前节点的lChild引用指向新节点，完成了双向关联
    return lChild = new BinNode(e, this);
} //O(1)

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const &e)
{
    return rChild = new BinNode(e, this);
} //O(1)

template <typename T>
int BinNode<T>::size()
{
    int s = 1; //计入自身
    //以下为统计所有子树上的节点数
    if (lChild)
        s += lChild->size(); //递归计入左子树规模
    if (rChild)
        s += rChild->size(); //递归计入右子树规模
    return s;
} //O(n = |size|)