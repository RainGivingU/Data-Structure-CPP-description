int sum(int A[], int lo, int hi) //区间范围是[lo,hi)
{
    if (hi - lo < 2)
        return A[lo];
    int mi = (lo + hi) >> 1;                //以屁中单元为界，将原匙间一分为二
    return sum(A, lo, mi) + sum(A, mi, hi); //递归对各子数组求和，然后合计
}