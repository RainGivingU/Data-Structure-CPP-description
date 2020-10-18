#include "Graph.h"

template <typename Tv, typename Te> //顶点类型、边类型
void Graph<Tv, Te>::DFS(int v, int &clock)
{
    dTime(v) = ++clock;
    status(v) = DISCOVERED;                               //发现当前顶点v
    for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u)) //考察v的每一邻居u
    {
        switch (status(u)) //视其状态不同分别处理
        {
        case UNDISCOVERED:     //u尚未发现，即支撑树可以在此拓展
            type(v, u) = TREE; //[v, u]边归为TREE
            parent(u) = v;     //u的父亲设为v
            DFS(u, clock);     //从u开始递归
            break;
        case DISCOVERED:           //u已被发现但尚未访问完毕
            type(v, u) = BACKWARD; //[v, u]应属于被后代指向的祖先，归为BACKWARD
            break;
        default: //status(u) = VISITED
            type(v, u) = dTime(v) < dTime(u) ? FORWARD : CROSS;
            break;
        }
    }
    status(v) = VISITED; //至此当前顶点v访问完毕
    fTime(v) = ++clock;
}

template <typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s)
{
    //初始化
    reset();
    int clock = 0;
    int v = s;
    do
    {
        if (UNDISCOVERED == status(v)) //一旦遇到尚未发现的顶点
            DFS(v, clock);             //从该顶点出发启动一次DFS
    } while (s != (v = (++v % n)));    //按序号访问，不重不漏
}