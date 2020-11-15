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

![image-20201113112605969](https://i.loli.net/2020/11/13/OFPbzyvKWdmjUHn.png)![image-20201114182942551](https://i.loli.net/2020/11/14/MTjB4cKsfqk8YQS.png)

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

## 7.3 平衡二叉搜索树(Balanced BST)

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

节点数目固定时，兄弟子树的高度越接近**平衡**，全树也将倾向于**更低**

由n个节点组成的二叉树，高度不致低于log<sub>2</sub>n，达到这一下界时，被称为**理想平衡**(如完全二叉树CBT)

但是出现理想平衡的概率很低，维护成本很高，不具有现实意义，我们需要适当放松标准

![image-20201114153245010](https://i.loli.net/2020/11/14/5mRIWj6aUYCBuZQ.png)

高度**渐进地**不超过**O(logn)**，称作**适度平衡**

适度平衡的BST称作**平衡二叉搜索树(BBST)**

### 7.3.3 等价变换

#### 等价BST

**上下可变**（联接关系，继承关系可能发生变化）

**左右不乱**（中序遍历序列完全一致）

![image-20201114154651762](https://i.loli.net/2020/11/14/HyvaD6FNw9zfxUs.png)

#### 限制条件-局部性

后面将要介绍的各种BBST都可视为BST的某一子集，相应地满足精心设计的**限制条件**

- 单次动态修改操作后，至多**O(logn)**处局部不再满足限制条件
- 可在**O(logn)**时间内，使这些局部（以至全树）重新满足

#### 等价变换+旋转调整

刚刚失衡的BST必可在**O(logn)**甚至**O(1)**次旋转后转换为一棵等价BBST（实际上，经过不超过**O(n)**次旋转，等价BST均可相互转化）

![image-20201114155414209](https://i.loli.net/2020/11/14/wWaMH6EuXoAJq4Z.png)

zig和zag仅涉及**常数**个节点，仅需调整其间的联接关系，均属于**局部的基本操作**

调整之后的v/c深度加/减1,子（全）树高度的变化幅度上下差异**不超过1**

## 7.4 AVL树(a kind of BBST)

![image-20201114174839026](https://i.loli.net/2020/11/14/PfAqwCKQUSnDZE1.png)

我们下面主要有两项任务要完成：

①如何界定满足什么条件下的BST为BBST（前提是BBST的高度在**O(logn)**以下）

②某次操作导致BBST失衡，怎样进行重平衡且复杂度在**O(logn)**以下

##### 什么是AVL树？-平衡因子

Balance Factor：`balFac(v) = height(lChild(v)) - height(rChild(v))`

G.**A**delson-**V**elsky & E.**L**andis(1962)提出AVL树，定义：任一AVL树v满足**|balFac(v)|≤1**

![image-20201114180026870](https://i.loli.net/2020/11/14/FwuRLn3B97pMkNH.png)

### 7.4.1 适度平衡

AVL是一种达到了**适度平衡（h < O(logn）**的BBST，下面是证明过程

![image-20201114181509496](https://i.loli.net/2020/11/14/cua5bdsM73hKIGw.png)

![image-20201114181533019](C:%5CUsers%5CspringmorningQ%5CAppData%5CRoaming%5CTypora%5Ctypora-user-images%5Cimage-20201114181533019.png)

Note：注意这里**S(h) = fib(h - 3)**指的高度不包含根，即只有一个节点的树的高度记为0，空树高度记为-1

### 7.4.2 重平衡

#### 接口

```cpp
#include "BST.h"
#define Balanced(x) \
    (stature((x).lChild) == stature((x).rChild)) //理想平衡
#define BalFac(x) \
    (stature((x).lChild) - stature((x).rChild)) //平衡因子
#define AvlBalanced(x) \
    ((-2 < BalFax(x)) && (BalFac(x) < 2)) //AVL平衡条件

template <typename T>
class AVL : public BST<T> //BST派生
{
public:
    //search()等接口直接沿用
    BinNodePosi(T) insert(const T &); //重写插入
    bool remove(const T &);           //重写删除
};
```

#### 失衡

按照上面的接口对AVL树进行动态操作时，AVL的平衡性有可能被破坏！

因节点`x`的插入或删除而暂时失衡的节点构成失衡节点集，记作**UT(x)**

![image-20201114191207491](https://i.loli.net/2020/11/15/p8Cwl9aOsLWnSbX.png)

对于`insert()`，我们发现很有可能会导致插入节点的一系列祖先失衡，UT(x)包含多个节点

而对于`remove()`，仅可能会导致某个祖先发生失衡看，UT(x)仅含单个节点（因为删除的节点如果会使它的父亲失衡，它必然存在于它父亲的较矮的子树中，删除它对它父亲的高度没有影响，其他祖先同理）

对于除动态操作对象的祖先之外的节点，均不会受到影响，不会失衡

下面将介绍这些动态操作后，如何使之重平衡

### 7.4.3 插入

#### 失衡节点集

思考一下，我们发现引入新节点`x`后，**UT(x)中的节点都是`x`的祖先，且高度不会低于`x`的祖父**

我们将UT(x)中的最深者记为`g(x)`，在`x`与`g(x)`的通路上，设`p`为`g(x)`的孩子，`v`是`p`的孩子（如果`g(x)`恰是`x`的祖父，那么`v`就恰是`x`，否则`v`是`x`的祖先；`p`必是`x`的真祖先；切记，`p`, `v`, `x`都是未失衡的节点）

#### 重平衡

1. 首先要找到如上定义的`g(x)`，方法是由`x`出发沿着`parent`指针上行并核对平衡因子，首个失衡节点就是`g(x)`，原树是BBST，那么这一过程用时**O(logn)**
2. 然后再找到`p`与`v`，我们注意到，既然是引入了`x`导致的失衡，那么`p`和`v`的高度必不会低于其各自的兄弟，所以反过来由`g(x)`可以找到`p`与`v`

```cpp
#define tallerChild(x)( \
    stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild :(   /*左高*/ \
    stature((x)->lChild) < stature((x)->rChild) ? (x)->rChild :(   /*右高*/ \
    IsLChild(*(x)) ? (x)->lChild : (x)->rChild          /*等高：与父亲x同侧者优先*/ \
    )\
    )\
)
```

下面，根据节点`g(x)`, `p`, `v`之间的具体联接方向，将采用不同的局部调整方案

#### 单旋

插入的`x`为黄色方块中的任一个，若`v`是`p`的右孩子，`p`是`g`的右孩子，我们只需做一次旋转（单旋）即可

示例中进行的zag(逆时针)旋转可以由以下步骤完成

1. 定义一个`p`的引用`rc`
2. `p`的左子树`T1`成为`g`的右子树
3. `g`成为`p`的左孩子
4. 这个局部子树的根由`g`变为`p`

![image-20201115103156703](https://i.loli.net/2020/11/15/BaeVdCQzfg7DmEP.png)

经过上面的**O(1)**时间的操作，整棵树已恢复为BBST（局部高度恢复到了插入前的状态，`g`上方的失衡祖先必然到达平衡）

#### 双旋

若`v`是`p`的左孩子，`g`是`p`的右孩子，我们需要做两次方向相反的旋转（双旋）

![image-20201115104332060](https://i.loli.net/2020/11/15/WO8t9pZXTSwzrqP.png)

#### 实现

```cpp
template <typename T>
BinNodePosi(T) AVL<T>::insert(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if (x)        //若要插入的节点已经存在
        return x; //直接返回已存在的节点
    BinNodePosi(T) xx = x = new BinNode<T>(e, _hot);
    _size++; //创建新节点
    //此时，若x的父亲_hot增高，则其祖父有可能失衡
    for (BinNodePosi(T) g = _hot; g; g = g->parent) //从x之父向上检查各代祖先g
    {
        if (!AvlBalanced(*g)) //一旦发现g失衡，采用3+4算法使之复衡
        {
            FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g))); //重新接入原树
            break;                                                    //g复衡后局部子树高度复原，则整棵树到达平衡
        }
        else                 //g依然平衡
            updateHeight(g); //更新其高度
    }
    return xx; //返回新节点位置
} //无论e是否存在于原树，AVL::insert(e)->data == e
```

#### 复杂度分析

插入`x`，在原BBST中查找`g(x)`，用时**O(logn)**，至多进行两次旋转即可使该树恢复平衡，总体复杂度为**O(logn)**

### 7.4.4 删除

#### 失衡节点集

与插入操作不同，摘除节点`x`后，以及在随后的调整过程中，失衡节点集UT(x)始终至多只含一个节点（可能就是`x`的父亲），该节点`g(x)`若存在，其高度必与失衡前相同（只有删除了`g(x)`较矮子树中的`x`才会使`g(x)`失衡，而这不影响`g(x)`的高度）

#### 重平衡

从`_hot`节点出发沿`parent`指针上行，经过**O(logn)**的时间即可确定`g(x)`的位置，`g(x)`含`x`的子树一定比不含`x`的子树高度小1（这样在删除`x`后才会失衡，高度差变成2了）；

我们设定`g(x)`在不含`x`的那一侧的孩子为`p`（`p`的高度至少为1，即`p`必有孩子）；

若`p`的两个孩子不等高，选高的的孩子做`v`，若等高则选和`p`同向的（均为左孩子或右孩子）

同样，根据节点`g(x)`, `p`, `v`之间的具体联接方向，将采用不同的局部调整方案

#### 单旋

图解：删除的节点位于T3底部，图1中给出的是已删除已失衡的状态，红色节点可能存在可能不存在，两个黄色节点至少存在其中之一

![image-20201115144348542](https://i.loli.net/2020/11/15/QB6dJPslV5n7ZCE.png)

若红色节点不存在，我们可以发现这个**局部子树的高度减少了1**（而非插入中的复原），这可能会导致更高祖先发生失衡，这种失衡可能会随着一次一次的调整向上传递，导致最多可能要做**O(logn)**次调整！

#### 双旋

![image-20201115144627611](https://i.loli.net/2020/11/15/J9Y1VaHPftmBRvF.png)

#### 实现

```cpp
template <typename T>
bool AVL<T>::remove(const T &e)
{
    BinNodePosi(T) &x = search(e);
    if(!x)  //目标不存在
        return false;   //返回删除失败
    removeAt(x, _hot);  //物理删除x
    _size--;
    //从_hot出发逐层向上，依次检查各代祖先g
    for (BinNodePosi(T) g = _hot; g; g = g->parent)
    {
        if(!AvlBalanced(*g))    //一旦发现g失衡，就通过调整恢复平衡
            g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
        updateHeight(g);    //更新高度
    }   //可能需要O(logn)次调整，全树高度有可能下降
    return true;    //删除成功
}
```

#### 复杂度分析

需要平衡操作的节点仍然不过是`x`的祖先，累计仍然**O(logn)**的时间

### 7.4.5 “3+4”重构

#### 算法

设`g`为最低的失衡节点，沿最长分支考察祖孙三代：g ~ p ~ v

按中序遍历次序，重命名为：a < b < c

它们总共拥有4棵子树（可为空）

按中序遍历次序，重命名为：T<sub>0</sub> < T<sub>1</sub> < T<sub>2</sub> < T<sub>3</sub>

（我们根据不同的情况，为它们进行重命名，这个工作由`rotateAt()`函数完成，我们在后面会有介绍）

将原先以`g`为根的子树S，替换为一棵新子树S'

等价变换保持中序遍历次序：**T<sub>0</sub>  < a < T<sub>1</sub> < b < T<sub>2</sub> <  c < T<sub>3</sub>**

![image-20201115152151957](https://i.loli.net/2020/11/15/z4GOld9BoLah7kq.png)

#### 实现

```cpp
template <typename T>
BinNodePosi(T) BST<T>::connect34(
    BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
    BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3)
{
    a->lChild = T0;
    if(T0)
        T0->parent = a;
    a->rChild = T1;
    if(T1)
        T1->parent = a;
    c->lChild = T2;
    if(T2)
        T2->parent = c;
    c->rChild = T3;
    if(T3)
        T3->parent = c;
    b->lChild = a;
    a->parent = b;
    b->rChild = c;
    c->parent = b;
    updateHeight(a);
    updateHeight(c);
    updateHeight(b);
    return b;
}
```

#### 统一调整

zig（顺时针旋转）与zag（逆时针旋转）

![image-20201115164524376](https://i.loli.net/2020/11/15/QalfVtmHCO6FbuP.png)

zag-zig与zig-zag（双旋）

![image-20201115164545382](https://i.loli.net/2020/11/15/cCLOdyEezanwJlA.png)

```cpp
template <typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)
{
    BinNodePosi(T) p = v->parent, g = p->parent;
    if (IsLChild(*p))     //zig
        if (IsLChild(*v)) //zig-zig
        {
            p->parent = g->parent; //向上联接
            return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
        }
        else //zag-zig(先zag再zig)注意，双旋时v成为该局部的根(即b)
        {
            v->parent = g->parent;
            return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
        }
    else                  //zag
        if (IsRChild(*p)) //zag-zag
    {
        p->parent = g->parent;
        return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
    }
    else //zig-zag(先zig再zag)
    {
        v->parent = g->parent;
        return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
    }
}
```

### 综合评价

![image-20201115170833928](https://i.loli.net/2020/11/15/Ls5jgPQriSIDVhf.png)

