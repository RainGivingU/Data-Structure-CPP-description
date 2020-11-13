#include <iostream>
using namespace ::std;
int match(char *P, char *T)
{
    int *next = buildNext(P);      //构造next表
    int n = (int)strlen(T), i = 0; //文本串指针
    int m = (int)strlen(P), j = 0; //模式串指针
    while (j < m && i < n)
        if (j < 0 || T[i] == P[j]) //若匹配
        {
            //携手共进
            ++i;
            ++j;
        }
        else             //若失配
            j = next[j]; //P查表右移，T不动
    delete[] next;
    return i - j;
}

int *buildNext(char *P) //构造next表
{
    int m = (int)strlen(P), j = 0; //“主”串指针
    int *N = new int[m];           //next表
    int t = N[0] = -1;             //模式串指针
    while (j < m - 1)
    {
        //匹配 or 不匹配
        (t < 0 || P[j] == P[t]) ? N[++j] = ++t : t = N[t];
    }
    return N;
}