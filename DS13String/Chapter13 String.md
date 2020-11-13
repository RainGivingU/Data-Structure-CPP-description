# Chapter13 String

## 11.1 串及串匹配简述

### 11.1.1 串

#### 术语

由n个字符构成的串记作 S = "a<sub>0</sub> a<sub>1</sub> ... a<sub>n-1</sub>" ，其中 a<sub>i</sub> ∈ ∑， 0 ≤ i ＜ n

![image-20201027170751154](https://i.loli.net/2020/10/27/HucTBO1gRDbXiAN.png)

∑是所有可用字符的集合，称作**字符表(alphabet)**；n称作S的**长度**，记作|S| = n，一般认为n小于无穷，长度为0的串称作**空串(null string)**

字符串中任一连续的片段称作其**子串(substring)**，

对于任意0 ≤ i ≤ i + k ＜ n，由字符串S中**起始于位置`i`的连续`k`个字符组成的子串**记作：

**S.substr(i, k) = "a<sub>i</sub> a<sub>i+1</sub> ... a<sub>i+k+1</sub>" = S[i, i + k)**

![image-20201027170810623](https://i.loli.net/2020/10/27/cFi4AUThL3zgJvx.png)

**起始于位置0，长度为k的子串称为前缀(prefix)；而终止于位置n - 1，长度为k的子串称为后缀(suffix)，分别记作：**

**prefix(S, k) = S.substr(0, k) = S[0, k)**

![image-20201027170830249](https://i.loli.net/2020/10/27/2n7OvWKj3R8zkJi.png)

**suffix(S, k) = S.substr(n - k, k) = S[n - k, n)**

![image-20201027170844209](https://i.loli.net/2020/10/27/ZNiLQjcsHSO84lY.png)

空串是任何字符串的（平凡）子串，也是任何字符串的（平凡）前缀和（平凡）后缀，则任何字符都有自己的子串及前缀后缀。除字符串本身之外的所有非空子串、前缀和后缀称为真子串、真前缀和真后缀

#### ADT

![image-20201027170944007](https://i.loli.net/2020/10/27/8bogKaSih4QkBeH.png)

![image-20201027171035570](https://i.loli.net/2020/10/27/KOVMlhLkrHSQ2nN.png)

### 11.1.2 串匹配

对基于同一字符表的任何**文本串T(|T| = n)**和**模式串P(|P| = m)** *2 << m << n*:

判定T中是否存在某一子串与P相同；若存在则返回该子串的初始位置

### 11.1.3 串匹配算法性能的评测

随机选取文本串T，并从T中随机取出长度为m的子串作为模式串P

## 11.2 蛮力匹配

#### 构思

 自左向右，以字符为单位，依次移动模式串；直至在某个位置，发现匹配

![image-20201027191741652](https://i.loli.net/2020/10/27/qCMRZneiTrsGgL7.png)

#### 实现1

```cpp
int match_I(char *P, char *T)
{
    size_t n = strlen(T), i = 0;
    size_t m = strlen(P), j = 0;
    while (j < m && i < n) //自左向右逐个比对字符
    {
        if (T[i] == P[j]) //若匹配
        {
            ++i; //则转到下一对字符
            ++j;
        }
        else //否则
        {
            //T回退到上一次和P比对的初始位置的下一位置
            i -= j - 1;
            j = 0; //P复位
        }
    }
    return i - j;
}
```

![image-20201027193540297](https://i.loli.net/2020/10/27/4CTwE6hSblJtWiB.png)

**`i` `j`的更新问题**

经过一次失配(或第一次循环)后，`i`与`j`对齐，此时`j = 0`；每次循环都有`j++`，经过`j`次循环后发现`T[i]`与`P[j]`失配（实际`j`就记录了由上一次失配后的成功比对次数），此时的`i `向后走了`j`步，即原位置为`i - j`，根据算法设计此时`P[i]`应该和原位置的下一位对齐，即`T[i - j + 1]`

**循环结束条件与返回值**

不满足`j < m`：对应于模式串P得到了整体的匹配（ 实际`j`就记录了由上一次失配后的成功比对次数），此时返回的`i - j`就对应T中这个子串的开始位置

不满足`i < n`：此时i必然恰好为`n`，而`j`尚处于合法区间`j < m `，那么返回的`i - j > n - m` (我们知道，当P恰好与T末尾的长度为m的子串匹配时，`i - j = n - m`为最大值)，据此判断没有找到匹配的子串

#### 实现2

```cpp
int match_II(char *P, char *T)
{
    size_t n = strlen(T), i = 0;
    size_t m = strlen(P), j;
    for (i = 0; i < n - m + 1; i++) //T[i]与P[0]对齐
    {
        for (j = 0; j < m; j++) //逐次比对
            if (T[i + j] != P[j])   //失配，转向下一对齐位置
                break;
        if (m <= j) //完全匹配
            break;
    }
    return i;
}
```

![image-20201027202503129](https://i.loli.net/2020/10/27/KkAd3cFxXW7OeC8.png)

#### 复杂度分析

最好情况：只经过一轮比对即确认匹配 **O(m)**

---

最坏情况：每轮都比对至P的末字符直到T末尾

![image-20201027203612433](https://i.loli.net/2020/10/27/sDuLfxmyFqtQkg5.png)

每轮循环：成功比对`m - 1`次，失配`1`次 = *O(m)*

循环次数：`n - m + 1`

因为 `n << m`，`m * (n - m + 1) = ` **O(n * m)**

我们发现，字符表∑越小，最坏情况出现的概率越高；而模式串长度`m`越大，最坏情况的后果就更加严重，蛮力算法有时在一趟遍历的最后几步才能发现之前的遍历都是无效的，**大量的重复比对**往往导致了它的低效

![image-20201027204320938](https://i.loli.net/2020/10/27/BXoq25N4hV61ncs.png)

## 11.3 KMP算法

> 知易者不占，善易者不卜。

![image-20201028194453944](https://i.loli.net/2020/10/28/vAUer2ik8uLqwW1.png)

### 11.3.1 从记忆力到预知力

#### 不变性

![image-20201028200705166](https://i.loli.net/2020/10/28/mestRqGTUrbCBHh.png)

`T[i]`和`P[j]`发生失配前的任一时刻，都有`T[i - j, i) == P[0, j)`

#### 记忆力

也即此时我们已经掌握了**`T[i - j ,i)`的所有信息**，那么在下一轮比对中，`T[i - j, i)`可以径直接受而不必再次接受比对

#### 预知力

![image-20201028202437816](C:%5CUsers%5CspringmorningQ%5CAppData%5CRoaming%5CTypora%5Ctypora-user-images%5Cimage-20201028202437816.png)

一个子串值得进行比对的必要条件是**该子串的首字符和模式串中的首字符相等**，`E`和`G`显然可以直接跳过而不用比对，所以我们看到上图的例子中模式串向后滑动了三个字符

### 11.3.2 next表（查询表）

> 好记性不如烂笔头

#### 主算法

和蛮力算法有两处不同

- `if`的条件增加了并列的`j < 0`
- 失配时只需`j`根据查询表做出变化，`i`不变

![image-20201106132004032](https://i.loli.net/2020/11/06/VIQ4alMcyzS5FDf.png)

```cpp
int match(char *P, char *T)
{
    int *next = buildNext(P);      //构造next表
    int n = (int)strlen(T), i = 0; //文本串指针
    int m = (int)strlen(P), j = 0; //模式串指针
    while (j < m && i < n)
        if (j < 0 || T[i] == P[j]) //若匹配
        {
            //携手共进
            ++i;
            ++j;
        }
        else             //若失配
            j = next[j]; //P查表右移，T不动
    delete[] next;
    return i - j;
}
```

### 11.3.3 理解next[]表

**`P[j]`对应的`t = next[j]`语义是`P[j]`前面的部分（即`P[0, j)`）的最大真前缀和真后缀相等的长度为`t`**

1. 发生一次失配`T[i] != P[j]`

   此时P的前缀是`T[0, j)`，P和T比对过（完全相同）的前缀是`P[i - j, i)`

2. 查询`next[]`表，将`P[t]`与`T[i]`对齐开始比对，`P[t]`的前缀是`P[0, t)`，这个前缀对应T中的`T[i - t, i)`，对应1中的`P[j - t, j)`。

   `P[t]`前缀是在不再进行比对的部分，即要保证`P[0, t)` == `T[i - t, i)` == `P[j - t, j)`

   **`P[0, t)` == `P[j - t, j)`即可求出`j`对应的`t`，果然，和T没有关系！**

   我们发现，**`P[0, t)`和`P[j - t, j)`分别是`P[0, j)`的真前缀和真后缀**

3. 相较于1中对齐的`P[j]`，模式串向后滑动了`j - t`步

   `t`越大，位移量越小，在实现过程中往往选择集合`N(P, j)`中最大的`t`以确保跳过比对的字符串确实是不必比对的

![image-20201106134840031](https://i.loli.net/2020/11/06/8X6mzIoSqV2exbE.png)

特别地，若在`j = 0`时就发生了失配，我们令`next[0] = -1`（假想存在`P[-1]`这个哨兵，它和任何字符都匹配），并在`if`语句中将这种情况和成功比对等价，随后`++j; ++i;`相当于使模式串向右移动一个单位开始下一轮比对

### 11.3.4 构造next[]表

#### 递推：已知`next[j]`，如何高效计算`next[j + 1]`

若`next[j] = t`，则在`P[0, j)`中，自匹配的真前缀和真后缀的最大长度为`t`，可得`next[j + 1]≤ next[j] + 1`

特别地，  由于`P[0, next[j])`必然和`P[0, j)`的后缀相等，当`P[j]` = `P[next[j]]`时，匹配的长度加1，`next[j + 1] = next[j] + 1`

![image-20201106150833127](https://i.loli.net/2020/11/06/7YZQsvMTVk4mqzb.png)

那么一般地，若`P[j] != P[next[j]]`

`next[j + 1]`的下一个候选者依次是，`next[next[j]] + 1`，`next[next[next[j]]] + 1`，...

反复使用`next[t]`替换`t`,直至发现`P[j]`与`P[t]`匹配后（别忘了`P[-1]`为通配），令	`next[j + 1] = next[t] + 1`

![image-20201106152641066](https://i.loli.net/2020/11/06/C6BgnM2R8bzQuIl.png)

`t`在递减且有最小值`-1`，故算法具备正确性

### 11.3.5 分摊分析

![image-20201107194147594](https://i.loli.net/2020/11/07/FCbkurTB4x2NQVq.png)

同理，构造next表的复杂度为O(m)，则总体复杂度为**O(m + n)**

### 11.3.6 再改进

