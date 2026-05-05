# 线段树本质总结

## 1. 是什么

线段树是一棵完全二叉树，每个结点存储它所覆盖区间的**聚合值**。

- 叶结点：存原始数组元素
- 内部结点：存左右孩子区间聚合后的结果
- 父依赖子，自底向上构建

```
            [1,5]=15
           /        \
       [1,3]=6     [4,5]=9
       /    \       /    \
   [1,2]=3 [3]=3 [4]=4  [5]=5
   /    \
[1]=1  [2]=2
```

---

## 2. 本质：结合律操作的区间加速结构

线段树不是专为"区间和"设计的，而是为**任意满足结合律的二元操作**提供 O(log N) 的区间查询能力。

**能用线段树的操作 = 能分解成原子问题再合并的操作 = 满足结合律的操作**。

### 为什么必须是结合律？

线段树查询时，把大区间拆成若干子区间分别读值，再逐层合并。  
结合律保证：**无论怎么拆、怎么嵌套合并，结果和整体直接计算等价。**

```
op(op(A, B), C) == op(A, op(B, C))
```

没有结合律，父结点存的聚合值就失去意义，拆法不同结果不同，树不可信。

### 反例：减法不满足结合律

```
(3 - 1) - 2 = 0
3 - (1 - 2) = 4   ← 拆法不同，结果不同
```

所以减法不能直接用线段树。

---

## 3. 支持的操作

只要满足结合律，都可以套线段树框架，只需替换 merge 函数：

| 操作     | merge          | 单位元 |
|----------|----------------|--------|
| 区间和   | `a + b`        | 0      |
| 区间最小 | `min(a, b)`    | INF    |
| 区间最大 | `max(a, b)`    | -INF   |
| 区间 GCD | `gcd(a, b)`    | 0      |
| 区间乘积 | `a * b`        | 1      |

**GCD 满足结合律的原因**：由算术基本定理，gcd 对每个质因数取最小幂次，min 操作天然满足结合律，故 gcd 满足结合律。

**算术基本定理**：每个数可以唯一分解成质因数的幂次积，gcd 对每个质因数取**最小幂次**。

```
12 = 2² × 3
8  = 2³
6  = 2 × 3

gcd(12, 8, 6)：
  2 的最小幂次：min(2,3,1) = 1
  3 的最小幂次：min(1,0,1) = 0
  结果 = 2¹ × 3⁰ = 2
```

"取最小幂次"这个操作对每个质因数独立进行，min 本身满足结合律，所以 gcd 也满足结合律。不管先算哪两个数的 gcd，最终三个数的各质因数最小幂次是唯一确定的。

---

## 4. 三种核心操作

### Build — O(N)
后序遍历，叶结点存原始值，内部结点回溯时合并左右孩子。

```c
#define MAX_N 100000
int tree[4 * MAX_N];
int A[MAX_N];

void build(int node, int start, int end) {
    if (start == end) {
        tree[node] = A[start];          /* 叶结点直接存原始值 */
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node,     start, mid);    /* 递归建左子树 */
    build(2 * node + 1, mid + 1, end);  /* 递归建右子树 */
    tree[node] = tree[2*node] + tree[2*node+1]; /* 回溯时合并，换操作即换功能 */
}
/* 调用：build(1, 1, n) */
```

### Query — O(log N)
三种情况：
- 完全不相交 → 返回单位元
- 完全包含 → 直接返回 tree[node]
- 部分重叠 → 递归左右子树，合并结果

```c
int query(int node, int start, int end, int L, int R) {
    if (R < start || end < L)           /* 完全不相交，返回单位元 */
        return 0;
    if (L <= start && end <= R)         /* 完全包含，直接返回预存值 */
        return tree[node];
    /* 部分重叠：拆开递归，合并结果 */
    int mid = (start + end) / 2;
    int left  = query(2*node,     start, mid, L, R);
    int right = query(2*node + 1, mid+1, end, L, R);
    return left + right;                /* 结合律保证拆开合并等价于整体 */
}
/* 调用：query(1, 1, n, L, R) */
```

### Update — O(log N)
- **点更新**：从根找到目标叶，修改叶，回溯时沿路更新祖先
- **区间更新**：用 Lazy Propagation，打懒标记延迟下推，避免 O(N log N)

```c
/* 点更新：idx 位置改为 val */
void update(int node, int start, int end, int idx, int val) {
    if (start == end) {
        A[idx] = val;
        tree[node] = val;               /* 找到叶结点，直接改 */
        return;
    }
    int mid = (start + end) / 2;
    if (idx <= mid)
        update(2*node,     start, mid,   idx, val); /* 目标在左子树 */
    else
        update(2*node + 1, mid+1, end,   idx, val); /* 目标在右子树 */
    tree[node] = tree[2*node] + tree[2*node+1];     /* 回溯时更新祖先 */
}
/* 调用：update(1, 1, n, idx, val) */
```

---

## 5. Lazy Propagation 本质

区间更新时，当查询区间完全包含当前结点区间，不继续递归子结点，  
而是在当前结点打懒标记，记录"子树尚未更新"，等真正访问子结点时再下推。

核心操作 push_down：把当前结点的 lazy 传给左右孩子，清除自身标记。  
Query 遇到部分重叠时，也需要先 push_down，否则读到旧值。

```c
int lazy[4 * MAX_N];  /* 初始全0 */

/* 下推懒标记：把当前结点的承诺传给左右孩子 */
void push_down(int node, int start, int end) {
    if (lazy[node] != 0) {
        int mid = (start + end) / 2;
        int lc = 2 * node, rc = 2 * node + 1;
        /* 更新左孩子 */
        tree[lc]  += lazy[node] * (mid - start + 1);
        lazy[lc]  += lazy[node];
        /* 更新右孩子 */
        tree[rc]  += lazy[node] * (end - mid);
        lazy[rc]  += lazy[node];
        /* 清除当前标记，承诺已兑现 */
        lazy[node] = 0;
    }
}

/* 区间更新：[L,R] 内每个元素加 c */
void range_update(int node, int start, int end, int L, int R, int c) {
    if (R < start || end < L)           /* 完全不相交，跳过 */
        return;
    if (L <= start && end <= R) {       /* 完全包含，打标记，不下推 */
        tree[node] += c * (end - start + 1);
        lazy[node] += c;
        return;
    }
    push_down(node, start, end);        /* 部分重叠，先兑现旧承诺再递归 */
    int mid = (start + end) / 2;
    range_update(2*node,     start, mid,   L, R, c);
    range_update(2*node + 1, mid+1, end,   L, R, c);
    tree[node] = tree[2*node] + tree[2*node+1];
}

/* 带 lazy 的查询：部分重叠前必须先 push_down */
int query_lazy(int node, int start, int end, int L, int R) {
    if (R < start || end < L) return 0;
    if (L <= start && end <= R) return tree[node];
    push_down(node, start, end);        /* 先把旧标记下推，否则子结点值是旧的 */
    int mid = (start + end) / 2;
    return query_lazy(2*node,     start, mid, L, R)
         + query_lazy(2*node + 1, mid+1, end, L, R);
}
```

---

## 6. 空间

数组大小开 **4N**，保证完全二叉树最坏情况下不越界。

### 推导

**树高公式 h = ⌈log₂n⌉ + 1**

线段树每次把区间对半分，直到长度为1（叶结点）。对半分的次数 = ⌈log₂n⌉，加上根这一层，树高 h = ⌈log₂n⌉ + 1。

注意是向上取整：n=5 时 ⌈log₂5⌉=3，h=4；若用向下取整 ⌊log₂5⌋=2，h=3，与实际不符。

```
n=4：4→2→1，分2次，⌈log₂4⌉=2，h=3
n=5：5→3→2→1，分3次，⌈log₂5⌉=3，h=4
n=8：8→4→2→1，分3次，⌈log₂8⌉=3，h=4
```

**最大下标 = 2^h - 1**

根在下标1，左孩子 = 2*node，右孩子 = 2*node+1。第 h 层下标范围 [2^(h-1), 2^h-1]，最大下标 = 2^h - 1。

**最坏情况：n = 2^k + 1**

```
n = 2^k     → ⌈log₂n⌉ = k   → h = k+1 → 最大下标 = 2^(k+1) - 1 ≈ 2n
n = 2^k + 1 → ⌈log₂n⌉ = k+1 → h = k+2 → 最大下标 = 2^(k+2) - 1 ≈ 4n
```

n 只比 2^k 多1，树高多一层，最大下标从 2n 跳到 4n。

**4n 的严格推导**

```
最大下标 = 2^(k+2) - 1
         = 4 × 2^k - 1
         < 4 × (2^k + 1)
         = 4n
```

所以开 4N 的数组，任何 n 下最大下标都不会越界。4N 是安全上界，实际用不满。

---

## 7. 更深的本质理解

**线段树是"预计算 + 按需组合"的空间换时间。**

朴素做法：每次查询 [L,R] 都从头遍历，O(N)。  
线段树：Build 时把所有可能用到的子区间结果预存好，Query 时直接取出来组合，O(log N)。

代价是 O(N) 的额外空间（存 2N-1 个结点的聚合值），换来每次查询从 O(N) 降到 O(log N)。

**为什么是 O(log N) 而不是更少？**

Query 时，任意区间 [L,R] 最多被分解成 O(log N) 个"完全包含"的子区间——这是二叉树高度决定的，每层最多贡献2个边界结点，树高 log N，所以最多 2log N 个结点被访问。

**Lazy 的本质是"延迟实现承诺"。**

区间更新时，对覆盖的子树打标记等于"承诺这个区间会被更新"，但暂时不兑现。只有当后续操作真正需要访问子结点时，才把承诺下推兑现。本质是把无效的更新工作推迟甚至合并，避免对不会被查询的结点做无谓计算。
