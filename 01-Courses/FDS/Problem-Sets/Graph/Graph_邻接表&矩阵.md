# 有向-无向图&邻接表-矩阵

## 一、连通性

无向图的"连通"很直接：任意两点之间存在路径，整张图就是连通图。这里的路径不关心方向，因为无向边本来就双向可走。

有向图的连通分两层：

- **强连通**：任意两点 u、v 都有 u→v 的有向路径，同时也有 v→u 的有向路径。
- **弱连通**：把所有边的方向去掉之后，得到的无向图是连通的。

弱连通只保证"形状上连着"，不保证从任意 source 出发能到达所有节点。例子：1→2→3，弱连通但不强连通，从 3 出发哪也去不了。

这件事对 Dijkstra 的影响：

- 无向连通图：从任意 source 出发，所有节点都可达，dist 不会有 INF。
- 有向弱连通图：从 source 出发可能有节点不可达，dist 永远是 INF。
- 有向强连通图：和无向连通图一样，所有节点可达。

Dijkstra 算法本身不区分有向无向，它只问"从当前节点能到哪些邻居"——邻接表（或矩阵）回答这个问题，方向由建图时决定。

## 二、邻接表

每个节点维护一个链表，链表里存"从这个节点能直接到达的邻居"以及边权。

```c
struct EdgeNode {
    int neighbor;
    int weight;
    struct EdgeNode *next;
};
struct EdgeNode *graph[MAXV];   /* graph[u] 指向 u 的邻接链表头 */
```

### 有向图建图

每条边 u→v 只插入一次，插到 u 的链表里：

```c
void addEdge(int u, int v, int w) {
    struct EdgeNode *node = malloc(sizeof(struct EdgeNode));
    node->neighbor = v;
    node->weight   = w;
    node->next     = graph[u];
    graph[u]       = node;
}

/* 主程序里 */
for (int i = 0; i < Ne; i++) {
    int u, v, w;
    scanf("%d %d %d", &u, &v, &w);
    addEdge(u, v, w);
}
```

### 无向图建图

每条无向边拆成两条方向相反的有向边，双向插入：

```c
for (int i = 0; i < Ne; i++) {
    int u, v, w;
    scanf("%d %d %d", &u, &v, &w);
    addEdge(u, v, w);
    addEdge(v, u, w);   /* 反向 */
}
```

`addEdge` 函数本身和有向图一模一样，区别只在主程序调用几次。

### 遍历邻居

不管有向无向，遍历方式相同：

```c
for (struct EdgeNode *p = graph[v]; p; p = p->next) {
    int w = p->neighbor;
    /* w 是 v 的一个邻居，边权 p->weight */
}
```

无向图建图时已经把"双向"编码进邻接表了，遍历的时候不需要再判断方向。

## 三、邻接矩阵

二维数组 `int adj[MAXV][MAXV]`，`adj[u][v]` 存 u 到 v 的边权（或 INF 表示没有边）。

### 初始化

```c
for (int i = 1; i <= Nv; i++)
    for (int j = 1; j <= Nv; j++)
        adj[i][j] = (i == j) ? 0 : INFINITY;
```

### 有向图建图

```c
for (int i = 0; i < Ne; i++) {
    int u, v, w;
    scanf("%d %d %d", &u, &v, &w);
    adj[u][v] = w;          /* 只设一个方向 */
}
```

### 无向图建图

```c
for (int i = 0; i < Ne; i++) {
    int u, v, w;
    scanf("%d %d %d", &u, &v, &w);
    adj[u][v] = w;
    adj[v][u] = w;          /* 矩阵关于主对角线对称 */
}
```

无向图的邻接矩阵是对称的，有向图一般不对称。

### 遍历邻居

```c
for (int w = 1; w <= Nv; w++) {
    if (adj[v][w] != INFINITY) {
        /* w 是 v 的邻居，边权 adj[v][w] */
    }
}
```

不管有没有边都要扫一遍 Nv 个位置，所以邻接矩阵遍历邻居是 O(Nv)，邻接表是 O(deg(v))。

## 四、表 vs 矩阵的取舍

|             | 邻接表           | 邻接矩阵           |
|-------------|------------------|--------------------|
| 空间        | O(Nv + Ne)       | O(Nv²)             |
| 查"u-v 是否相邻" | O(deg(u))    | O(1)               |
| 遍历 v 的邻居 | O(deg(v))      | O(Nv)              |
| 适合        | 稀疏图（Ne 远小于 Nv²） | 稠密图、需频繁查边 |

Dijkstra 用邻接表配堆是 O((Nv + Ne) log Nv)，用邻接矩阵配线性扫描是 O(Nv²)。前者适合稀疏图，后者在 Ne 接近 Nv² 时反而更优。

## 五、回到一个常见误解

"Dijkstra 跑无向图需要双向遍历来保证完整性吗？"

不需要。无向图的"双向"在 `addEdge` 双向插入那一步已经编码完了。算法层面没有方向的概念，每次松弛只看"当前节点的邻接表里有谁"，邻接表回答这个问题。Dijkstra 的本质是按距离从小到大向外扩散，在已知连通关系上传递距离信息——和方向无关，和邻接关系有关。
