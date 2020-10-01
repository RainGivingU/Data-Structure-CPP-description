#include <iostream>
#include <vector>
#define DEFAULT_CAPACITY 3
using namespace ::std;
typedef int Rank;

template <typename T>
class Vector
{
protected:
    Rank _size;    //规模,即有效元素的数量
    int _capacity; //容量
    T *_elem;      //数据存储区域
    /*内部函数*/
    void copyFrom(T const *A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
    void expand();                               //空间不足时扩容
    void shrink();                               //装填因子过小时压缩
    bool bubble(Rank lo, Rank hi);               //扫描交换
    Rank bubble2(Rank lo, Rank hi);
    void bubbleSort(Rank lo, Rank hi);     //起泡排序算法
    Rank max(Rank lo, Rank hi);            //选取最大元素
    void selectionSort(Rank lo, Rank hi);  //选择排序算法
    void merge(Rank lo, Rank mi, Rank hi); //归并算法
    void mergeSort(Rank lo, Rank hi);      //归并排序算法
    void heapSort(Rank lo, Rank hi);       //堆排序（稍后结合完全堆讲解）
    Rank partition(Rank lo, Rank hi);      //轴点构造算法
    void quickSort(Rank lo, Rank hi);      //快速排序算法
    void shellSort(Rank lo, Rank hi);      //希尔排序算法
public:
    /*构造函数*/
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v)
            ;
    }                          //s<=c
    Vector(T const *A, Rank n) //数组整体复制
    {
        copyFrom(A, 0, n);
    }
    Vector(T const *A, Rank lo, Rank hi) //数组区间复制
    {
        copyFrom(A, lo, hi);
    }
    Vector(Vector<T> const &V) //向量整体复制
    {
        copyFrom(V._elem, 0, V._size);
    }
    Vector(Vector<T> const &V, Rank lo, Rank hi) //向量区间复制
    {
        copyFrom(V._elem, lo, hi);
    }
    /*析构函数*/
    ~Vector() //释放内部空间
    {
        delete[] _elem;
    }
    /*只读接口*/
    Rank size() const { return _size; }                       //规模
    bool empty() const { return !_size; }                     //判空
    Rank find(T const &e) const { return find(e, 0, _size); } //无序向量整体查找
    Rank find(T const &e, Rank lo, Rank hi) const;            //无序向量区间查找
    Rank search(T const &e) const                             //有序向量整体查找
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    Rank search(T const &e, Rank lo, Rank hi) const; //有序向量区间查找
    int disordered() const;
    /*可写接口*/
    T &
    operator[](Rank r);                                  //重载下标操作符，可以类似于数组形式引用各元素
    const T &operator[](Rank r) const;                   //仅限于做右值的重载版本
    Vector<T> &operator=(Vector<T> const &);             //重载赋值操作符，以便直接克隆向量
    T remove(Rank r);                                    //删除秩为r的元素
    int remove(Rank lo, Rank hi);                        //删除秩在区间[lo, hi)之内的元素
    Rank insert(Rank r, T const &e);                     //插入元素
    Rank insert(T const &e) { return insert(_size, e); } //默认作为末元素插入
    void sort(Rank lo, Rank hi);                         //对[lo, hi)排序
    void sort() { sort(0, _size); }                      //整体排序
    void unsort(Rank lo, Rank hi);                       //对[lo, hi)置乱
    void unsort() { unsort(0, _size); }                  //整体置乱
    int deduplicate();                                   //无序去重
    int uniquify();                                      //有序去重
    /*遍历接口*/
    void traverse(void (*)(T &)); //遍历（使用函数指针，只读或局部性修改）
    template <typename VST>
    void traverse(VST &); //遍历（使用函数对象，可全局性修改）
};

//重载赋值运算符
template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> const &V)
{
    if (_elem)
        delete[] _elem;             //释放原内容
    copyFrom(V._elem, 0, V.size()); //整体复制
    return *this;                   //返回当前对象的引用，以便链式赋值
}

//重载下标运算符,仅作右值
template <typename T>
const T &Vector<T>::operator[](Rank r) const //重载下标运算符
{
    return _elem[r];
    // assert: 0 <= r < _size
}

//重载下标运算符,可作左值
template <typename T>
T &Vector<T>::operator[](Rank r) //重载下标运算符
{
    return _elem[r];
    // assert: 0 <= r < _size
}

template <typename T>
//以数组区间A[lo, hi)为蓝本复制向量
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)]; //分配空间，为何是两倍？
    _size = 0;                                //规模清零
    while (lo < hi)                           //A[lo, hi)中的元素逐一
        _elem[_size++] = A[lo++];             //复制到_elem[0, hi - lo)
}

template <typename T>
void Vector<T>::expand() //向量空间不足时扩容
{
    if (_size < _capacity) //尚未满员时，不必扩容
        return;
    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY; //不低于最小容量
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1]; //容量加倍
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i]; //复制原向量内容（T为基本类型或者‘=’是已重载赋值操作符）
    delete[] oldElem;          //释放原空间
}

template <typename T>
void Vector<T>::shrink() //装填因子过小时压缩向量所占空间
{
    if (_capacity < DEFAULT_CAPACITY << 1) //不致收缩到默认容量下
        return;
    if (_size << 2 > _capacity) //以25%为界
        return;

    T *oldElem = _elem;
    _elem = new T[_capacity >> 1];  //容量减半
    for (int i = 0; i < _size; i++) //复制原向量内容
        _elem[i] = oldElem[i];
    delete[] oldElem; //释放原向量空间
}

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