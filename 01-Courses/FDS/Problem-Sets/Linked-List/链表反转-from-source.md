反转链表

结论先行：链表反转的底层逻辑从来不是数据的乾坤大挪移，而是**引线（指针）在堆内存中的物理重定向**。针对输入输出的边界契约（是否有假头），工程上有两套绝对的标准范式：“三指针原地调换”与“假头锚定头插法”。

直觉判断：只要你把“假头（Dummy）”看作一个绝对静止的物理锚点，把“游标（curr/p）”看作外出执行拆迁任务的侦察兵，所有的反转变体都万变不离其宗。

以下是针对四种边界契约的终极代码实现矩阵：

------

## 一、 链表反转的全场景代码矩阵

#### 场景 1：输入无假头 $\rightarrow$ 输出无假头（最经典的 LeetCode 标准）

- **战术**：三指针兵团（`prev`, `curr`, `next_temp`）贴身肉搏，原地调转指针方向。
- **物理防线**：初始化 `prev = NULL`，确立反转后的绝对终点。

C

```c
struct Node* Reverse_No_No(struct Node* L) {
    if (L == NULL || L->Next == NULL) return L;

    struct Node *prev = NULL;  // 充当新链表的尾部（指向虚无）
    struct Node *curr = L;
    struct Node *next_temp = NULL;

    while (curr != NULL) {
        next_temp = curr->Next; // 1. 探路保底
        curr->Next = prev;      // 2. 物理回头
        prev = curr;            // 3. 战线推进
        curr = next_temp;
    }
    return prev; // prev 最终停留在原链表的尾节点，即新链表的头
}
```

#### 场景 2：输入有假头 $\rightarrow$ 输出有假头（PTA/考研最高频）

- **战术**：复用输入假头，将后面的有效节点逐一“剥离”并“头插”到假头正后方。
- **物理防线**：强制 `L->Next = NULL` 斩断旧世界，防止自循环。

C

```c
struct Node* Reverse_Yes_Yes(struct Node* L) {
    if (L == NULL || L->Next == NULL) return L;

    struct Node *p = L->Next; // p 指向第一个真实数据
    L->Next = NULL;           // 斩断假头与旧队伍的联系，确立终点！

    struct Node *temp = NULL;
    while (p != NULL) {
        temp = p->Next;       
        p->Next = L->Next;    // 新节点接管已反转的队伍
        L->Next = p;          // 假头重新锚定新节点
        p = temp;
    }
    return L;
}
```

#### 场景 3：输入无假头 $\rightarrow$ 输出有假头

- **战术**：强行 `malloc` 一个假头作为锚点，然后对原链表执行头插法。

C

```c#
struct Node* Reverse_No_Yes(struct Node* L) {
    struct Node *dummy = (struct Node*)malloc(sizeof(struct Node));
    dummy->Next = NULL; // 创世锚点

    struct Node *curr = L;
    struct Node *temp = NULL;

    while (curr != NULL) {
        temp = curr->Next;
        curr->Next = dummy->Next;
        dummy->Next = curr;
        curr = temp;
    }
    return dummy; 
}
```

#### 场景 4：输入有假头 $\rightarrow$ 输出无假头

- **战术**：越过假头，对后续链表执行标准三指针反转。为了防止内存泄漏，最后必须超度（`free`）掉那个失去利用价值的假头。

C

```c
struct Node* Reverse_Yes_No(struct Node* L) {
    if (L == NULL || L->Next == NULL) {
        if (L) free(L); // 释放假头
        return NULL;
    }

    struct Node *prev = NULL;
    struct Node *curr = L->Next; // 越过假头
    struct Node *next_temp = NULL;

    while (curr != NULL) {
        next_temp = curr->Next;
        curr->Next = prev;
        prev = curr;
        curr = next_temp;
    }
    
    free(L); // 物理超度旧假头
    return prev;
}
```

------

## 二、 对话精粹与底层认知重构

回顾刚才我们步步紧逼的排查过程，你对 C 语言指针的理解已经完成了从“语法层”到“物理内存层”的跨越。以下是四个核心的认知重构点：

1. **假头（Dummy）的物理实体**：

   假头绝不是 `NULL`。它是你在堆内存中花真金白银（`malloc`）盖起的一座“空房子”。它的存在价值在于消除边界特判（`if L == NULL`），为所有真实的业务节点提供一个永远存在的**前驱锚点**。

2. **左值的物理穿透力**：

   `p->Next = L->Next` 绝不是单纯的变量赋值，它是直接对堆内存中物理连接的重构。`p` 是你手里的遥控器，按下去，物理世界里的水管就改变了流向。这种操作必然会影响到从 `L` 遍历所能看到的全局视图。

3. **斩断旧世界（防自环铁律）**：

   无论使用哪种反转方法，都必须在初始阶段显式地切断链表原有的延续（如 `L->Next = NULL` 或 `prev = NULL`）。如果不确立这个物理终点，链表的头部节点就会无可救药地指向自己，在内存中形成黑洞般的死循环。

4. **探路先锋的绝对优先级**：

   在打断任何一根 `Next` 指针之前，强制要求使用 `temp = p->Next` 购买“保险”。在指针重定向的危险地带，必须“吃着碗里的，看着锅里的”，否则链表瞬间断裂，内存永久泄漏。

我个人认为，能把这几个底层逻辑盘清楚，你在数据结构的指针操控上已经没有盲区了。

需要我为你提供几道专门针对这些“边界契约”和“内存陷阱”的自测选择题，来验证一下肌肉记忆吗？