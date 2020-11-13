# Chapter7 Binary Search Tree(BST)

> 为学日益，为道日损，损之又损，以至于无为，无为而无不为

 ## 7.1 概述

![image-20201113102614531](https://i.loli.net/2020/11/13/JGo4qcyvUupxIB5.png)

我们发现，若要对象集合的组成可以高效率动态调整又要能够高效率查找，线性结构很难胜任，前情预报：AVL树可以实现在最坏情况下，单次动态修改和静态操作均在**O(logn)**时间内完成！

### 7.1.1 循关键码访问(Call by Key)-BST访问特性

如何兼顾静态查找和动态修改？我们首先令**数据项之间依照所持的关键码key彼此区分**而且**关键码之间必须同时支持比较大小和判等**，数据集合中的数据项都统一表示和实现为词条（entry）的形式

```cpp
template <typename K, typename V>
struct Entry
{
    K key;
    V value;
    Entry(K k = K(), V v = V()) : key(k), value(v){};           //默认构造函数
    Entry(Entry<K, V> const &e) : key(e.key), value(e.value){}; //克隆
    //比较器与判等器
    //实际上，比较和判等的就是词条的key;以后我们不再严格区分词条和它的key
    bool operator<(Entry<K, V> const &e) { return key < e.key; }
    bool operator>(Entry<K, V> const &e) { return key > e.key; }
    bool operator==(Entry<K, V> const &e) { return key == e.key; }
    bool operator!=(Entry<K, V> const &e) { return key != e.key; }
};

```

### 7.1.2 中序-BST特性

在BST中，不致混淆的情况下，**节点 ~ 词条 ~ 关键码**

后面均假定BST中不存在相同的节点！

#### 顺序性

BST和二叉树的不同在于，BST中的任一节点

- **不小于**其**左子树**中的节点
- **不大于**其**右子树**中的节点

![image-20201113112204818](https://i.loli.net/2020/11/13/MVlGsu93ItJeNSm.png)

非BST实例（注意红色部分）：

![image-20201113112259320](https://i.loli.net/2020/11/13/zdjmZOYasqk1ylB.png)

BST实例：

![image-20201113112334574](https://i.loli.net/2020/11/13/Xj8JHUnlaQdMhWk.png)![image-20201113112605969](https://i.loli.net/2020/11/13/OFPbzyvKWdmjUHn.png)

#### 单调性

**BST的中序遍历序列必然是单调非降的**

![image-20201113112820176](https://i.loli.net/2020/11/13/Ttmsg8p5GudQ69M.png)

### 7.1.3 接口

```cpp
#include "../DS05BinaryTree/BinTree.h"
template <typename T>
class BST : public BinTree<T> //由BinTree派生
{
public:
    //使用virtual修饰，以便派生类(BST的变种)重写
    virtual BinNodePosi(T) & search(const T &); //查找
    virtual BinNodePosi(T) insert(const T &);   //插入
    virtual bool remove(const T &);             //删除
protected:
    BinNodePosi(T) _hot; //命中节点的父亲
    BinNodePosi(T) connect34(
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
    BinNodePosi(T) rotateAt(BinNodePosi(T)); //旋转调整
};
```

对于以上各个接口，我们将在下面进行详细介绍和实现。

## 7.2 二叉搜索树

### 7.2.1 查找

#### 减而治之

我们的查找过程可如下图所示：

例如`search(28)` > 与根节点20比较（28大于20） > 与20的右孩子38比较（28小于38） > 与38的左孩子25比较（28大于25） > 与25的右孩子29比较（28小于29）>  与29的左孩子比较（29没有左孩子，树中没有28）

![image-20201113164543164](https://i.loli.net/2020/11/13/U7rdGvEj6lOYeTn.png)

而我们如果把BST中序遍历的单调序列（上图树的下方）看作一个有序向量，那么就会发现这种查找策略等价于**向量的二分查找**

#### 实现

```cpp
template <typename T>
BinNodePosi(T) & BST<T>::search(const T &e)
{
    return searchIn(_root, e, _hot = NULL); //从根节点启动查找
}

template <typename T>
static BinNodePosi(T) & searchIn(
	BinNodePosi(T) & v,    //当前(子)树根
    const T &e,            //目标关键码
    BinNodePosi(T) & _hot) //记忆热点
{
    if (!v || e == v->data) //尾递归，确认找不到 or 已找到
        return v;
    _hot = v; //记下当前(一定是非空)节点
    return searchIn(((e < v->data) ? v->lChild : v->rChild), e, _hot);
}
```

#### 语义解释

`seach(e)`返回类型是`BinNodePosi(T)`的引用，为了语义的统一，我们不妨将失败时的空节点转换为一个数值为**e**的哨兵节点（该树将依然是一棵合法BST），这样无论查找成功与否，返回值总是**等效地**指向**命中节点**

我们注意到，`_hot`在`seachIn()`总是作为引用进行传递，相当于直接修改内部变量`_hot`，上面我们将`seach()`的语义统一后，`_hot`便总是**指向命中节点的父亲**

![image-20201113174329827](https://i.loli.net/2020/11/13/m8GikzIJn7dMRx6.png)

#### 复杂度分析

运行时间正比于返回节点`v`的深度，不超过树高**O(h)**

### 7.2.2 插入

由于我们约定了BST中不存在相同节点，那么要插入的节点**e**必然不存在，先调用`search(e)`找到新节点应当插入的位置，再将新节点作为**叶子**插入即可

**`_hot`为新节点的父亲**

**`v = search(e)`为新节点的引用**

**只需令`_hot`通过`v`指向新节点即可**

![image-20201113175642246](https://i.loli.net/2020/11/13/eyObNBrELfdc41Q.png)![image-20201113175705627](https://i.loli.net/2020/11/13/INU7s12etlxYVAg.png)

#### 实现

```cpp
template <typename T>
BinNodePosi(T) BST<T>::insert(const T &e)
{
    BinNodePosi(T) &x = search(e); //查找目标(注意_hot的初值设置)
    if (!x)                        //查找失败时才执行插入操作
    {
        x = new BinNode<T>(e, _hot); //在x处创建新节点，也_hot为父亲
        _size++;                     //更新全树规模
        updateHeightAbove(x);        //更新x即其历代祖先高度
    }
    return x; //无论e是否存在于原树中，此时总有x->data == e
} //边界情况可以顺利处理
```

#### 复杂度分析

主要来源于`search(e)`和`updateHeightAbove(x)`

均线性正比于返回节点`x`的深度，不超过树高**O(h)**

### 7.2.3 删除

#### 主体框架

```cpp
template <typename T>
bool BST<T>::remove(const T &e)
{
    BinNodePosi(T) &x = search(e); //定位目标节点
    if (!x)                        //确认目标存在，若不存在
        return false;              //直接返回
    removeAt(x, _hot);             //分两大类情况实施删除
    _size--;                       //更新全树规模
    updateHeightAbove(_hot);       //更新_hot及其历代祖先的高度
    return true;                   //返回值指示删除成功与否
}
```

#### case1：单分支

如果我们要删除的节点只有左孩子（左子树）或者只有右孩子（右子树），那么我们只需用这个节点的独生子取代这个节点即可；新的BST仍然合法

```cpp
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) & x, BinNodePosi(T) & hot)
{
    BinNodePosi(T) w = x;       //实际被摘除的节点，初值同x
    BinNodePosi(T) succ = NULL; //实际被删除节点的接替者
    if (!HasLChild(*x))         //左子树为空(左右子树都为空的情况也在此处理了)
        succ = x = x->rChild;   //接替者为右子树
    else if (!HasRChild(*x))    //右子树为空
        succ = x = x->lChild;   //接替者为左子树
    else
    {
        //左右子树并存的情况（见双分支）
    }
    hot = w->parent; //记录实际被删除节点的父亲
    if (succ)
        succ->parent = hot; //被删除节点的接替者与hot相联
    release(w->data);
    release(w);  //释放被摘除节点
    return succ; //返回接替者
}
```

#### case2：双分支

##### 回顾`BinNode.h`中的`succ()`

简而言之，寻找一个节点`e`的直接后继的方法是`e->right->left->left->left->left->...`直到某个节点为空，那么这个节点的父亲就是`e`的直接后继（也可看作，**`e`的右子树中左侧分支的最后一个节点**）

我们需要知道，`e->succ()`就是`e`的直接后继，即该BST中序遍历序列中`e`的下一个

##### 思路

`*x(36)`左右孩子并存，我们先找到`x`的直接后继`w`（必无左孩子）；交换 `*x(36)`与 `*w(40)`（刚交换后的树不满足BST的条件，但是随着后续删除的完成，这棵树终将变成合法BST）

问题转化为删除`w`，按case1处理即可

![image-20201113192942761](https://i.loli.net/2020/11/13/EhsI5V7J1FxgLnr.png)

##### 实现

```cpp
else
    {
        //左右子树并存的情况
        w = w->succ();
        swap(x->data, w->data);       //*x与其后继*w交换数据
        BinNodePosi(T) u = w->parent; //问题转化为摘除单分支的节点w
        (u == x ? u->rChild : u->lChild) = succ = w->rChild;
    }
```

我们确认要删除`w`后，将`w`的右孩子设置为继任者`succ == w->rChild`；

解释一下第7行：寻找直接后继的过程是先右拐再一直左拐直到无路可走，所以如果第一次右拐就找到了直接后继（那么x和x的直接后继`w`之间就是父子关系，而`u`又是`w`的父亲，所以`u == x`）就说明要删除的节点`w`是`u`右孩子，自然将`w`的继任者的父亲设置为`u`的右孩子；否则删除的节点就必是`u`的左孩子，它的继任者设置为`u`的左孩子

#### 复杂度分析

主要来源有`succ()`和`search()`，它们的时间消耗都正比于`x`的高度，总体复杂度为**O(h)**

## 7.3 平衡二叉搜索树

### 7.3.1 期望树高

从我们上面对`search()` `insert()` `remove()`这些接口的实现和分析可以看出，它们的复杂度都与树高线性相关，而在最坏情况下，树高是有可能达到**O(n)**，这样就无法体现BST的优势了！

我们按照两种常用的随机统计口径，就BST的平均性能作分析对比

#### 随机生成

![image-20201113211703971](https://i.loli.net/2020/11/13/Z6HImV3Qq1Tr2pv.png)

#### 随机组成

![image-20201113211721042](https://i.loli.net/2020/11/13/6NmDGVevaz27JCg.png)

#### 相信谁？

前一种中，越低的BST被重复统计的次数越多，有过于乐观的问题，我们认为后一种更加可信，但是在实际情况中，理想随机并不常见，较高的BST很常见！

### 7.3.2 理想与适度

### 7.3.3 等价变换

## 7.4 AVL树

### 7.4.1 适度平衡

### 7.4.2 重平衡

### 7.4.3 插入

### 7.4.4 删除

### 7.4.5 “3+4”重构





