
> 记录在做 FDS Project 时遇到的真实工程问题，每个知识点来自实际 bug。

---

## 1. 栈溢出：大数组不要在栈上声明

### 问题描述

在函数内部声明大数组时程序直接崩溃，没有任何有意义的报错：

```c
void solve() {
    int dp[100000][100000]; // 崩溃！
}
```

### 根本原因

局部变量分配在**栈（stack）上。栈空间有严格的平台限制：

| 平台    | 默认栈大小 |
| ------- | ---------- |
| Windows | 1 MB       |
| Linux   | 8 MB       |

`int dp[100000][100000]` 需要约 40 GB，远超限制，操作系统直接触发栈溢出（stack overflow）终止程序。

### 正确做法

**方案一：全局变量**（适合固定大小、程序生命周期内一直需要的数据）

```c
int dp[1000][1000]; // 全局，分配在 BSS/数据段，无大小限制（受物理内存限制）

void solve() {
    // 直接使用 dp
}
```

**方案二：`malloc` 动态分配**（适合大小运行时才确定的情况）

```c
void solve(int n) {
    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) { /* 处理分配失败 */ return; }

    // 使用 arr...

    free(arr); // 必须手动释放
}
```

### 记忆口诀

> 函数内大数组 → 必崩。要么全局，要么 malloc。

---

## 2. 堆与栈的区别

### 内存布局

一个程序运行时，操作系统给它划分了几个区域：

```
高地址
┌─────────────────┐
│   栈 Stack      │  ← 向下增长，局部变量、函数帧
├─────────────────┤
│       ↓         │
│                 │
│       ↑         │
├─────────────────┤
│   堆 Heap       │  ← 向上增长，malloc 分配的内存
├─────────────────┤
│   BSS 段        │  ← 未初始化的全局/静态变量（自动清零）
├─────────────────┤
│   数据段         │  ← 已初始化的全局/静态变量
├─────────────────┤
│   代码段         │  ← 程序指令（只读）
低地址
```

- **栈从高地址向低地址增长**，每次函数调用压一帧，返回时弹出
- **堆从低地址向高地址增长**，`malloc` 向操作系统申请，`free` 归还

全局变量（无论是否初始化）不在栈也不在堆，而是在数据段/BSS段，程序启动时分配，结束时释放。

### 对比表

| 特性       | 栈（Stack）              | 堆（Heap）                   |
| ---------- | ------------------------ | ---------------------------- |
| 分配方式   | 自动（函数调用时）       | 手动（`malloc`/`calloc`）    |
| 释放方式   | 自动（函数返回时）       | 手动（必须 `free`）          |
| 大小限制   | 严格（Windows 1MB / Linux 8MB） | 宽松（受物理内存限制）  |
| 访问速度   | 快（CPU 缓存友好）       | 较慢（可能触发缺页中断）     |
| 生命周期   | 函数作用域内             | 从 `malloc` 到 `free`        |
| 碎片问题   | 无                       | 长期运行可能产生碎片         |
| 典型用途   | 局部变量、函数调用帧     | 大数组、动态结构、跨函数数据 |

### 栈帧是什么

每次函数调用，栈上会压入一个**栈帧（stack frame）**，包含：
- 函数的局部变量
- 函数的参数
- 返回地址（函数执行完后跳回哪里）
- 保存的寄存器值

```c
int add(int a, int b) {   // a, b 在 add 的栈帧里
    int result = a + b;   // result 也在栈帧里
    return result;        // 返回后栈帧弹出，result 销毁
}
```

递归调用时，每一层递归都有自己的栈帧，这也是为什么递归太深会栈溢出。

### 常见陷阱：返回局部变量的指针

```c
// 错误：返回了栈上变量的地址
int* bad() {
    int x = 42;
    return &x;  // 函数返回后，x 的栈帧被销毁，指针悬空
}

// 正确：返回堆上分配的内存（调用方负责 free）
int* good() {
    int *x = malloc(sizeof(int));
    *x = 42;
    return x;  // 堆上内存在 free 之前一直有效
}
```

### 示例：同一个值，三种存储位置

```c
int g = 10;          // 数据段，程序整个生命周期有效

void example() {
    int a = 5;           // 栈：函数返回后自动销毁
    int *b = malloc(4);  // 堆：必须手动 free(b)，否则内存泄漏
    *b = 5;
    free(b);
    // g 仍然有效，a 已销毁，b 指向的内存已释放
}
```

---

## 3. 全局变量副作用传导：用返回值替代 flag

### 问题描述

用全局 flag 在函数间通信，导致状态污染：

```c
int found = 0; // 全局 flag

void search(int *arr, int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            found = 1; // 修改全局状态
            return;
        }
    }
}

int main() {
    search(arr, n, 5);
    if (found) { ... }    // 看起来 OK
    search(arr, n, 6);    // 但如果 6 不存在，found 仍然是 1！
    if (found) { ... }    // 误判！
}
```

### 根本原因

全局 flag 是**隐式的输出通道**。调用方必须记住在每次调用前重置它，这是一个极易遗忘的隐患。函数调用越多、代码越长，出错概率越高。

### 正确做法

**用返回值显式传递结果**，让每次调用自包含：

```c
int search(int *arr, int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) return 1;
    }
    return 0;
}

int main() {
    if (search(arr, n, 5)) { ... } // 清晰，无副作用
    if (search(arr, n, 6)) { ... } // 独立，不受上次调用影响
}
```

### 原则

> 函数应通过**返回值**输出结果，而不是修改全局状态。全局变量只用于真正需要全局共享的配置或数据。

---

## 4. 隐式类型截断：`long long` 传给 `int` 形参

### 问题描述

函数签名用 `int`，但调用时传入 `long long`，数值被静默截断：

```c
void process(int x) {
    printf("%d\n", x);
}

int main() {
    long long big = 3000000000LL; // 超过 int 最大值 2147483647
    process(big);                 // 截断！输出 -1294967296（未定义行为）
}
```

### 根本原因

`int` 通常为 32 位，最大值约 2.1×10⁹。`long long` 为 64 位。传参时编译器直接截取低 32 位，高位丢失，结果完全错误且**不报运行时错误**。

### 正确做法

**1. 函数签名与实际数据类型匹配：**

```c
void process(long long x) {
    printf("%lld\n", x);
}
```

**2. 开启编译器警告，让问题在编译期暴露：**

```bash
gcc -Wall -Wextra -o prog prog.c
# warning: implicit conversion loses integer precision: 'long long' to 'int'
```

**3. 养成习惯：** 涉及计数、索引、乘积时优先用 `long long`，避免在边界情况溢出。

### 记忆口诀

> 编译加 `-Wall`，截断无处藏。

---

## 5. 内存管理：释放指针数组的正确顺序

### 问题描述

有一个 `char **words` 指针数组，释放时操作顺序错误导致内存泄漏或 double-free：

```c
// 错误：先释放外层，内层指针丢失，内存泄漏
free(words);
for (int i = 0; i < n; i++) free(words[i]); // 未定义行为：words 已被释放
```

### 根本原因

`words` 是一个指针数组，内存结构如下：

```
words ──► [ ptr0 | ptr1 | ptr2 | ... ]
              │      │      │
              ▼      ▼      ▼
           "hello" "world" "foo"   ← 各自独立的堆内存块
```

`free(words)` 只释放"指针数组本身"，并不释放每个指针指向的字符串。必须先逐个释放字符串，再释放数组。

### 正确做法

**先释放内容，再释放容器：**

```c
// 正确：由内到外
for (int i = 0; i < n; i++) {
    free(words[i]); // 释放每个字符串
    words[i] = NULL; // 防止悬空指针
}
free(words);        // 最后释放指针数组
words = NULL;
```

### 原则

> 释放顺序与分配顺序**相反**。先分配的后释放，先释放内层（叶节点），再释放外层（根节点）。

---

## 6. 双指针去重：跳过重复值的正确写法

### 问题描述

在已排序数组中用双指针找配对（如两数之和），找到答案后需要跳过所有相同值，但写法有误导致漏跳或越界：

```c
// 错误：只移动一步，相同值仍会被重复处理
if (arr[l] + arr[r] == target) {
    // 记录结果
    l++;
    r--;
    // 如果 arr[l] == arr[l-1]，下次循环还会重复找到相同配对
}
```

### 根本原因

排序数组中相同值聚集在一起。找到配对后仅移动一步，指针仍停在重复值上，下次迭代会产生重复答案。

### 正确做法

找到配对后，用 `while` 循环跳过所有相同值：

```c
int l = 0, r = n - 1;
while (l < r) {
    int sum = arr[l] + arr[r];
    if (sum == target) {
        printf("(%d, %d)\n", arr[l], arr[r]);

        int left_val = arr[l], right_val = arr[r];
        // 跳过左侧所有重复值
        while (l < r && arr[l] == left_val) l++;
        // 跳过右侧所有重复值
        while (l < r && arr[r] == right_val) r--;

    } else if (sum < target) {
        l++;
    } else {
        r--;
    }
}
```

### 关键细节

- `while` 条件中必须保持 `l < r`，防止越界
- 先记录当前值再移动，不要在移动后比较（否则多移一步）
- 同样的去重逻辑适用于三数之和（3Sum）等变体

---

## 7. 栈模拟中序遍历 vs 预展开数组

> 背景：对两棵 BST 做双指针"两数之和"，需要从两棵树各取有序元素。

### 方案对比

| 维度           | 方案 A：预展开数组                  | 方案 B：栈迭代器                        |
| -------------- | ----------------------------------- | --------------------------------------- |
| 空间复杂度     | O(N1 + N2)，额外数组                | O(h1 + h2)，仅栈深度                    |
| 不平衡树表现   | 不受影响                            | 退化为链表时栈深度 O(N)                 |
| 实现复杂度     | 简单，正确性易保证                  | 较复杂，需理解显式栈语义                |
| 适合场景       | N 不大、追求简洁正确                | N 极大、内存敏感、或只需前 K 个结果     |
| 惰性求值       | 否（必须先遍历完整棵树）            | 是（按需取下一个节点）                  |

---

### 方案 A：预先展开为有序数组

递归中序遍历，结果填入数组，再对两个数组做双指针夹逼：

```c
void inorder(Node **h, int root, long long *arr, int *idx) {
    if (root == -1) return;
    inorder(h, h[root]->lc, arr, idx);
    arr[(*idx)++] = h[root]->ele;
    inorder(h, h[root]->rc, arr, idx);
}

// 调用方
int idx1 = 0, idx2 = 0;
inorder(h1, root1, arr1, &idx1);  // 展开树1
inorder(h2, root2, arr2, &idx2);  // 展开树2

int lo = 0, hi = idx2 - 1;
while (lo < idx1 && hi >= 0) {
    long long sum = arr1[lo] + arr2[hi];
    if (sum == tar)     { /* 输出 */; lo++; hi--; }
    else if (sum < tar) { lo++; }
    else                { hi--; }
}
```

**优点**：逻辑清晰，调试方便。**缺点**：必须先完整遍历两棵树，空间 O(N1+N2)。

---

### 方案 B：栈迭代器（惰性求值）

#### 本质

把递归的**隐式调用栈**变成**显式栈**，每次只走到下一个节点就暂停，实现惰性求值。

- `iter_next`：模拟正向中序（左→根→右），每次返回当前最小值
- `iter_prev`：模拟反向中序（右→根→左），每次返回当前最大值

#### 完整实现

```c
typedef struct {
    int stk[2000];
    int top;
    Node **h;
    int cur;  // 当前待下潜的节点，-1 表示已耗尽
} Iterator;

void iter_init(Iterator *it, Node **h, int root) {
    it->top = 0;
    it->h   = h;
    it->cur = root;
}

// 取下一个最小值（正向中序）
long long iter_next(Iterator *it) {
    // 一直沿左子树下潜，全部压栈
    while (it->cur != -1) {
        it->stk[it->top++] = it->cur;
        it->cur = it->h[it->cur]->lc;
    }
    // 弹出栈顶 = 当前最小节点
    int node    = it->stk[--it->top];
    long long val = it->h[node]->ele;
    // 下次从右子树继续
    it->cur = it->h[node]->rc;
    return val;
}

// 取下一个最大值（反向中序）
long long iter_prev(Iterator *it) {
    // 一直沿右子树下潜，全部压栈
    while (it->cur != -1) {
        it->stk[it->top++] = it->cur;
        it->cur = it->h[it->cur]->rc;
    }
    // 弹出栈顶 = 当前最大节点
    int node    = it->stk[--it->top];
    long long val = it->h[node]->ele;
    // 下次从左子树继续
    it->cur = it->h[node]->lc;
    return val;
}

// 判断迭代器是否还有元素（栈非空 或 cur 未耗尽）
int iter_has_next(Iterator *it) {
    return it->top > 0 || it->cur != -1;
}
```

#### 双指针主逻辑

```c
Iterator it1, it2;
iter_init(&it1, h1, root1);  // 从树1最小值出发
iter_init(&it2, h2, root2);  // 从树2最大值出发

long long A = iter_next(&it1);
long long B = iter_prev(&it2);

while (iter_has_next(&it1) || /* it1已取出A */ iter_has_next(&it2) || /* it2已取出B */ 1) {
    long long sum = A + B;
    if (sum == tar) {
        printf("%lld %lld\n", A, B);
        if (!iter_has_next(&it1) || !iter_has_next(&it2)) break;
        A = iter_next(&it1);
        B = iter_prev(&it2);
    } else if (sum < tar) {
        if (!iter_has_next(&it1)) break;
        A = iter_next(&it1);
    } else {
        if (!iter_has_next(&it2)) break;
        B = iter_prev(&it2);
    }
}
```

> **注意**：终止条件需要同时考虑"迭代器栈是否耗尽"，上面用 `iter_has_next` 在移动前检查，防止对空迭代器调用 `iter_next`/`iter_prev`（会导致栈下溢）。

---

### 关键细节

- `iter_next` 中 `while` 下潜 + 弹栈 + 转右子树，三步缺一不可
- `iter_prev` 是镜像：下潜改为右子树，转移改为左子树
- 两个迭代器可以分别指向不同的树，互不干扰
- 方案 B 若只需要找到第一对配对就停止，可以节省大量遍历——方案 A 做不到这一点

---

## 总结

| # | 问题               | 一句话记忆                              |
|---|--------------------|-----------------------------------------|
| 1 | 栈溢出             | 大数组不上栈，全局或 malloc             |
| 2 | 堆栈区别           | 栈自动快小，堆手动慢大                  |
| 3 | 全局 flag          | 用返回值，不用全局状态通信              |
| 4 | 类型截断           | 开 `-Wall`，用 `long long` 防溢出       |
| 5 | 释放顺序           | 先释放内容，再释放容器                  |
| 6 | 双指针去重         | while 跳重复，条件保持 l < r            |
| 7 | 中序遍历方案选择   | 简单用展开数组，内存敏感用栈迭代器      |
