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

### 6.3.1 概念

**邻接矩阵(adjacency matrix)**是图ADT最基本的实现方式，使用方阵`A[n][n]`表示由n个顶点构成的图，如果顶点`i`与`j`之间存在邻接关系，那么`A[i][j]`的值为1，反之为0

我们只考察简单图，所以对角线统一设置为0；**空间复杂度为O(n<sup>2</sup>)**，与图中实际边数无关。

![image-20201013195614645](https://i.loli.net/2020/10/13/PcIq8rUZ32vOzFd.png)

> 对应的，还有关联矩阵(incidence matrix)，用二维矩阵记录顶点与边之间的邻接关系，但是它的空间复杂度为O(n*e) = O(n<sup>3</sup>)，在解决某些问题时有效，本章主要考察邻接矩阵

### 6.3.2 实现

#### 顶点

```cpp
//Vertex
typedef enum
{
    UNDISCOVERED,
    DISCOVERED,
    VISITED
} VStatus; //顶点状态
template <typename Tv>
struct Vertex
{
    Tv date;
    int inDegree;
    int outDegree;
    VStatus status;       //三种状态
    int dTime, fTime;     //时间标签
    int parent;           //遍历树中的父节点
    int priority;         //在遍历树中的优先级
    Vertex(Tv const &d) : //构造新顶点
                          data(d), inDegree(0), outDegree(0), 	status(UNDISCOVERED),
                          dTime(-1), fTime(-1), parent(-1), priority(INT_MAX)
    {
    }
};
```

#### 边

```cpp
//Edge
typedef enum
{
    UNDETERMINED,
    TREE,
    CROSS,
    FORWARD,
    BACKWARD
} Etype;    //边状态(在遍历树中的类型)    
template <typename Te>
struct Edge
{
    Te data;                   //数据
    int weight;                //权重
    Etype type;                //在遍历树中所属的类型
    Edge(Te const &d, int w) : //构造新边
                               data(d), weight(w), type(UNDETERMINED)
    {
    }
};
```

#### 邻接矩阵

- 顶点集：一组顶点所构成的向量，向量长度即顶点规模
- 边集：某一顶点关联的所有边构成一个向量（该向量长度为n），由一组这样的向量（有n个顶点即有n个这样的向量）又构成的向量为边集；这样形成的二维矩阵就是邻接矩阵

得益于我们在Vector一章中重载了操作符`[]`，我们可以用`E[i][j]`指代顶点`i`与`j`之间潜在（是否存在未知）的边，并对其进行操作

```cpp
//GraphMatrix邻接矩阵
template <typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te>
{
private:
    Vector<Vector<Tv>> V;          //顶点集
    Vector < Vector<Edge<Te> *> E; //边集
public:
    GraphMatrix() { n = e = 0; } //构造
    ~GraphMatrix()               //析构
    {
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                delete E[j][k]; //清除所有动态申请的边记录
    }
};
```

#### 静态操作接口

```cpp
//顶点的读写
Tv &vertex(int i) { return V[i].data; }         //数据
int inDegree(int i) { return V[i].inDegree; }   //入度
int outDegree(int i) { return V[i].outDegree; } //出度
VStatus &status(int i) { return V[i].status; }  //状态
int &dTime(int i) { return V[i].dTime; }        //时间标签dTime
int &fTime(int i) { return V[i].fTime; }        //时间标签fTime
int &parent(int i) { return V[i].parent; }      //在遍历树中的父亲
int &priority(int i) { return V[i].priority; }  //优先级数
//边的读写
bool exists(int i, int j) //判断边(i, j)是否存在
{
    return ((0 < i || 0 == i) && i < n && (0 < j || 0 == j) && j < n && E[i][j] != NULL);
}
Te &edge(int i, int j) //边的数据,O(1)
{
    return E[i][j]->data;
}
Etype &type(int i, int j) //边的类型,O(1)
{
    return E[i][j]->type;
}
int &weight(int i, int j) //边的权重,O(1)
{
    return E[i][j]->weight;
}
//邻点枚举，对于任意顶点i，自顶点j开始，枚举其所有邻接顶点,O(n)
int firstNbr(int i) { return nextNbr(i, n); }
int nextNbr(int i, int j)
{
    while ((-1 < j) && !exists(i, --j))
        ;     //查找到第一个存在的邻居后停止
    return j; //返回这个邻居顶点
}
```

#### 边的动态操作

边的动态操作不会改变矩阵的规模，实现方法比较简单

```cpp
//边的插入
void insert(Te const &edge, int w, int i, int j)
{
    if (exists(i, j))
        return;                      //忽略已有边
    E[i][j] = new Edge<Te>(edge, w); //创建新边，data为edge，权重为w
    e++;                             //更新边计数
    V[i].outDegree++;                //更新顶点i的出度
    V[j].inDegree++;                 //更新顶点j的入度
}
//边的删除
Te remove(int i, int j) //删除顶点i和j之间的联边(断言此边存在)
{
    Te eBak = edge(i, j); //备份边(i, j)的信息
    delete E[i][j];
    E[i][j] = NULL;   //删除边(i, j)
    e--;              //更新边的计数
    V[i].outDegree--; //更新顶点i的出度
    V[j].inDegree--;  //更新顶点j的入度
    return eBak;      //返回被删除边的信息
}
```

#### 顶点的动态操作

注意矩阵规模的变化

我们假定顶点插入前的矩阵如下

![image-20201017110828914](https://i.loli.net/2020/10/17/7MVxaZpWEOmwP2r.png)

①矩阵的每一行都要增加一个元素，相对于矩阵增加了一列

②矩阵要增加一行，对应增加的顶点的邻接顶点集

③边集E增加一个元素(向量，即②中增加的一行)

④顶点集V要增加一个元素

![image-20201017111306099](https://i.loli.net/2020/10/17/FTVLgDIbz9xH5cr.png)

```cpp
//顶点插入
Te insert(Tv const &vertex) //插入顶点，返回编号
{
     //①
    for (int j = 0; j < n; j++)
    	E[j].insert(NULL);
    n++;

    E.insert(Vector<Edge<Te> *>(n, n, NULL)); //②③
    return V.insert(Vertex<Tv>(vertex));      //④
}
```

顶点的删除实际是逆向进行上面的过程

```cpp
//顶点删除
Tv remove(int i) //删除顶点及其关联边，返回该顶点信息
{
    for (int j = 0; j < n; j++)
        if (exists(i, j)) //删除所有出边，即②
        {
            delete E[i][j];
            V[j].inDegree--;
            e--;
        }
    E.remove(i);
    n--;                        //删除第i行，即③
    Tv vBak = vertex(i);        //备份顶点i
    V.remove(i);                //删除顶点i，即④
    for (int j = 0; j < n; j++) //删除所有入边，即①
        if (Edge<Te> *x = E[j].remove(i))
        {
            delete x;
            V[j].outDegree--;
            e--;
        }
    return vBak; //返回被删除顶点的信息
}
```

### 6.3.3 性能分析

#### 优点

- 直观易理解与实现
- 适用范围广泛
- 很多操作可以在常数时间内完成
- 扩展性，得益于Vector

#### 缺点

- O(n<sup>2</sup>)空间，与实际边数无关，当e << n<sup>2</sup>时有很大的浪费

  > **平面图(planar graph)**指一类可以嵌入平面(但边保证不相交)的图，下面四个栗子中前两个为平面图<img src="https://i.loli.net/2020/10/17/Wq4M9DFPwZjozQJ.png" alt="image-20201017114142961" style="zoom:50%;" />
  >
  > 可以证明，e ≤ 3*n - 6 = O(n) << O(n<sup>2</sup>)，此时的空间利用率约为1/n，这是极差的

## 6.6 广度优先搜索(Breadth-First Search)

### 化繁为简

在二叉树中我们介绍了各种遍历算法将这种半线性结构顺利转化为了线性结构，对于图这种非线性结构，我们通过遍历使其转化为半线性结构，也就是原图中的边和顶点构成一棵**支撑树（森林）**，称作**遍历树**，由于图的遍历更加强调甄别和查找，所以也称作**图搜索**。

### 策略

始自顶点s的广度优先搜索

1. 访问顶点s

2. 依次访问s所有未访问的邻接顶点（图中内圈蓝色点）

3. 依次访问它们未访问的邻接顶点（图中红色点）

4. ...................................................

   直至**没有尚未访问**的邻接节点，结果得到的图是一个无向连通图

![image-20201017165651646](https://i.loli.net/2020/10/17/MI4XnCA7JcN8R5i.png)

容易发现，这实际上就是一棵**支撑树(BFS tree)**，策略也完全等同于树的层次遍历

### 实现

图中每一个顶点都会经历Undiscovered（初始化）`->` Discovered（已入队）`->`Visited（`for`循环结束）的过程

```cpp
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
```

### 实例

![image-20201017194640652](https://i.loli.net/2020/10/17/IehKoJYt7E8uSFA.png)

![image-20201017194702564](https://i.loli.net/2020/10/17/8VHghDd95aG6lAf.png)

### 多连通情况

上述算法的实现前提是图中所有顶点在一个连通域中，否则将无法遍历图中所有顶点，下面解决这个问题

![image-20201017194744600](https://i.loli.net/2020/10/17/nBghyz4QtiHwNAj.png)

多个遍历树构成了一个**遍历森林**

```cpp
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
```

### 复杂度分析

我们主要分析以下算法主体部分：

```cpp
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
}
```

#### `while`循环

`while`循环执行的次数等于`dequeue()`执行的次数

根据每个顶点都会入队且仅入队一次的特点可知，`enqueue()`会执行**O(n)**次，相应地`dequeue()`也会执行**O(n)**次至队列为空，那么`while`循环也就恰好执行**O(n)**次

#### `for`循环

本质上看，`for`循环就是对`v`所在行向量进行扫描，每个行向量规模为**O(n)**，结合`while`循环，for循环执行次数将高达**理论复杂度O(n<sup>2</sup>)；**边存在时才会进入内循环，即内循环总共执行次数为**O(e)**，那么整体复杂度为O(n<sup>2</sup> + e) == **O(n<sup>2</sup>)**，这只具有理论意义

实际情况下，由于`for`循环中对行向量的查找是极其简单的基本操作，且行向量在物理上是连续的可以激活系统高速缓存状态，**这里的O(n)可以直接视为O(1)**，那么**实际上BFS算法的复杂度为O(n + e)**

### 最短路径

![image-20201017202500259](https://i.loli.net/2020/10/17/9o8PVaMWvuEjicf.png)

## 6.7 深度优先搜索(Depth-First Search)

### 策略

**DFS(s)**	//始自顶点s的深度优先搜索

1. 访问顶点s
2. - 若s尚有未被访问的邻居，则任取其一u，递归执行DFS(u)
   - 否则，返回

3. 若此时图中尚有顶点未被访问
   - 任取这样的一个顶点作为起始点
   - 重复2中过程
   - 直到所有点都被访问

2中的返回，即回溯，会再次判断s是否尚有未被访问的邻居；如果任取的邻居2是已访问过的，那么将特殊标记出来而不计在支撑树(DFS Tree)中

![image-20201018174554388](https://i.loli.net/2020/10/18/WmDSdUFi6hn9gMb.png)

### 框架

```cpp
template <typename Tv, typename Te> //顶点类型、边类型
void Graph<Tv, Te>::DFS(int v, int &clock)
{
    dTime(v) = ++clock;
    status(v) = DISCOVERED; //发现当前顶点v
    for (int u = firstNbr(v); - 1 < u; u = nextNbr(v, u))   //考察v的每一邻居u
    {
        //视u的状态分别处理
        switch (status(u)) 
        {
        case UNDISCOVERED:    
            /* ... */
            break;
        case DISCOVERED: 
            /* ... */
            break;
        default:  //即VISITED
            /* ... */
            break;
        }
    }
    status(v) = VISITED;    //至此当前顶点v访问完毕
    fTime(v) = ++clock; 
}
```

### 不同状态，分别处理

#### UNDISCOVERED

u就是那个尚未发现的邻居，将`[v, u]`这条边引入支撑树，然后直接递归执行**DFS(u)**

```cpp
case UNDISCOVERED:     //u尚未发现，即支撑树可以在此拓展
    type(v, u) = TREE; //[v, u]边归为TREE
    parent(u) = v;     //u的父亲设为v
    DFS(u, clock);     //从u开始递归
    break;
```

#### DISCOVERED

u已被发现但尚未访问完毕，那么u应该是v的祖先且`[u, v]`这条边已被引入支撑树，所以将`[v, u]`设置为**回边(backward)**

```cpp
case DISCOVERED:           //u已被发现但尚未访问完毕
	type(v, u) = BACKWARD; //[v, u]应属于被后代指向的祖先，归为BACKWARD
    break;
```

#### VSITED

> dTime和fTime记录了每个顶点的被发现和被访问完成的时刻，区间[dTime(v), fTime(v)]即是v的活跃期，而任意顶点v和u之间是否存在祖先/后代的血缘关系，完全取决于二者的活跃期是否相包含，详见**嵌套引理**

u已经访问完毕（那么fTime(u)必然要小于fTime(v)，即只有可能v是u的祖先，v的活跃期可能包含u的活跃期），我们要通过对比u, v谁更早被发现（即时间标签dTime谁大谁小）来判断`[v, u]`边的类型是

- 若dTime(v) < dTime(u)，那么v必是u的祖先，设置[v, u]边为**前向边(FORWARD)**
- 否则，v和u没有血缘关系，来自不同的独立分支，设置`[v, u]`边为跨边**(CROSS)**

```cpp
default: //status(u) = VISITED
	type(v, u) = dTime(v) < dTime(u) ? FORWARD : CROSS;
	break;
```

### 多可达域情况

效仿BFS中的做法，我们得到以下解决方法

```cpp
template <typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s)
{
    //初始化
    reset();
    int clock = 0;
    int v = s;
    do
    {
        if(UNDISCOVERED == status(v))   //一旦遇到尚未发现的顶点
            DFS(v, clock);  //从该顶点出发启动一次DFS
    } while (s != (v = (++v % n))); //按序号访问，不重不漏
    
}
```

### 实例

#### 无向图例子

![image-20201018192548392](https://i.loli.net/2020/10/18/DtWHfoyuTM4anF3.png)

![image-20201018192603365](https://i.loli.net/2020/10/18/6YmJ2nxr3cMTu8w.png)

![image-20201018192621753](https://i.loli.net/2020/10/18/8XAdeTnuWYEfQiz.png)

![image-20201018192640933](https://i.loli.net/2020/10/18/pjUG4YzDHyNB58R.png)

#### 有向图例子

![image-20201018192959955](https://i.loli.net/2020/10/18/Dg6UEQ9GrzFIWc1.png)

![image-20201018193029209](https://i.loli.net/2020/10/18/UzMWClGnN3baFXt.png)

![image-20201018193048069](https://i.loli.net/2020/10/18/yMoqIBDx6RVTXUC.png)

![image-20201018193102196](https://i.loli.net/2020/10/18/TSNZmaL4FgCOR72.png)

![image-20201018193114703](https://i.loli.net/2020/10/18/PipO1Lb7qaZcBHN.png)

![image-20201018193128440](https://i.loli.net/2020/10/18/zOlfNDE2bwqaSYi.png)

![image-20201018193146010](https://i.loli.net/2020/10/18/kgFObd92NSJcvKR.png)

### 嵌套引理

![image-20201018193247129](https://i.loli.net/2020/10/18/BFQvpal64mKX9YI.png)