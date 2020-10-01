#include <iostream>
#include <string>
using namespace ::std;
string LCS(string A, string B)
{
    int m = A.size() - 1;
    int n = B.size() - 1;
    if (A.empty() || B.empty())
        return string("");
    else if (A[m] == B[n])
    {
        auto sameChar = A[m];
        A.pop_back();
        B.pop_back();
        return LCS(A, B) + sameChar;
    }
    else
    {
        string str1 = LCS(A.substr(0, A.length() - 1), B);
        string str2 = LCS(A, B.substr(0, B.length() - 1));
        return str1.size() > str2.size() ? str1 : str2;
    }
}

string LCS(string, string);

int main()
{
    string A, B;
    cin >> A >> B;
    string s = LCS(A, B);
    cout << s << endl;
}