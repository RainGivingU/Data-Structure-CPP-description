# Chapter 6 Graph

## 6.1 概念小结

> 黄药师告罪一番，向曲风荷池走去，约莫一盏茶的时间，见他捧着一卷卷轴而来对欧阳克道：“这是桃花岛的总图，岛上所有五行生克、阴阳八卦的变化，全记在内，你拿去好好研习罢。”欧阳克好生失望，原盼在桃花岛多住一时，哪知他却拿出一张图来，所谋眼见是难成的了，也只得躬身去接。黄药师忽道：“且慢！”欧阳克一怔，双手缩了回去。黄药师道：“你拿了这图，到临安府找一家客店或是寺观住下，三月之后，我派人前来取回。图中一切，只许心记，不得另行抄录印摹。”

### 基本术语

**G = (V, E)**，其中集合V中的元素称为**顶点(vertex)**；集合E中的元素分别对应某一对顶点(u, v)，表示它们之间存在某种关系，称作**边(edge)**，我们通常将其规模分别记为**n = |V|**，**e = |E|**

同一条边的**两个顶点**，彼此**邻接(adjacency)**；同一顶点自我邻接构成**自环(self-loop)**；不含自环的为**简单图(simple graph)**，*我们这里只讨论简单图*

**顶点与其所属边**，彼此**关联(incidence)**，与某一顶点关联的边数称为**度(degree)**

若邻接顶点u和v的次序无所谓，则(u, v)为**无向边(undirected edge)**；所有边均为无方向的图为**无向图(undigraph)**，反之则所有边均是**有向边(directed edge)**的图为**有向图**，**(u, v)中v是头(head)，u是尾(tail)**；无向边有向边并存的图称为混合图，*本章主要针对**有向图**介绍相关结构与算法*

和树一样，图中也有**路径pi**，即由一顶点经过多个互相有邻接关系的顶点到达另一顶点pi = <v<sub>0</sub>，v<sub>1</sub>，...，v<sub>k</sub>>，长度|pi| = k

若路径上没有经过重复的顶点，则该路径为**简单路径**

![image-20201012193111274](https://i.loli.net/2020/10/12/LEKIzxsmZklBOT5.png)

若v<sub>0</sub> = v<sub>k</sub>，则该路径是一个**环路**

**有向无环图(DAG)**

欧拉环路：|pi| = |E|，各边恰好出现一次 

![image-20201012193519828](https://i.loli.net/2020/10/12/su9bMAocRkYFx7P.png)

哈密尔顿环路：|pi| = |V|，各顶点恰好出现一次

![image-20201012193145116](https://i.loli.net/2020/10/12/W5EqGhHazIYDA7t.png)

### 复杂度

在无向图中，每对顶点至多贡献一条边，包含n个顶点的图中至多n(n - 1)/2条边，这种图即**完全图**；而在有向图中，每对顶点至多可以贡献两条边，因此至多**n(n - 1)**条边，即**e = O(n<sup>2</sup>)**

## 6.2 抽象数据类型

### 6.2.1 操作接口

图ADT支持的边操作接口

| 操作接口       | 功能描述               |
| -------------- | ---------------------- |
| `e()`          | 边总数\|E\|            |
| `exist(v, u)`  | 判断联边(v, u)是否存在 |
| `insert(v, u)` | 引入从顶点v到u的联边   |
| `remove(v, u)` | 删除从顶点v到u的联边   |
| `status(v, u)` | 边(v, u)的状态         |
| `edge(v, u)`   | 边(v, u)对应的数据域   |
| `weight(v, u)` | 边(v, u)的权重         |

图ADT支持的顶点操作接口

| 操作接口                   | 功能描述                 |
| -------------------------- | ------------------------ |
| `n()`                      | 顶点总数\|V\|            |
| `insert(v)`                | 在顶点集V中插入新顶点v   |
| `remove(v)`                | 将顶点v从顶点集中删除    |
| `inDegree(v) outDegree(v)` | 顶点v的入度、出度        |
| `firstNbr(v)`              | 顶点v的首个邻接顶点      |
| `nextNbr(v, u)`            | 在v的邻接顶点中，u的后继 |
| `status(v)`                | 顶点v的状态              |
| `dTime(v) fTime(v)`        | 顶点v的时间标签          |
| `parent(v)`                | 顶点v在遍历树中的父节点  |
| `priority(v)`              | 顶点v在遍历树中的权重    |

### 6.2.2 Graph模板类

```cpp
typedef enum
{
    UNDISCOVERED,
    DISCOVERED,
    VISITED
} VStatus; //顶点状态
typedef enum
{
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
} EStatus;                          //边状态
template <typename Tv, typename Te> //顶点类型、边类型
class Graph
{ //图Graph模板类
private:
    void reset()
    { //所有顶点、边的辅助信息复位
        for (int i = 0; i < n; i++)
        { //所有顶点的
            status(i) = UNDISCOVERED;
            Time(i) = fTime(i) = -1; //状态，时间标签
            parent(i) = -1;
            priority(i) = INT_MAX;      //（在遍历树中的）父节点，优先级数
            for (int j = 0; j < n; j++) //所有边的
                if (exists(i, j))
                    status(i, j) = UNDETERMINED; //状态
        }
    }
    void BFS(int, int &);                //（连通域）广度优先搜索算法
    void DFS(int, int &);                //（连通域）深度优先搜索算法
    void BCC(int, int &, Stack<int> &);  //（连通域）基亍DFS的双连通分量分解算法
    bool TSort(int, int &, Stack<Tv> *); //（连通域）基亍DFS的拓扑排序算法
    template <typename PU>
    void PFS(int, PU);                                  //（连通域）优先级搜索框架
public:                                                 // 顶点
    int n;                                              //顶点总数
    virtual int insert(Tv const &) = 0;                 //插入顶点，返回编号
    virtual Tv remove(int) = 0;                         //删除顶点及其关联边，返回该顶点信息
    virtual Tv &vertex(int) = 0;                        //顶点v的数据（该顶点的确存在）
    virtual int inDegree(int) = 0;                      //顶点v的入度（该顶点的确存在）
    virtual int outDegree(int) = 0;                     //顶点v的出度（该顶点的确存在）
    virtual int firstNbr(int) = 0;                      //顶点v的首个邻接顶点
    virtual int nextNbr(int, int) = 0;                  //顶点v的（相对于顶点j的）下一邻接顶点
    virtual VStatus &status(int) = 0;                   //顶点v的状态
    virtual int &dTime(int) = 0;                        //顶点v的时间标签dTime
    virtual int &fTime(int) = 0;                        //顶点v的时间标签fTime
    virtual int &parent(int) = 0;                       //顶点v在遍历树中的父亲
    virtual int &priority(int) = 0;                     //顶点v在遍历树中的优先级数
                                                        // 边：这里约定，无向边均统一转化为斱向互逆的一对有向边，从而将无向图视作有向图的特例
    int e;                                              //边总数
    virtual bool exists(int, int) = 0;                  //边(v, u)是否存在
    virtual void insert(Te const &, int, int, int) = 0; //在顶点v和u之间插入权重为w的边e
    virtual Te remove(int, int) = 0;                    //删除顶点v和u之间的边e，返回该边信息
    virtual EStatus &status(int, int) = 0;              //边(v, u)的状态
    virtual Te &edge(int, int) = 0;                     //边(v, u)的数据（该边的确存在）
    virtual int &weight(int, int) = 0;                  //边(v, u)的权重
                                                        // 算法
    void
    bfs(int);              //广度优先搜索算法
    void dfs(int);         //深度优先搜索算法
    void bcc(int);         //基亍DFS的双连通分量分解算法
    Stack<Tv> *tSort(int); //基亍DFS的拓扑排序算法
    void prim(int);        //最小支撑树Prim算法
    void dijkstra(int);    //最短路径Dijkstra算法
    template <typename PU>
    void pfs(int, PU); //优先级搜索框架
};
```

## 6.3 邻接矩阵

