# 双栈实现+_malloc分配堆+流式输出

## 来源

原文件：`双栈实现+_malloc分配堆+流式输出.c`

## 用途

TODO：补充这段代码对应的题目或功能。

## 核心思路

TODO：补充算法思路。

## C 代码

```c
#include <stdio.h>
#include <stdlib.h> // 引入 malloc 和 free

// 使用 typedef 简化类型名称，这是专家级 C 代码的标配
typedef struct {
    int *element; // 这是一个没有长度的指针，等待后续分配动态内存
    int top;
} Stack;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    Stack s1, s2;
    s1.top = -1;
    s2.top = -1;

    // 1. 动态内存分配 (Dynamic Memory Allocation)
    // 从堆内存申请 N 个 int 的空间。堆内存极大，绝不爆栈。
    s1.element = (int *)malloc(n * sizeof(int));
    s2.element = (int *)malloc(n * sizeof(int));

    // 强制防线：检查内存是否真的分配成功
    if (s1.element == NULL || s2.element == NULL) {
        printf("Fatal Error: 内存分配失败！\n");
        return -1;
    }

    char op;
    int val;
    int count = 0;

    // ==========================================
    // 流式处理 (Online Processing)
    // 读一个，做一个，极致节约缓存 (Cache) 资源
    // ==========================================
    for (int i = 0; i < n; i++) {
        scanf(" %c", &op);

        if (op == 'I') {
            scanf("%d", &val);
            s1.element[++s1.top] = val; // 接收新货
        } 
        else if (op == 'O') {
            count = 0; // 重置代价统计器
            
            if (s2.top == -1) {
                if (s1.top == -1) {
                    printf("ERROR\n");
                    continue; 
                }
                while (s1.top > -1) {
                    s2.element[++s2.top] = s1.element[s1.top--];
                    count += 2;
                }
            }
            
            count++;
            // 直接向标准输出流写入，底层由系统缓冲区代劳
            printf("%d %d\n", s2.element[s2.top--], count);
        }
    }

    // 2. 内存回收 (Memory Deallocation)
    // C 语言没有垃圾回收机制，谁借的内存谁负责还，这是铁律。
    free(s1.element);
    free(s2.element);

    return 0;
}
```
