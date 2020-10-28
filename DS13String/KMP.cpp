#include <iostream>
using namespace ::std;
int match(string P, string T)
{
    int *next = buildNext(P);
    int n = T.size(), i = 0;
    int m = P.size(), j = 0;
    while (j < m && i < n)
        if (j < 0 || T[i] == P[j])
        {
            ++i;
            ++j;
        }
        else
            j = next[j];
    delete[] next;
    return i - j;
}

int *buildNext(string P)
{
    size_t m = P.size(), j = 0;
    int *N = new int[m];
    int t = N[0] = -1;
    while (j < m - 1)
    {
        (t < 0 || P[j] == P[t]) ? N[++j] = ++t : t = N[t];
    }
    return N;
}