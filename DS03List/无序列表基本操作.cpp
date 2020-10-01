#include "List.h"

template <typename T>
void List<T>::init() //列表初始化，在调用列表时统一调用
{
    hearder = new ListNode<T>; //创建头哨兵节点
    trailer = new ListNode<T>; //创建尾哨兵节点
    header->succ = trailer;
    header->pred = NULL;
    trailer->pred = header;
    trailer->succ = NULL;
    _size = 0; //记录规模
}

//在无序列表内节点p(可以是trailer)的n个前驱中找到等于e的最后者
template <typename T>
ListNodePosi(T) List<T>::find(T const &e, int n, ListNodePosi(T) p) const
{
    while (0 < n--)                   //对于p的最近的n个前驱，从右向左
        if (e == (p = p->pred)->data) //逐个比对，直到命中或范围越界
            return p;                 //返回e的位置p
    return NULL;                      //查找失败，返回NULL
}

//插入接口
//把e当作首元素插入
template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const &e)
{
    _size++;
    return header->insertAsSucc(e);
}
//把e当作末元素插入
template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const &e)
{
    _size++;
    return trailer->insertAsPred(e);
}
//把e当作前驱插入
template <typename T>
ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const &e)
{
    _size++;
    return p->insertAsPred(e); //e的前插入
}
//把e当作后继插入
template <typename T>
ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const &e)
{
    _size++;
    return p->insertAsSucc(e); //e的后插入
}

//前插入：将新元素e作为当前节点的前驱插至列表
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const &e)
{
    //创建新节点，它的前驱是当前节点的前驱，后继为当前节点
    ListNodePosi(T) x = new ListNode(e, pred, this);
    pred->succ = x; //当前节点的前驱的后继设置为x
    pred = x;       //当前节点的前驱设置为x
    return x;       //返回新节点的位置
}

//后插入：将新元素e作为当前节点的后继插至列表
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const &e)
{
    //创建新节点：它的前驱是当前节点，后继为当前节点的后继
    ListNodePosi(T) x = new ListNode(e, this, succ);
    succ->pred = x;
    succ = x;
    return x;
}

//基于复制的构造
//内部方法：复制列表自位置p起的n项
template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)
{
    init();     //创建头尾哨兵节点并做初始化
    while (n--) //自p起的n项
    {
        insertAsLast(p->data); //将p作为末节点插入
        p = p->succ;           //将p设置为下一项
    }
}
//接口
template <typename T>
List<T>::List(ListNodePosi(T) p, int n) //复制列表中自p起的n项
{
    copyNodes(p, n);
}
template <typename T>
List<T>::List(List<T> const &L) //整体复制列表L
{
    copyNodes(L.first(), L.size());
}
template <typename T>
List<T>::List(List<T> const &L, int r, int n) //复制L中自第r项起的n项
{
    copyNodes(L[r], n);
}

//删除
template <typename T>
T List<T>::remove(ListNodePosi(T) p)
{
    T e = p->data;           //备份待删除节点的数值
    p->pred->succ = p->succ; //p的后继赋给p的前驱的后继
    p->succ->pred = p->pred; //p的前驱赋给p的后继的前驱
    delete p;                //释放节点
    _size--;                 //更新规模
    return e;                //返回备份数据
}

//析构
template <typename T>
List<T>::~List()
{
    clear();        //清空列表
    delete header;  //释放header
    delete trailer; //释放trailer
}
template <typename T>
int List<T>::clear() //清空列表
{
    int oldSize = _size;
    while (0 < _size)
        remove(header->succ) //反复删除首节点直至列表变空
}

//唯一化
template <typename T>
int List<T>::deduplicate()
{
    if (_size < 2)              //平凡情况
        return 0;               //自然没有重复
    int oldSize = _size;        //记录原规模
    ListNodePosi(T) p = header; //从首节点开始
    Rank r = 0;
    while (trailer != (p = p->succ)) //直到末节点结束
    {
        ListNodePosi(T) q = find(p->data, r, p); //在p的r个真前驱中查找雷同者
        q ? remove(q)                            //q存在，删除q
          : r++;                                 //q=NULL，查找的前驱数r加一，为后继节点的查找做准备
    }
    return oldSize - _size; //列表规模变化量，即删除元素的总数
}

//遍历
template <typename T>
void List<T>::traverse(void (*visit)(T &)) //利用函数指针遍历机制
{
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}
template <typename T>
template <typename VST>
void List<T>::traverse(VST &visit) //利用函数对象遍历机制
{
    for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}