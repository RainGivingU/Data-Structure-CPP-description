#include <iostream>
using namespace ::std;

_int64 fib1(int n);
_int64 fib2(int n, _int64 &prev);
_int64 fib3(int n);
//二分递归方法：速度在n>45左右变得肉眼可见的慢，时间复杂度是指数级别
_int64 fib1(int n)
{
    return (n < 2) ? n : fib1(n - 1) + fib1(n - 2);
}

//线性递归方法

_int64 fib2(int n, _int64 &prev)
{
    if (n == 0) //若到达了递归基即fib(0)
    {
        prev = 1;
        return 0;
    }
    else
    {
        _int64 prevPrev;
        prev = fib2(n - 1, prevPrev); //递归计算前两项
        return prev + prevPrev;       //其和即位正解
    }
}
//迭代：动态规划 由自顶向下求值改为自上到下，时间复杂度为O(n)
_int64 fib3(int n)
{
    _int64 f = 0, g = 1;
    if (n == 0)
        return 0;

    while (n-- > 0) //O(n)
    {
        g = g + f;
        f = g - f;
    }
    return g;
}

int main()
{
    int n;
    cin >> n;
    for (auto i = 0; i <= n; ++i)
        cout << fib3(i) << endl;
}