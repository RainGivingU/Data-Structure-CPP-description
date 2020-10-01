#include "../DS02Vector/vector.h"
#include <vector>
template <typename T>
class Stack : public Vector<T>
{
public:
    //入栈，等效于将新元素作为向量末元素插入
    void push(T const &e) { this->insert(this->size(), e); }
    //出栈，等效于将向量末元素删除
    T pop() { return this->remove(this->size() - 1); }
    //取项，直接返回向量末元素
    T &top() { return (*this)[this->size() - 1]; }
    //size(),empty()等开放接口可直接沿用
};
