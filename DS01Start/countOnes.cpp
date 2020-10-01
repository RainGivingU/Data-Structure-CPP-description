int countOnes(unsigned int n)
{
    int ones = 0; // 负责计数
    while (n > 0)
    {
        ones += (1 & n); //最低位若为1则计数
        n >>= 1;         //右移一位
    }
}