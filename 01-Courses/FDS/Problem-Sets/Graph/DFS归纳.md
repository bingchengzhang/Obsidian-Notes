# 中序后序建树C语言实现 \- Gemini

**Date**: March 30, 2026 at 11:11 PM
**Turns**: 1
**Source**: [Gemini Chat](https://gemini.google.com/u/0/app/75231d205008f5fc)

---

## Turn 1

### 🤖 Assistant

既然你决定要搞一次“大满贯”，把二叉树的遍历地图全部点亮，那我们就把这些招式分门别类，从最优雅的**递归**到最硬核的**迭代**，一次性全部拆解清楚。
为了保持一致性，我们统一使用你代码里的结构：

```c
typedef struct tree_node {
    int element;
    struct tree_node *left, *right;
} tree_node;

```

---

## 1. 深度优先搜索 (DFS)：递归全家桶
递归的精髓在于：**逻辑完全一样，只是处理“根”的时机不同**。

| 遍历方式 | 顺序 | 代码逻辑 (极简版) |
| --- | --- | --- |
| 前序 (Pre-order) | 根 -> 左 -> 右 | visit(root); DFS(root->left); DFS(root->right); |
| 中序 (In-order) | 左 -> 根 -> 右 | DFS(root->left); visit(root); DFS(root->right); |
| 后序 (Post-order) | 左 -> 右 -> 根 | DFS(root->left); DFS(root->right); visit(root); |

---

## 2. 深度优先搜索 (DFS)：手动栈迭代版
当你不想让系统自动压栈（怕爆栈）时，就得自己背着“包（Stack）”上山。

## A. 前序遍历 (根-左-右) —— 最简单的迭代
前序最爽，因为一看到节点就能处理。

```c
void iter_preorder(tree_node* root) {
    if (!root) return;
    tree_node* stack[MAX];
    int top = -1;
    stack[++top] = root;
    while (top != -1) {
        tree_node* curr = stack[top--];
        printf("%d ", curr->element); // 根
        // 注意：栈是后进先出，所以先压右，再压左，出来的就是左-右
        if (curr->right) stack[++top] = curr->right;
        if (curr->left) stack[++top] = curr->left;
    }
}

```

## B. 中序遍历 (左-根-右) —— 撞墙回头法
这就是我们之前剖析过的“一路向左钻，钻不动了再回头”。

```c
void iter_inorder(tree_node* root) {
    tree_node* stack[MAX];
    int top = -1;
    tree_node* curr = root;
    while (curr || top != -1) {
        while (curr) { stack[++top] = curr; curr = curr->left; } // 钻左边
        curr = stack[top--];
        printf("%d ", curr->element); // 根
        curr = curr->right;           // 去右边
    }
}

```

## C. 后序遍历 (左-右-根) —— 双栈法
后序迭代是“大 BOSS”。最取巧的方法是：前序是“根-左-右”，我们稍微改下顺序变成“根-右-左”，最后把结果翻转过来，就是“左-右-根”。用两个栈实现最优雅：

```c
void iter_postorder(tree_node* root) {
    if (!root) return;
    tree_node *s1[MAX], *s2[MAX]; // s2 用来存逆序结果
    int t1 = -1, t2 = -1;
    s1[++t1] = root;
    while (t1 != -1) {
        tree_node* curr = s1[t1--];
        s2[++t2] = curr; // 进 s2 暂存
        if (curr->left) s1[++t1] = curr->left;
        if (curr->right) s1[++t1] = curr->right;
    }
    while (t2 != -1) printf("%d ", s2[t2--]->element); // 弹出即为左右根
}

```

---

## 3. 广度优先搜索 (BFS)：层序遍历
这就是你刚才练习的“收费站”模型，利用队列实现。

```c
void level_order(tree_node* root) {
    if (!root) return;
    tree_node* queue[MAX];
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        tree_node* curr = queue[front++];
        printf("%d ", curr->element);
        if (curr->left) queue[rear++] = curr->left;
        if (curr->right) queue[rear++] = curr->right;
    }
}

```

---

## 专家的硬核总结表

| 维度 | 递归 DFS | 迭代 DFS (栈) | 层序 BFS (队列) |
| --- | --- | --- | --- |
| 代码量 | 极少 (3-5行) | 较多 | 中等 |
| 直觉感 | 极强 | 较差 (需模拟栈) | 强 (剥洋葱) |
| 内存瓶颈 | 系统调用栈 (易溢出) | 手动申请的栈空间 | 队列宽度 (树的最宽处) |
| 核心技巧 | 找子问题共同点 | 找入栈/出栈时机 | 拍层级快照 |
**现在你手里已经掌握了二叉树通行的所有“步法”。既然所有的遍历都能写出来了，你要不要试着把这几种方法封装进一个测试程序里，用你之前那个 8 节点的数据跑一下，亲眼看看同一个树在不同“步法”下的输出差异？**

---

*Exported from [Voyager](https://github.com/Nagi-ovo/gemini-voyager)*  
*Generated on March 30, 2026 at 11:11 PM*