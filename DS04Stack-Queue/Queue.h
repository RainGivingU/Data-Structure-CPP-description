#include "../DS03List/list.h"
template <typename T>
class Queue : public List<T>
{
public:                                           //size()和empty()直接沿用即可
    void enqueue(T const &e) { insertAsLast(e); } //入队
    T dequeue() { return remove(fist()); }        //出队
    T &front() { return first()->data; }          //队首
};