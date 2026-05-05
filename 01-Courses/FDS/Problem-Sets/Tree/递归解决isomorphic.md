## 为什么递归？

### 问题的本质

同构的定义是：**允许对任意节点翻转其左右子树**，翻转后两棵树结构相同。

关键：**每一层都独立决定翻不翻转**，不是整棵树统一翻转。

------

### 从根节点往下看

假设 t1 和 t2 根值相同，现在考虑 t1 选择"翻转"：

```
t1:          t2:
    A             A
   / \           / \
  B   C         C   B
```

翻转意味着：t1 的左子树 B 要去匹配 t2 的右子树 B，t1 的右子树 C 去匹配 t2 的左子树 C。

这一层匹配成功了。但 B 和 C 内部还有子树，**它们是否同构是一个独立的子问题**。

------

### 为什么是递归

`isIsomorphic(t1->left, t2->right)` 这个调用在问：

> **"B 和 B 这两棵子树，同构吗？"**

B 和 B 内部可能又需要翻转，也可能不翻转——这由递归的下一层自己决定。当前层不管，也不需要管。

每一层只做一件事：

```
当前节点根值相同？
├── 是 → 试两种配对，子树同构性交给递归
└── 否 → 直接返回 false
```

------

### 一个反例说明"不递归"会错

如果你只在根节点翻转一次，遇到这种情况就会失败：

```
t1:            t2:
    A               A
   / \             / \
  B   B           B   B
 / \             / \
C   D           D   C
```

根节点不需要翻转（左B配左B），但左子树内部需要翻转（C、D互换）。必须递归到每一层独立判断。

------

### 总结

递归的合理性来自**子问题结构相同**：判断两棵子树是否同构，和判断整棵树是否同构是完全相同的问题。所以每一层都调用自身，每一层独立决定翻转与否，`||` 保证两种选择只要有一种成立即可。



```c#
#include <stdbool.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left, *right;
} TreeNode;

bool isIsomorphic(TreeNode *t1, TreeNode *t2) {
    // 两个都为空：同构
    if (!t1 && !t2) return true;
    // 一个为空一个不为空：不同构
    if (!t1 || !t2) return false;
    // 根值不同：不同构
    if (t1->val != t2->val) return false;
    
// 不翻转：左配左 && 右配右
bool no_flip = isIsomorphic(t1->left,  t2->left)
            && isIsomorphic(t1->right, t2->right);

// 翻转：左配右 && 右配左
bool flip    = isIsomorphic(t1->left,  t2->right)
            && isIsomorphic(t1->right, t2->left);

return no_flip || flip;

}
```

==同构：翻转或不翻转==