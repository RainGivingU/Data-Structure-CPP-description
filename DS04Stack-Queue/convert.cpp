#include "Stack.h"
#include <iostream>
using namespace ::std;
void convert(Stack<char> &S, int64_t n, int base)
{
    char digit[] = "0123456789ABCDEF"; //数位符号，如有必要可继续扩充
    while (0 < n)                      //由低到高，逐一计算出新进制下的各数位
    {
        S.push(digit[n % base]); //余数(即当前计算出的数位)入栈
        n /= base;               //更新n
    }
    //新进制下由高到低的各数位，在栈中自顶而下保存
}

//调用
main()
{
    Stack<char> S;
    int64_t n = 9999;
    int base = 16;
    convert(S, n, base);
    while (!S.empty())
        printf("%c", S.pop()); //逆序输出
}