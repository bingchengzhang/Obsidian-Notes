# 用栈实现BST中序遍历

## 栈的本质

递归能"上去"，依赖的是系统调用栈自动保存每一层的状态，return之后自动回到上一层。

显式栈做的事情完全一样，只是把这个过程手动管理：
- **压栈** = 记住我在哪
- **弹出** = 回到上一级，每弹出一次就得到一个parent

不需要指针反向索引，利用的是栈保存上一级信息。

**推论：所有递归都可以用显式栈改写。** 递归依赖系统调用栈，显式栈只是把这个过程暴露出来自己管理。DFS既可以递归写也可以用栈写，两者完全等价。

---

## 为什么中序遍历需要栈

递归版本：

```c
void inorder(Node *root) {
    if (!root) return;
    inorder(root->left);   // 先左
    printf("%d ", root->val); // 再根
    inorder(root->right);  // 再右
}
```

系统调用栈隐式保存了"我还没处理完"的节点。
显式栈版本就是把这个隐式过程手动写出来。

---

## 核心逻辑

用一句话概括：

> **每次必弹，所以先压所有左，有右就进右继续压弹，没右就往上弹。**

两种情况：
- **弹出节点有右子树** → current转向右子树，把右子树的左链全压进来，再次触底反弹
- **弹出节点没有右子树** → current=NULL，内层while不执行，直接继续弹，弹出的是parent

外层循环条件：**有得压或者有得弹，就继续循环。**

```c
while (current != NULL || i > 0)
```

- `current != NULL`：还有节点没压
- `i > 0`：栈里还有节点没弹
- 两个条件缺一个都会漏情况

---

## 完整代码

```c
// 假设节点定义
typedef struct Node {
    int val;
    struct Node *left, *right;
} Node;

void inorder(Node *root) {
    Node *stack[100];   // 栈，存节点指针（不是val）
    int i = 0;          // 栈顶指针
    Node *current = root;

    while (current != NULL || i > 0) {
        // 压左链：一路往左走到底
        while (current != NULL) {
            stack[i++] = current;
            current = current->left;
        }
        // 触底：弹出栈顶，输出val
        current = stack[--i];
        printf("%d ", current->val);
        // 转向右子树
        current = current->right;
    }
}
```

**注意：压栈存的是节点指针，不是val。** 因为弹出之后需要访问right，只有val找不到右子树。

---

## 执行过程示例

树结构：
```
        4
       / \
      2   6
     / \ / \
    1  3 5  7
```

| 步骤 | 操作 | 栈内容 | 输出 |
|------|------|--------|------|
| 压左链 | 压4→2→1，current=NULL | [4,2,1] | |
| 触底反弹 | 弹1，current=1的右=NULL | [4,2] | 1 |
| 继续弹 | 弹2，current=2的右=3 | [4] | 2 |
| 压左链 | 压3，current=NULL | [4,3] | |
| 触底反弹 | 弹3，current=3的右=NULL | [4] | 3 |
| 继续弹 | 弹4，current=4的右=6 | [] | 4 |
| 压左链 | 压6→5，current=NULL | [6,5] | |
| 触底反弹 | 弹5，current=5的右=NULL | [6] | 5 |
| 继续弹 | 弹6，current=6的右=7 | [] | 6 |
| 压左链 | 压7，current=NULL | [7] | |
| 触底反弹 | 弹7，current=7的右=NULL | [] | 7 |

输出：**1 2 3 4 5 6 7** ✓

---

## 总结

```
压左链 → 触底 → 弹 → 有右就扎进去重复
                  → 没右继续弹
```

栈在整个过程中存储的始终是"左链上还没输出的节点"，保证了中序遍历左→根→右的顺序。
