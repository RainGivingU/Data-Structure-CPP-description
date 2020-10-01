#include <iostream>
using namespace ::std;

void reverse(int *A, int lo, int hi)
{
    if (lo < hi)
    {
        swap(A[lo], A[hi]); // 交换
        reverse(A, lo + 1, hi - 1);
    }
}

void swap(int &a, int &b)
{
    auto temp = a;
    a = b;
    b = temp;
}

int main()
{
    int A[9] = {1, 2, 4, 8, 4, 6, 9, 9, 78};
    reverse(A, 0, sizeof(A) / sizeof(int) - 1);
    for (auto i = 0; i < sizeof(A) / sizeof(int); ++i)
        cout << A[i] << endl;
}