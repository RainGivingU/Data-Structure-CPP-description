#include "vector.h"

//插入
template <typename T>
Rank Vector<T>::insert(Rank r, T const &e)
{
    expand();                       //如有必要则扩容
    for (int i = _size; i > r; i--) //自后向前
        _elem[i] = _elem[i - 1];    //将后继元素顺次后移一个单元
    _elem[r] = e;                   //置入新元素
    _size++;                        //更新规模
    return r;                       //返回秩
}

//区间删除
template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi) //出于对效率的考虑，单独处理退化情况
        return 0;
    while (hi < _size)
        _elem[lo++] = _elem[hi++]; //[hi,_size)顺次前移hi-lo个单位
    _size = lo;                    //更新规模，这里的lo经过循环更新后就等于当前规模
    shrink();                      //如有必要，进行缩容
    return hi - lo;                //返回被删除元素数目
}

//单元素删除
template <typename T>
T Vector<T>::remove(Rank r)
{
    T e = _elem[r];   //备份被删除元素
    remove(r, r + 1); //等效对区间[r, r+1)的删除
    return e;         //返回被删除元素
}

//无序向量的顺序查找
template <typename T>
Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const
{
    while ((lo < hi--) && (e != _elem(hi))) //从后向前顺序查找
        return hi;                          // 如果hi < lo，则说明查找失败；否则hi就是命中元素的Rank
}

//无序向量的唯一化(去重)
template <typename T>
int Vector<T>::deduplicate()
{
    int oldSize = _size;                        //记录原规模
    Rank i = 1;                                 //从_elem[1]开始
    while (i < _size)                           //自前向后逐一考察各元素
        (find(_elem[i], 0, i) < 0) ?            //前缀中是否存在雷同者
            i++                                 //不存在-继续查找下一个元素；
                                   : remove(i); //存在-删除该元素
    return oldSize - _size;                     //返回向量规模变化量，即删除元素数
}

//遍历
template <typename T>
void Vector<T>::traverse(void (*visit)(T &)) //函数指针，只读或局部性修改
{
    for (int i = 0; i < _size; i++)
        visit(_elem[i]);
}

template <typename T>
template <typename VST>
void Vector<T>::traverse(VST &visit) //函数对象，全局性修改更便捷
{
    for (int i = 0; i < _size; i++)
        visit(_elem[i]);
}

//置乱
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi) //等概率随机置乱向量区间[lo, hi)
{
    T *V = _elem;                      // 将子向量_elem[lo, hi)视作另一向量V[0, hi-lo)
    for (Rank i = hi - lo; i > 0; i--) //自后向前
        swap(V[i - 1], V[rand() % i]); //将V[i-1]与V[0,i)中某一元素随机交换
}