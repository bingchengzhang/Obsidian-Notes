# 并查集（Disjoint Set / Union-Find）完整笔记

---

## 1. 背景：等价关系

等价关系必须满足三条性质：

| 性质 | 英文 | 定义 |
|------|------|------|
| 自反性 | Reflexive | a ~ a |
| 对称性 | Symmetric | a ~ b ⟹ b ~ a |
| 传递性 | Transitive | a ~ b 且 b ~ c ⟹ a ~ c |

**三条性质对数据结构设计的意义：**

- **自反性** → 每个元素必须有所属集合，初始化时每个元素自成一集
- **对称性** → 集合内部无序，结点平等，谁当根都行，路径压缩因此合法
- **传递性** → Union 合法，把两棵树的根连起来就足够，无需逐一验证新集合内所有元素对

---

## 2. 基本数据结构

用**数组**表示森林，下标即元素编号，指针从子结点指向父结点（与普通树相反）。

```
S[x] > 0  →  x 不是根，S[x] 是 x 的父结点
S[x] = 0  →  x 是根（朴素版）
S[x] < 0  →  x 是根（size/rank 版，|S[x]| = 集合大小或 rank）
```

两个元素同属一集合 ⟺ Find 结果相同 ⟺ 根相同。

**核心操作：**

- **Find(x)**：沿父指针爬到根，返回根的下标（集合代表元）
- **Union(a, b)**：Find(a)、Find(b) 各自找到根，按规则把一棵树挂到另一棵树下

---

## 3. Find 详解

**本质**：从 x 出发，沿父指针向上爬，直到 `S[root] ≤ 0`（根），返回根的下标。

非根的父结点编号永远是正整数，所以 `S[x] > 0` 的判断在三种版本下都通用。

### 朴素 Find

```c
int Find(int x) {
    for (; S[x] > 0; x = S[x]);  /* S[x]>0 说明不是根，继续爬 */
    return x;
}
```

### 带路径压缩的 Find（迭代两趟版，推荐）

```c
int Find(int x) {
    int root, trial, lead;
    /* 第一趟：找根，不改任何指针 */
    for (root = x; S[root] > 0; root = S[root]);
    /* 第二趟：把路径上所有结点直接接到根 */
    for (trial = x; trial != root; trial = lead) {
        lead = S[trial];   /* 先保存原父结点，否则覆盖后丢失 */
        S[trial] = root;   /* 直接接到根 */
    }
    return root;
}
```

> **为什么需要 lead**：`S[trial] = root` 覆盖了原父结点，不提前保存的话
> 下一轮 `trial = lead` 就找不到原来的路径了。

### 带路径压缩的 Find（递归版）

```c
int Find(int x) {
    if (S[x] <= 0) return x;
    else return S[x] = Find(S[x]);  /* 回溯时把每个结点直接接到根 */
}
```

简洁，但树很深时有栈溢出风险，迭代版更安全。

**路径压缩的合法性**：对称性保证集合内结点平等，谁直接挂在根下都不影响等价关系。

**效果**：第一次 Find(x) 走 k 步，顺手压平路径，之后再 Find 这些结点全部 O(1)，
把第一次的代价均摊给后续操作。不压缩的话同一条路每次都重复走。

---

## 4. 朴素实现

```c
#define MAX 10002
int S[MAX];

void init(int n) {
    for (int i = 1; i <= n; i++)
        S[i] = 0;  /* 每个元素自成一集，根存0 */
}

int Find(int x) {
    for (; S[x] > 0; x = S[x]);
    return x;
}

void Union(int a, int b) {
    int ra = Find(a), rb = Find(b);
    if (ra != rb)
        S[rb] = ra;  /* 直接把 rb 挂到 ra 下，无脑合并 */
}
```

**最坏情况**：按顺序 `union(2,1), union(3,2), ..., union(N,N-1)`，
退化成链，Find = O(N)，总体 **O(N²)**。

---

## 5. Union-by-Size（按大小合并）

**规则**：小树挂大树，根存 `-size`，初始 `-1`。

合并时必须先 Find 到根，因为大小信息只存在根上，中间结点不知道集合大小。

```c
void init(int n) {
    for (int i = 1; i <= n; i++)
        S[i] = -1;  /* 每棵树大小为1，存-1 */
}

void Union_size(int a, int b) {
    int ra = Find(a), rb = Find(b);
    if (ra == rb) return;  /* 判断根相同，不是值相同！ */

    if (S[ra] <= S[rb]) {  /* ra 更大（负数更小 = 绝对值更大） */
        S[ra] += S[rb];    /* 合并大小 */
        S[rb] = ra;        /* 小树挂大树 */
    } else {
        S[rb] += S[ra];
        S[ra] = rb;
    }
}
```

> ⚠️ **常见 bug**：`if (S[ra] == S[rb]) return` 是错的。
> 初始全是 `-1`，两个不同集合的根值相等，
> 用值判断会导致任何 Union 都直接 return，关系永远建不起来。
> 应判断**根的下标**是否相同：`if (ra == rb) return`。

**树高上界**：结点深度每增加1，集合大小至少翻倍（被合并进至少同等大小的树）。
大小上限是 N，翻倍次数上限是 log₂N，所以任意结点深度 ≤ **⌊log₂N⌋**。

复杂度：**O(N + M log₂N)**。

---

## 6. Union-by-Rank（按秩合并）

**规则**：矮树挂高树，根存 `-rank`（树高估计值），初始 `-1`。

```c
void Union_rank(int a, int b) {
    int ra = Find(a), rb = Find(b);
    if (ra == rb) return;

    int rank_a = -S[ra], rank_b = -S[rb];

    if (rank_a > rank_b) {
        S[rb] = ra;          /* b 挂 a，rank 不变 */
    } else if (rank_a < rank_b) {
        S[ra] = rb;          /* a 挂 b，rank 不变 */
    } else {
        S[rb] = ra;          /* 等高任选，被挂的 rank+1 */
        S[ra]--;             /* 负数-1 = rank+1 */
    }
}
```

**树高上界证明（归纳）**：

- **base case**：rank=0，单结点，结点数 = 1 = 2⁰，成立
- **归纳步**：rank+1 **只能**由两棵 rank=k 的树合并产生（规则保证），
  各自结点数 ≥ 2^k，合并后 ≥ 2^k + 2^k = 2^(k+1)，归纳成立

结论：rank=k ⟹ 结点数 ≥ 2^k ⟹ **rank ≤ log₂N** ⟹ 树高 ≤ log₂N。

**最坏情况构造**：每次合并等 rank 的树，高度+1，结点数×2，
经过 log₂N 次后到达 N，树高恰好 ⌊log₂N⌋。
并查集树不限制度数，是多叉树，二叉只是这种构造的副产品，与结构无关。

**路径压缩后的兼容性**：压缩改变真实树高，rank 变成估计值（上界），
但合并方向仍然正确，不影响实际效果。

复杂度：**O(N + M log₂N)**。

---

## 7. 路径压缩 + Union-by-Rank 结合

| 版本 | 复杂度 |
|------|--------|
| 朴素 | O(N²) |
| Union-by-Rank 单独 | O(N + M log N) |
| 路径压缩单独 | O(N + M log N) |
| **两者结合** | **O(M · α(M,N))** |

**为什么单独用各自只有 O(M log N)**：
- 单独 rank：路径不压缩，同一条路每次重复走，无均摊收益
- 单独压缩：树高无约束，第一次走的路径可能极长，压缩本身代价过高

**两者合用才能达到 O(Mα)**：
- rank 保证树高 ≤ log N，路径长度有上界
- 压缩消除重复遍历，走过的路不重走
- 双重约束使均摊代价从 O(log N) 降到 O(α)，效果非线性叠加

---

## 8. Tarjan 引理与逆 Ackermann 函数

**T(M, N)**：处理 M 次 Find + N-1 次 Union 的总时间。

**Tarjan 引理**（紧界）：

$$k_1 M \cdot \alpha(M,N) \leq T(M,N) \leq k_2 M \cdot \alpha(M,N)$$

上下界同阶，复杂度**恰好是** Θ(Mα(M,N))，不能再优化。

**log\* N（迭代对数）**：对 N 反复取 log₂ 直到 ≤ 1 的次数：

| N | log\* N |
|---|---------|
| 2 | 1 |
| 4 | 2 |
| 16 | 3 |
| 65536 | 4 |
| 2^65536 | 5 |

现实中 log\* N ≤ 4 永远成立。α 比 log\* 更小，实际 **α ≤ 4**，工程上视为常数。

**α 的来源**：Tarjan 用势能函数对所有可能操作序列做均摊分析，
α 是分析闭合的数学产物，不是从算法结构直接看出来的。

**总复杂度**：N-1 次 Union 共 O(N)，M 次 Find 均摊 O(Mα)，
M ≥ N 时 O(N) 被吸收，总计 **O(Mα(M,N)) ≈ O(M)**。

---

## 9. 完整代码（三版对比）

```c
#include <stdio.h>
#define MAX_N 10002

/* ============================================================
 * 朴素版：S[root] = 0
 * ============================================================ */
int S_naive[MAX_N];

void naive_init(int n) {
    for (int i = 1; i <= n; i++) S_naive[i] = 0;
}

int naive_find(int x) {
    for (; S_naive[x] > 0; x = S_naive[x]);
    return x;
}

void naive_union(int a, int b) {
    int ra = naive_find(a), rb = naive_find(b);
    if (ra != rb) S_naive[rb] = ra;
}

/* ============================================================
 * Union-by-Size：S[root] = -size，初始 -1
 * ============================================================ */
int S_size[MAX_N];

void size_init(int n) {
    for (int i = 1; i <= n; i++) S_size[i] = -1;
}

int size_find(int x) {
    int root, trial, lead;
    for (root = x; S_size[root] > 0; root = S_size[root]);
    for (trial = x; trial != root; trial = lead) {
        lead = S_size[trial];
        S_size[trial] = root;  /* 路径压缩 */
    }
    return root;
}

void size_union(int a, int b) {
    int ra = size_find(a), rb = size_find(b);
    if (ra == rb) return;
    if (S_size[ra] <= S_size[rb]) {  /* ra 更大 */
        S_size[ra] += S_size[rb];
        S_size[rb] = ra;
    } else {
        S_size[rb] += S_size[ra];
        S_size[ra] = rb;
    }
}

/* ============================================================
 * Union-by-Rank + 路径压缩：S[root] = -rank，初始 -1
 * ============================================================ */
int S_rank[MAX_N];

void rank_init(int n) {
    for (int i = 1; i <= n; i++) S_rank[i] = -1;
}

int rank_find(int x) {
    int root, trial, lead;
    for (root = x; S_rank[root] > 0; root = S_rank[root]);
    for (trial = x; trial != root; trial = lead) {
        lead = S_rank[trial];
        S_rank[trial] = root;  /* 路径压缩 */
    }
    return root;
}

void rank_union(int a, int b) {
    int ra = rank_find(a), rb = rank_find(b);
    if (ra == rb) return;
    int rank_a = -S_rank[ra], rank_b = -S_rank[rb];
    if (rank_a > rank_b) {
        S_rank[rb] = ra;
    } else if (rank_a < rank_b) {
        S_rank[ra] = rb;
    } else {
        S_rank[rb] = ra;
        S_rank[ra]--;  /* 负数-1 = rank+1 */
    }
}
```

---

## 10. 应用：File Transfer（连通性判断）

**问题**：N 台计算机，动态加入连接（I），查询两台是否连通（C），最终输出连通分量数。

### 操作映射

| 输入 | 对应操作 | 含义 |
|------|----------|------|
| `I c1 c2` | Union(c1, c2) | 建立连接，合并两个集合 |
| `C c1 c2` | Find(c1)==Find(c2) | 查询是否连通 |
| `S` | 停止，统一输出所有结果 | — |

**Dynamic（动态）**：I 和 C 任意穿插，不是先建图再查询。
比如先 C 查询得 no，再 I 建立连接，再 C 查询得 yes。
并查集天然支持在线 Union/Find 混合操作，正适合此题。

**为什么需要 state[]**：题目要求所有 C 的结果在读到 S 之后统一输出，
不能读到 C 就立刻 printf，必须先用 state[] 存下来，最后统一输出。

### 完整代码

```c
#include <stdio.h>
#define MAX 10002

int set[MAX];    /* 并查集数组，全局默认0，需手动初始化为-1 */
int state[MAX];  /* 存储每次 C 查询的结果，0=no，1=yes */

int Find(int c) {
    int root, trial, lead;
    for (root = c; set[root] > 0; root = set[root]);  /* 找根 */
    for (trial = c; set[trial] > 0; trial = lead) {   /* 路径压缩 */
        lead = set[trial];
        set[trial] = root;
    }
    return root;
}

void Union_size(int c1, int c2) {
    int rc1 = Find(c1), rc2 = Find(c2);
    if (rc1 == rc2) return;                /* 判断根相同，不是值相同 */
    if (set[rc1] <= set[rc2]) {            /* rc1 更大（负数更小） */
        set[rc1] += set[rc2];
        set[rc2] = rc1;
    } else {
        set[rc2] += set[rc1];
        set[rc1] = rc2;
    }
}

void judge_connect(int N) {
    int count = 0;
    for (int i = 1; i <= N; i++)
        if (set[i] < 0) count++;          /* 是根就计数，一个根=一个连通分量 */
    if (count == 1)
        printf("The network is connected.");
    else
        printf("There are %d components.", count);
}

int main() {
    int N;
    scanf("%d", &N);
    for (int i = 1; i <= N; i++)
        set[i] = -1;                       /* 初始每个元素自成一集 */

    char op;
    int c1, c2, j = 0;
    while (1) {
        scanf(" %c", &op);                 /* %c 前加空格跳过换行符 */
        if (op == 'S') break;
        scanf("%d %d", &c1, &c2);
        if (op == 'C')
            state[j++] = (Find(c1) == Find(c2)); /* 存结果 */
        else
            Union_size(c1, c2);
    }
    for (int i = 0; i < j; i++)           /* 统一输出所有 C 的结果 */
        printf("%s\n", state[i] ? "yes" : "no");
    judge_connect(N);
    return 0;
}
```

### 值得学习的地方

**1. 全局数组与手动初始化**

`int set[MAX]` 全局默认0，但 union-by-size 需要根存 `-1`，
必须手动 `for (i=1; i<=N; i++) set[i] = -1`，下标从1开始，不是0。

**2. `scanf(" %c", &op)` 的空格**

`%c` 前必须加空格，跳过上一行输入留下的换行符 `\n`，否则 op 读错。
`%d` 不需要，因为它自动跳过空白字符。

**3. Union 判断根相同而非值相同**

```c
if (rc1 == rc2) return;              /* ✓ 判断根的下标 */
if (set[rc1] == set[rc2]) return;    /* ✗ 判断值，初始全-1会误判 */
```

**4. 连通分量数 = 根的数量**

`set[i] < 0` 是根，一个根代表一个连通分量。
直接数根的个数，不要用最大集合的大小（最小负数的绝对值）当分量数，
两者是完全不同的东西。

**5. state[] 必须保留**

题目要求所有 C 的结果在 S 之后统一输出，不能即时 printf，
必须用 state[] 存储，最后统一输出。

**6. `char op` + `scanf` 读操作符**

操作符只有一个字符，`char op` + `scanf(" %c", &op)` 最简洁，
配合 `if/else` 分支处理三种操作，不需要字符串或其他复杂结构。

---

## 11. 复杂度总结

| 版本 | 总复杂度 |
|------|---------|
| 朴素 | O(N²) |
| Union-by-Size/Rank | O(N + M log N) |
| + 路径压缩 | O(M · α(M,N)) ≈ O(M) |

```
N-1 次 Union：O(N)        每次 O(1)，不是瓶颈
M   次 Find ：O(Mα)       路径压缩后均摊
总计         ：O(N + Mα)  M ≥ N 时 O(N) 被吸收，写作 O(Mα)
```
