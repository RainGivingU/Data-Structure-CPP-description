#include "Stack.h"
#include <iostream>
using namespace ::std;
bool paren(const char exp[], int lo, int hi)
{
    Stack<char> S;                //使用栈记录已发现但尚未匹配的左括号
    for (int i = lo; i < hi; i++) //逐一检查当前字符
        if (exp[i] == '(')        //遇左括号
            S.push(exp[i]);       //则push左括号
        else if (!S.empty())      //遇右括号且此时栈非空
            S.pop();              //则pop刚刚push的左括号
        else                      //遇右括号且栈以空
            return false;         //则必不匹配，原因是缺少左括号
    return S.empty();             //最终，匹配=>栈空，栈非空则不匹配，原因是缺少右括号
}

//拓展：多种标志的嵌套匹配检查
bool parenPro(const char exp[], int lo, int hi)
{
    Stack<char> S;
    for (int i = lo; i < hi; i++)
    {
        if (exp[i] == '(' || exp[i] == '[')      //如果是左括号
            S.push(exp[i]);                      //则入栈
        else if (exp[i] == ')' || exp[i] == ']') //如果是右括号
            if (S.top() == exp[i])               //则与栈顶对比
                S.pop();                         //相等则pop栈顶
            else
                return false; //不相等则必不匹配
        else                  //其他字符
            break;            //跳过
        return S.empty();
    }
}

//demo
main()
{
    char exp[100];
    cout << "Enter the expression to be judged (within 100 characters):" << endl;
    cin >> exp;
    cout << "lo = ";
    int lo;
    cin >> lo;
    cout << "hi = ";
    int hi;
    cin >> hi;
    cout << paren(exp, lo, hi) << endl;
    cout << parenPro(exp, hi, lo);
}