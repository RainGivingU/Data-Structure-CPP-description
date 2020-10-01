#include "vector.h"

template <typename T>
struct Increase //函数对象，递增一个T类函数
{
    virtual void operator()(T &e)
    {
        e++; //假设T可以直接递增或已重载++
    }
};

template <typename T>
void increase(Vector<T> &V) //统一递增向量中的各元素
{
    V.traverse(Increase<T>());
}