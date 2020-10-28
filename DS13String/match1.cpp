#include <string>
#include <iostream>
using namespace ::std;

int match_I(char *P, char *T)
{
    size_t n = strlen(T), i = 0;
    size_t m = strlen(P), j = 0;
    while (j < m && i < n) //自左向右逐个比对字符
    {
        if (T[i] == P[j]) //若匹配
        {
            ++i; //则转到下一对字符
            ++j;
        }
        else //否则
        {
            //T回退到上一次和P比对的初始位置的下一位置
            i -= j - 1;
            j = 0; //P复位
        }
    }
    return i - j;
}

int match_II(char *P, char *T)
{
    size_t n = strlen(T), i = 0;
    size_t m = strlen(P), j;
    for (i = 0; i < n - m + 1; i++) //T[i]与P[0]对齐
    {
        for (j = 0; j < m; j++)   //逐次比对
            if (T[i + j] != P[j]) //失配，转向下一对齐位置
                break;
        if (m <= j) //完全匹配
            break;
    }
    return i;
}