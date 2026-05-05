#include <stdio.h>

// 定义常量，略大于 10^6 以防止边界溢出
#define MAX_N 1000005 

// 1. 全局变量区：这里的变量占用全局静态内存，不会引发 Stack Overflow
struct Stack {
    int element[MAX_N];
    int top;
};

struct Stack s1, s2;

// 用全局数组充当“输入缓冲区”，满足你“全部读完”的要求
char op_list[MAX_N];
int val_list[MAX_N]; 

int main() {
    int n;
    // 防御性读取，防止没有输入直接导致死循环
    if (scanf("%d", &n) != 1) return 0; 

    // ==========================================
    // 第一阶段：离线读取 (Offline Reading)
    // 严格满足“全读完再处理”的硬性要求
    // ==========================================
    for (int i = 0; i < n; i++) {
        // " %c" 前面的空格极其重要，用于吃掉上一行的换行符
        scanf(" %c", &op_list[i]);
        if (op_list[i] == 'I') {
            scanf("%d", &val_list[i]);
        }
    }

    // 初始化栈指针
    s1.top = -1;
    s2.top = -1;
    int count = 0; // 用于计算底层操作代价

    // ==========================================
    // 第二阶段：全量处理与输出 (Processing & Output)
    // ==========================================
    for (int i = 0; i < n; i++) {
        if (op_list[i] == 'I') {
            // 入队操作：直接压入 s1，无需代价统计
            s1.element[++s1.top] = val_list[i];
        } 
        else if (op_list[i] == 'O') {
            count = 0; // 每次出队前，重置计费器
            
            // 核心判定：s2 空了，且 s1 有货，才允许去 s1 进货
            if (s2.top == -1) {
                if (s1.top == -1) {
                    printf("ERROR\n");
                    continue; // 双空状态，直接跳过本次操作
                }
                
                // 物理搬运：利用两次 LIFO 实现时间倒流
                while (s1.top > -1) {
                    s2.element[++s2.top] = s1.element[s1.top--];
                    count += 2; // 搬运一次，包含 Pop 和 Push，代价记为 2
                }
            }
            
            // 此时 s2 栈顶绝对是队列中“最老”的有效数据
            count++; // 执行真正的出队交付，代价记为 1
            
            // 打印并剥夺旧数据的访问权（唯一合法的 s2.top-- 位置）
            printf("%d %d\n", s2.element[s2.top--], count);
        }
    }

    return 0;
}