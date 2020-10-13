#include <iostream>
#include "../DS04Stack-Queue/Stack.h"
#include "../DS02Vector/vector.h"
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
} Etype;                            //边状态(在遍历树中所属的类型)
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

//Vertex
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
                          data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
                          dTime(-1), fTime(-1), parent(-1), priority(INT_MAX)
    {
    }
};

//Edge
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

    //静态操作接口
    Tv &vertex(int i) { return V[i].data; }         //数据
    int inDegree(int i) { return V[i].inDegree; }   //入度
    int outDegree(int i) { return V[i].outDegree; } //出度
    VStatus &status(int i) { return V[i].status; }  //状态
    int &dTime(int i) { return V[i].dTime; }        //时间标签dTime
    int &fTime(int i) { return V[i].fTime; }        //时间标签fTime
    int &parent(int i) { return V[i].parent; }      //在遍历树中的父亲
    int &priority(int i) { return V[i].priority; }  //优先级数
};
