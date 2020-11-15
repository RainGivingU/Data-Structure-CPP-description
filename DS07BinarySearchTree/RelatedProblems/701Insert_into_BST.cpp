#include <iostream>
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
// You are given the root node of a binary search tree (BST) and a value to insert into the tree.
//Return the root node of the BST after the insertion.
//It is guaranteed that the new value does not exist in the original BST.
// Notice that there may exist multiple valid ways for the insertion, as long as the tree remains a BST after insertion.
//You can return any of them.

//Note:解法-模拟
//使程序完整按照题目所叙述的方式运行进而得到答案
//这道题只需要依据BST的特性寻找适合插入的地方即可，因为要求了不会和原有的点重复
class Solution
{
public:
    TreeNode *insertIntoBST(TreeNode *root, int val)
    {
        if (root == nullptr)
            return new TreeNode(val);
        TreeNode *pos = root;
        while (pos != nullptr)
        {
            if (val < pos->val)
            {
                if (pos->left == nullptr)
                {
                    pos->left = new TreeNode(val);
                    break;
                }
                else
                    pos = pos->left;
            }
            else
            {
                if (pos->right == nullptr)
                {
                    pos->right = new TreeNode(val);
                    break;
                }
                else
                    pos = pos->right;
            }
        }
        return root;
    }
};