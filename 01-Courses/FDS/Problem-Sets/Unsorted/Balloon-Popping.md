# Balloon popping

Balloon popping is a fun game for kids.  Now *n* balloons are positioned in a line. The goal of the game is very simple: to pop as many balloons as possible.  Here we add a special rule to this game -- that is, you can only make ONE jump.  Assume that a smart baby covers his/her body by thorns（刺）, jumps to some position and lies down (as shown by the figures below), so that the balloons will be popped as soon as they are touched by any part of the baby's body.  Now it is your job to tell the baby at which position he/she must jump to pop the most number of balloons.

![f1.jpg](https://images.ptausercontent.com/8ac137cc-1253-42d6-ba2f-3da93df331bc.jpg)

![f2.jpg](https://images.ptausercontent.com/7a0b9adf-ee5a-45ed-81e3-4ad970784ff2.jpg)

### Input Specification:

Each input file contains one test case. For each case, two positive integers are given in the first line: *n* (≤105), the number of balloons in a line, and *h* (≤103), the height of the baby with his/her arms stretched up.  Then *n* integers are given in the next line, each corresponds to the coordinate of a balloon on the axis of the line.  It is guaranteed that the coordinates are given in ascending order, and are all in the range [−106,106].

### Output Specification:

Output in a line the coordinate at which the baby shall jump, so that if the baby jumps at this position and then lie down, the maximum number of the balloons can be popped beneath his/her body.  Then also print the maximum number of balloons that can be popped.  If the coordinate is not unique, output the smallest one.

The numbers must be separated by 1 space, and there must be no extra space at the beginning or the end of the line.

---

## “滑动窗口” (Sliding Window)

抛弃模拟一毫米一毫米的移动策略而进行多重循环，我们不如使用**滑动窗口（双指针）,维护一个长度为 h 的窗口，让窗口的右边缘依次扫过每一个气球，看看窗口里最多能框住几个气球。**这样可以将时间复杂度大幅降低至 *O*(*N*)。

**核心逻辑：**

- ==为了打破尽可能多的气球，你的跳跃区间应该总是**恰好结束在某个气球的位置上**==。如果我们想覆盖一组以气球 *a*[*R*] 作为最后一个目标的气球，最优（也是最小）的跳跃起始位置 *X* 必然是 *a*[*R*]−*h*。
- 我们可以使用两个指针：`L`（左指针）和 `R`（右指针）。
- 我们让 `R` 在数组中向右移动。如果 `R` 处的气球和 `L` 处的气球之间的距离大于了我们的跳跃范围（*h*），说明一跳无法同时覆盖这两个气球，我们就把 `L` 向前（向右）移动，从而缩小窗口。
- 当前有效窗口内的气球数量就是 `R - L + 1`。

---

C

```c
#include <stdio.h>

// 全局数组，防止数据量太大撑爆栈内存
int a[100005]; 

int main() {
    int n, h;
    
    // 1. 读取输入
    if (scanf("%d %d", &n, &h) != 2) return 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    int L = 0;              // 滑动窗口左边界（气球下标）
    int max_balloons = 0;   // 记录打破的最大气球数
    int best_jump_pos = 0;  // 记录最佳起跳坐标

    // 2. 核心逻辑：滑动窗口 (O(N) 时间复杂度)
    // R 是滑动窗口右边界（我们假设这一跳的右边缘刚好压在第 R 个气球上）
    for (int R = 0; R < n; R++) {
        
        // 如果 L 和 R 之间的距离超过了人的身高 h
        // 说明无法同时覆盖它们，我们需要把左边界向右收缩
        while (a[R] - a[L] > h) {
            L++;
        }

        // 此时，从 L 到 R 的所有气球都可以被一跳覆盖
        int current_balloons = R - L + 1;
        
        // 为了刚好能覆盖到 a[R]，身体的最左端必须放在 a[R] - h 的位置
        int current_jump_pos = a[R] - h;

        // 3. 更新最大值
        // 这里必须用严格大于(>)。
        // 因为是从左向右遍历，如果遇到数量相等的平局情况，
        // 第一次遇到的天然就是坐标更小的（满足题目"output the smallest one"要求）
        if (current_balloons > max_balloons) {
            max_balloons = current_balloons;
            best_jump_pos = current_jump_pos;
        }
    }

    // 4. 按题目要求格式输出
    printf("%d %d\n", best_jump_pos, max_balloons);

    return 0;
}
```

## 为什么跳跃区间“恰好结束在某个气球上”是最佳策略？

**贪心思想 (Greedy Approach)**。我们可以通过一个简单的思想实验来证明：

假设你随意跳到了某个位置 *X*，你的身体覆盖的范围是 [*X*,*X*+*h*]。 在这个范围内，你压破了几个气球。我们关注你压破的**最右边**的那个气球，假设它的坐标是 *P*。 显然，*P*≤*X*+*h*。

**情况 A：区间右边缘超出了最右边的气球 (X + h  > P) **

这意味着你的头顶（区间的右边缘）和最右边的气球 *P* 之间有一段**空隙**。这段空隙没有压到任何气球，完全是浪费的。

- **右边：** 气球 *P* 依然被压着，你**没有损失**任何右侧的气球。
- **左边：** 因为你整体向左移动了，你的脚（左边缘）向左延伸了，你**有可能额外压到**原本在左侧够不到的气球。
- **结论：** 向左平移，气球总数**只增不减**。

同时，题目要求：**“如果坐标不唯一，输出最小的那个”**。把身体尽可能向左移动（直到右边缘贴住某个气球），天然就能得到更小的起跳坐标 *X*。因此，以某个气球作为区间的终点去反推起点，绝对是完美的最优解。