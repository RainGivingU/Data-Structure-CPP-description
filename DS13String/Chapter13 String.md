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

一个子串值得进行比对的必要条件是**该子串的首字符和模式串中的首字符相等**，所以我们看到上图的例子中模式串向后滑动了三个字符

### 11.3.2 next表（查询表）

> 好记性不如烂笔头

### 11.3.3 理解next[]表

### 11.3.4 构造next[]表

### 11.3.5 分摊分析

### 11.3.6 再改进

