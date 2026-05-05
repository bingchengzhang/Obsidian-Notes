# 二叉树与BST知识总结

## 一、基本概念

- **二叉树**：每个节点最多两个子节点（左/右）
- **度**：节点的子节点数（0/1/2）
- **叶节点**：度为0的节点
- **高度/深度**：根到该节点的路径长度

### 重要性质
$$n_0 = n_2 + 1$$
度为0的节点数 = 度为2的节点数 + 1

### 满二叉树 vs 完全二叉树

| | 满二叉树 | 完全二叉树 |
|---|---|---|
| 定义 | 每一层全满 | 最后一层从左到右连续，中间无空洞 |
| 节点数 | 恰好 $2^h - 1$ | $\leq 2^h - 1$ |
| 关系 | 满二叉树是完全二叉树的特例 | 更宽泛 |

完全二叉树可用数组紧凑存储：节点 $i$ 的左子为 $2i$，右子为 $2i+1$，父节点为 $\lfloor i/2 \rfloor$。

---

## 二、BST 定义与性质

**定义**：对任意节点，左子树所有节点的键 < 该节点键 < 右子树所有节点的键（递归成立，不只是直接子节点）。

**核心推论**：中序遍历 BST 得到严格递增序列。BST 是BST，当且仅当其中序遍历严格递增。

### 节点结构
```c
typedef struct BSTNode {
    int key;
    struct BSTNode *left, *right;
} BSTNode, *BST;
```

### 关于大小方向
- 标准定义：左小右大（惯例，与数轴一致）
- BST性质对**整棵子树**成立，不只是直接子节点
- 验证是否为BST需传递上下界：

```c
bool isBST(BSTNode *node, int min, int max) {
    if (!node) return true;
    if (node->key <= min || node->key >= max) return false;
    return isBST(node->left, min, node->key) &&
           isBST(node->right, node->key, max);
}
```

---

## 三、查找

```c
BSTNode* search(BST root, int key) {
    if (!root || root->key == key) return root;
    if (key < root->key) return search(root->left, key);
    else                 return search(root->right, key);
}
```

- 查找成功：落在内部节点
- 查找失败：走到 NULL（外部节点）

---

## 四、插入

**原则**：查找失败的位置就是插入位置。

**原因**：查找路径沿BST性质一路向下，路径上每次转向都保证了新节点与所有祖先的大小关系正确。走到NULL说明此处是唯一合法位置。

- 插入的新节点**一定是叶节点**
- 第一个插入的节点成为根，决定整棵树的骨架
- 物理上是接在空指针上，逻辑上是中序序列的前驱和后继之间

```c
BSTNode* insert(BST root, int key) {
    if (!root) {
        BSTNode *node = malloc(sizeof(BSTNode));
        node->key = key;
        node->left = node->right = NULL;
        return node;
    }
    if (key < root->key)      root->left  = insert(root->left,  key);
    else if (key > root->key) root->right = insert(root->right, key);
    return root;
}
```

### 插入建树流程
从空树开始，按给定顺序逐个调用 insert。每次插入都从根开始比较，这是保证BST性质的必要条件。

---

## 五、删除

分三种情况：

| Case | 条件 | 操作 |
|---|---|---|
| 1 | 叶节点 | 直接删，父指针置NULL |
| 2 | 只有一个子节点 | 用子节点替代 |
| 3 | 有两个子节点 | 找中序后继覆盖值，再删后继 |

**Case 3 的关键**：中序后继（右子树最左节点）无左子，删它必然是 Case 1 或 2，不会递归触发 Case 3。

```c
BSTNode* delete(BST root, int key) {
    if (!root) return NULL;
    if (key < root->key)
        root->left = delete(root->left, key);
    else if (key > root->key)
        root->right = delete(root->right, key);
    else {
        if (!root->left) {
            BSTNode *tmp = root->right;
            free(root); return tmp;
        } else if (!root->right) {
            BSTNode *tmp = root->left;
            free(root); return tmp;
        } else {
            BSTNode *succ = root->right;
            while (succ->left) succ = succ->left;
            root->key = succ->key;
            root->right = delete(root->right, succ->key);
        }
    }
    return root;
}
```

**`root->left = delete(root->left, key)` 的含义**：去左子树删key，把删完后左子树的新根接回来。统一处理三种情况（NULL/子节点/根不变）。

---

## 六、时间复杂度

| 操作 | 平均 | 最坏 |
|---|---|---|
| 查找 | $O(\log n)$ | $O(n)$ |
| 插入 | $O(\log n)$ | $O(n)$ |
| 删除 | $O(\log n)$ | $O(n)$ |

所有操作均为 $O(h)$，$h$ 为树高，范围 $[\log n, n]$。

**最坏情况**：按序插入（1,2,3...）退化成链表，$h = n$。

**为什么平衡时是 $O(\log n)$**：平衡树每层节点数约为上一层2倍，$n \approx 2^h \Rightarrow h \approx \log n$。偏斜时每层只有1个节点，$h = n$。

---

## 七、AVL 树

BST 加约束：任意节点左右子树高度差（平衡因子 BF）$\leq 1$。

**为什么保证 $O(\log n)$**：设 $N(h)$ 为高度 $h$ 的AVL树最少节点数：
$$N(h) = N(h-1) + N(h-2) + 1$$
结构同Fibonacci数列，$N(h) \approx \phi^h$，反推得 $h \leq 1.44\log_2 n$，即 $h = O(\log n)$。

**失衡与旋转**：插入后可能破坏平衡因子，分四种情况（LL/RR/LR/RL），单旋或双旋修复。旋转不改变中序序列，BST性质保持。

**意义**：用旋转的额外开销换取高度上界保证，使最坏情况从 $O(n)$ 降到 $O(\log n)$。

---

## 八、判定树（Decision Tree）

### 概念
把二分查找（有序数组上）的决策过程画成二叉树：
- 每个内部节点 = 一次比较
- 左子树 = 比它小时的后续查找范围
- 右子树 = 比它大时的后续查找范围
- 外部节点（NULL）= 查找失败的位置

本质：判定树就是BST，两者查找逻辑相同，只是载体不同（数组 vs 指针树）。

### 合法条件
对每个内部节点，左右子树节点数差 $\leq 1$。

**原因**：二分查找每次取中间元素，$n$ 个元素中间1个，剩余 $n-1$ 个：
- $n-1$ 为偶数：左右各 $(n-1)/2$，差为0
- $n-1$ 为奇数：左右差为1

差 $> 1$ 说明某次没取中间元素，不合法。

### 合法条件2：取中点方式全树一致
偶数个元素时，中间两个取左或取右都合法，但必须**全树统一**：
- 要么所有节点都是左子树节点数 ≥ 右子树节点数（取左中点）
- 要么所有节点都是右子树节点数 ≥ 左子树节点数（取右中点）

不能有的节点取左中点，有的取右中点，否则对应不到任何一种合法的二分查找实现。

### 判断方法
从根开始**递归地**对每个节点检查，不能只看根。同时验证条件1（差≤1）和条件2（偏向一致）。

### 查找性能
$n$ 个元素的二分查找，判定树高度 $h = \lceil \log_2(n+1) \rceil$，最多比较 $h$ 次。这是基于比较的查找的理论下界。
