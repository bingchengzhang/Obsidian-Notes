# 中序后序建树C语言实现 \- Gemini

**Date**: March 30, 2026 at 10:50 PM
**Turns**: 1
**Source**: [Gemini Chat](https://gemini.google.com/u/0/app/75231d205008f5fc)

---

## Turn 1

### 🤖 Assistant

既然你对 **ZigZag（之字形）** 遍历的算法深度感兴趣，那我们就跳过那个需要手动反转数组的“单队列法”，直接上最硬核、物理逻辑最美妙的方案：**双栈法（Two-Stacks Approach）**。
这个算法的神奇之处在于：它完全不需要 `isodd` 这种逻辑开关去翻转数组索引，而是利用栈“后进先出”的物理特性，让左右子树在进出栈的过程中，自动排成正确的“之字形”。

---

## 双栈法核心逻辑
我们准备两个栈：`stack_current`（当前层）和 `stack_next`（下一层）。

1. **Level 0 (根节点)**：从 `stack_current` 弹出根节点。为了让下一层（Level 1）实现**从左往右**，我们按照**先右后左**的顺序把孩子压入 `stack_next`。
2. **Level 1**：从 `stack_next` 弹出节点。由于栈是后进先出，左孩子会先出来。为了让再下一层（Level 2）实现**从右往左**，我们按照**先左后右**的顺序把孩子压回 `stack_current`。

---

## 完整的 C 代码实现
这份代码包含了你之前的 `treebuild` 修复逻辑，并完整实现了双栈遍历。

```c
#include <stdio.h>
#include <stdlib.h>

#define MAX 100 // N <= 30，100 的容量绰绰有余

typedef struct tree_node {
    int element;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

// 预定义函数
tree_node* treebuild(int* inorder, int* postorder, int in_l, int in_r, int post_l, int post_r);
void dual_stack_zigzag(tree_node* root, int N);

int main() {
    int N;
    if (scanf("%d", &N) != 1 || N <= 0) return 0;

    int inorder[MAX], postorder[MAX];
    for (int i = 0; i < N; i++) scanf("%d", &inorder[i]);
    for (int i = 0; i < N; i++) scanf("%d", &postorder[i]);

    // 使用 4 边界法建立唯一的二叉树
    tree_node* root = treebuild(inorder, postorder, 0, N - 1, 0, N - 1);
    
    // 执行双栈之字形打印
    dual_stack_zigzag(root, N);

    return 0;
}

// ---------------------------------------------------------
// 核心：双栈之字形遍历算法
// ---------------------------------------------------------
void dual_stack_zigzag(tree_node* root, int N) {
    if (!root) return;

    // 定义两个栈
    tree_node* s1[MAX]; // 负责“从右往左”出的层
    tree_node* s2[MAX]; // 负责“从左往右”出的层
    int top1 = -1, top2 = -1;
    int printed_count = 0;

    // 初始：将根节点放入 s1
    s1[++top1] = root;

    // 只要两个栈中还有一个不为空
    while (top1 != -1 || top2 != -1) {
        
        // 模式 A：从右往左出栈（此时下一层要从左往右打印）
        while (top1 != -1) {
            tree_node* curr = s1[top1--];
            printf("%d", curr->element);
            if (++printed_count < N) printf(" ");

            // 【关键点】为了让下一层从左往右，s2 压栈顺序：先右后左
            if (curr->right) s2[++top2] = curr->right;
            if (curr->left)  s2[++top2] = curr->left;
        }

        // 模式 B：从左往右出栈（此时下一层要从右往左打印）
        while (top2 != -1) {
            tree_node* curr = s2[top2--];
            printf("%d", curr->element);
            if (++printed_count < N) printf(" ");

            // 【关键点】为了让下一层从右往左，s1 压栈顺序：先左后右
            if (curr->left)  s1[++top1] = curr->left;
            if (curr->right) s1[++top1] = curr->right;
        }
    }
    printf("\n");
}

// ---------------------------------------------------------
// 辅助：二叉树重建 (4 指针法)
// ---------------------------------------------------------
tree_node* treebuild(int* inorder, int* postorder, int in_l, int in_r, int post_l, int post_r) {
    if (in_l > in_r || post_l > post_r) return NULL;

    tree_node* node = (tree_node*)malloc(sizeof(tree_node));
    int root_val = postorder[post_r];
    node->element = root_val;

    int i = in_l;
    while (i <= in_r && inorder[i] != root_val) i++;
    int left_len = i - in_l;

    node->left = treebuild(inorder, postorder, in_l, i - 1, post_l, post_l + left_len - 1);
    node->right = treebuild(inorder, postorder, i + 1, in_r, post_l + left_len, post_r - 1);

    return node;
}

```

---

## 代码逻辑关键剖析

1. **为什么不需要 isodd 变量？**
双栈法通过代码的**结构**（两个并列的 `while` 循环）天然地表达了“第一层这种规则，第二层那种规则”的逻辑。当 `s1` 的循环跑完，程序自然进入 `s2` 的循环，方向切换是自动发生的。
2. **压栈顺序的玄机**：
  - 在栈里，**后进去的先出来**。
  - 如果你希望下一层出栈顺序是 `L -> R`，你必须先压 `R` 再压 `L`。
  - 如果你希望下一层出栈顺序是 `R -> L`，你必须先压 `L` 再压 `R`。
  - 这就是代码中 `curr->right` 和 `curr->left` 交换位置的原因。
3. **空格控制**：
使用了 `printed_count` 全局计数。只要打印的数字还没达到总数 $N$，就补一个空格。这样完美避开了行末多余空格的坑。

## 专家经验反思
双栈法在处理 $N \le 30$ 这种小规模题目时性能优势不明显，但在**面试场景**下，它能展示你对数据结构特性（栈的 LIFO）的深刻理解。相比于队列法，它最大的魅力在于：**它没有改变数据的存储方向，而是改变了数据的读取节奏。**
**这份代码现在已经是一个可以直接提交并 AC 的完整版本了。你觉得双栈法这种“左右横跳”的压栈逻辑，比之前那个算数组下标的逻辑，是不是在直觉上更顺畅了一些？**

---

*Exported from [Voyager](https://github.com/Nagi-ovo/gemini-voyager)*  
*Generated on March 30, 2026 at 10:50 PM*