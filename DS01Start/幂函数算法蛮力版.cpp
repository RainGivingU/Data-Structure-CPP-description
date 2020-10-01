_int64 power2BF(int n)
{
    _int64 pow = 1; //初始化累积器O(1)
    while (n--)     // 循环 O(n) 递减 O(1)
        pow <<= 1;  // ×2 O(1)
    return pow;     // O(1)
}