void bubbleSort(int A[], int n)
{
    bool sorted = false; //整体排序标志，初始化为未排序
    while (!sorted)      // 无法确定完成排序，则逐趟扫描
    {
        sorted = true;
        for (int i = 1; i < n; i++)
        {
            if (A[i - 1] > A[i]) // 如果有两个相邻的数构成逆序
            {
                swap(A[i - 1], A[i]); //交换
                sorted = false;       // 无法确定完成排序
            }
        }
        n--; // 每扫描一趟必有扫描范围内的最大值来到序列尾，扫描范围减1
    }
}