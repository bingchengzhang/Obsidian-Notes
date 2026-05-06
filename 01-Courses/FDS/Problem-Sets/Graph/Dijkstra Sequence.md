![[Dijkstra Sequence(pr3) 1.png]]


```c#
#include <stdio.h>
#include <stdlib.h>

#define MAXV     1001              /* vertices numbered from 1 to Nv */
#define MAXE     100001            /* upper bound on input edges */
#define MAXK     101               /* upper bound on number of queries */
#define MAXH     (2 * MAXE + 10)   /* lazy heap may keep stale records; bounded by total relaxations */
#define INFINITY 0x3f3f3f3f        /* a large enough distance, safe under addition */

/* one item in an adjacency list */
struct EdgeNode {
    int neighbor;          /* endpoint */
    int weight;            /* edge length */
    struct EdgeNode *next; /* next edge from the same source vertex */
};

/* graph[u] is the head of the edge list starting from u */
struct EdgeNode *graph[MAXV];
int Nv, Ne;                /* graph size from input */

/* heap record: tentative distance and vertex number */
struct HeapNode {
    int dist;              /* heap key */
    int vertex;            /* vertex number */
};

/* 1-based min heap */
struct HeapNode heap[MAXH];
int heapSize;              /* current heap length */

/* add directed edge u -> v to the adjacency list (head insertion) */
void addEdge(int u, int v, int w)
{
    struct EdgeNode *node = malloc(sizeof(struct EdgeNode));
    node->neighbor = v;
    node->weight   = w;
    node->next     = graph[u];
    graph[u]       = node;
}

/* insert one (dist, vertex) record into the heap and sift up */
void push(int dist, int vertex)
{
    struct HeapNode item = {dist, vertex};
    int i = ++heapSize;

    /* move parents down until the right place is found */
    while (i > 1 && item.dist < heap[i / 2].dist) {
        heap[i] = heap[i / 2];
        i /= 2;
    }
    heap[i] = item;
}

/* remove and return the smallest record; sift the last element down */
struct HeapNode popMin(void)
{
    struct HeapNode top  = heap[1];
    struct HeapNode last = heap[heapSize--];
    int i = 1;

    while (i * 2 <= heapSize) {
        int child = i * 2;
    
        /* pick the smaller child */
        if (child + 1 <= heapSize && heap[child + 1].dist < heap[child].dist)
            child++;
    
        if (last.dist <= heap[child].dist)
            break;
    
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = last;
    return top;
}

/*
 * Return 1 if seq can be produced by some legal run of Dijkstra
 * starting at seq[0], otherwise 0.
 *
 * Lazy deletion: stale entries for already-known vertices are skipped
 * at the heap top instead of being removed eagerly.
 */
 int check(int seq[])
 {
    int dist[MAXV];        /* tentative distance */
    int known[MAXV];       /* 1 means already finalized */

    for (int i = 1; i <= Nv; i++) {
        dist[i]  = INFINITY;
        known[i] = 0;
    }

    int src = seq[0];      /* the source vertex */
    dist[src] = 0;
    heapSize = 0;
    push(0, src);

    for (int i = 0; i < Nv; i++) {
        int v = seq[i];    /* the vertex the sequence claims to finalize next */

        /* discard stale entries belonging to already-finalized vertices */
        while (heapSize > 0 && known[heap[1].vertex])
            popMin();      /* return value intentionally discarded */
     
        if (heapSize == 0)
            return 0;      /* should not happen on connected input with valid order */
     
        /*
         * Validity check: dist[v] must equal the current global minimum
         * among unknown vertices. Equal distances are tolerated since
         * Dijkstra may break ties arbitrarily.
         */
        if (dist[v] != heap[1].dist)
            return 0;
     
        known[v] = 1;      /* v's stale heap entries will be lazily skipped later */
     
        /* relax all unfinished neighbors of v */
        for (struct EdgeNode *p = graph[v]; p; p = p->next) {
            int w = p->neighbor;
            if (!known[w] && dist[v] + p->weight < dist[w]) {
                dist[w] = dist[v] + p->weight;
                push(dist[w], w);   /* new record; old one becomes stale */
            }
        }
    }
    return 1;
 }

int main(void)
{
    scanf("%d %d", &Nv, &Ne);

    /* the input graph is undirected: add both directions */
    for (int i = 0; i < Ne; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        addEdge(u, v, w);
        addEdge(v, u, w);
    }
    
    int K;
    scanf("%d", &K);
    
    int seq[MAXV];
    while (K--) {
        for (int i = 0; i < Nv; i++)
            scanf("%d", &seq[i]);
        printf(check(seq) ? "Yes\n" : "No\n");
    }
    
    return 0;
}

```

