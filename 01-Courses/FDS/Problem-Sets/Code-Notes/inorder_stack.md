# inorder_stack

## 来源

原文件：`inorder_stack.c`

## 用途

TODO：补充这段代码对应的题目或功能。

## 核心思路

TODO：补充算法思路。

## C 代码

```c
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    TreeNode *data[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

int isFull(Stack *s) {
    return s->top == MAX_SIZE - 1;
}

void push(Stack *s, TreeNode *node) {
    if (isFull(s)) {
        printf("Stack overflow\n");
        return;
    }
    s->data[++s->top] = node;
}

TreeNode* pop(Stack *s) {
    if (isEmpty(s)) {
        return NULL;
    }
    return s->data[s->top--];
}

TreeNode* createNode(int val) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* 使用栈实现二叉树的中序遍历 (左 -> 根 -> 右) */
void inorderTraversal(TreeNode *root) {
    Stack s;
    initStack(&s);
    TreeNode *cur = root;

    while (cur != NULL || !isEmpty(&s)) {
        /* 一直向左走，沿途节点入栈 */
        while (cur != NULL) {
            push(&s, cur);
            cur = cur->left;
        }
        /* 弹出栈顶节点并访问 */
        cur = pop(&s);
        printf("%d ", cur->val);
        /* 转向右子树 */
        cur = cur->right;
    }
    printf("\n");
}

void freeTree(TreeNode *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void) {
    /*
            1
           / \
          2   3
         / \   \
        4   5   6
    */
    TreeNode *root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->right = createNode(6);

    printf("Inorder traversal: ");
    inorderTraversal(root);  /* 期望输出: 4 2 5 1 3 6 */

    freeTree(root);
    return 0;
}
```
