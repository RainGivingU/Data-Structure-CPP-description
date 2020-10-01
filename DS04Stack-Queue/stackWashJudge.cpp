#include "Stack.h"
using namespace ::std;
template <class T>
bool stackWashJudge(Stack<T> A, Stack<T> B)
{
    Stack<T> S, R;
    //将栈B倒置为R，使R由栈顶到栈底的顺序等同于S->B的出栈顺序
    while (!B.empty())
    {
        R.push(B.pop());
    }
    while (!A.empty()) //A非空
    {
        S.push(A.pop());        //将A栈顶删除并push到S
        if (S.top() == R.top()) //S和R的栈顶相同
        {
            //相当于在真正栈混洗过程中，S的栈顶被push到B中
            S.pop();
            R.pop();
            //再判断在真正栈混洗过程中，S的栈顶(在A的栈顶push入S之前)是否连续push入B中
            while (!S.empty() && S.top() == R.top())
            {
                S.pop();
                R.pop();
            }
        }
    }
    return S.empty(); //S为空则代表B是A的合法栈混洗
}
