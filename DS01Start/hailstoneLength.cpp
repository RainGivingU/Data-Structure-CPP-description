#include <iostream>
using namespace ::std;

//计算Hailstone序列的长度
int hailstone(int n)
{
    int length = 1;
    while (1 < n)
    {
        n % 2 ? n = 3 *n + 1 : n /= 2;
        ++length;
    }
    return length;
}

int main()
{
    int n;
    cin >> n;
    cout << hailstone(n);
}