#include "Graph.h"

template <typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int v, int &clock)
{
    Queue<int> Q;           //初始化一个队列
    status(v) = DISCOVERED; //顶点v的状态转换为刚被发现
    Q.enqueue(v);           //顶点v入队

    while (!Q.empty()) //反复地
    {
        int v = Q.dequeue();                                  //取出队列首顶点v
        dTime(v) = ++clock;                                   //打上时间标签，表示当前的时间进度
        for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)) //考察v的每一个邻居u
        {
            //视情况分别处理
            if (UNDISCOVERED == status(u)) //若u尚未被发现过
            {
                status(u) = DISCOVERED; //发现该顶点
                Q.enqueue(u);           //入队
                status(v, u) = TREE;    //这条边必定可以引入支撑树
                parent(u) = v;          //设置u的父亲为v
            }
            else                      //u已被发现，即已在队列中
                status(v, u) = CROSS; //直接将这类边归为跨边
        }
        status(v) = VISITED; //所有邻居都被遍历，则v的状态转换为visited
    }
}

template <typename Tv, typename Te> //顶点类型，边类型
void Graph<Tv, Te>::bfs(int s)      //s为起始顶点
{
    //初始化
    reset();
    int clock = 0;
    int v = s;
    do //逐一检查所有顶点
    {
        if (UNDISCOVERED == status(v)) //一旦遇到尚未发现的顶点
            BFS(v, clock);             //从该顶点出发启动一次BFS
    } while (s != (v = (++v % n)));    //按序号访问，不重不漏
    //bfs自身累计仅需线性时间
}