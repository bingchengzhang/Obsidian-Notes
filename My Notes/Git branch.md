[Learn Git Branching]([Learn Git Branching](https://learngitbranching.js.org/?demo=&locale=zh_CN))
## 创建与切换 ##
```
git commit 创建新分支

git checkout <name> 转换当前所在分支

git checkout -b <name>
-b:表示"branch",意即切换(checkout)的同时创建分支

git switch -c <name>
-c:表示"create"创建并切换

git branch <新分支名>
```

## 分支与合并 ##

在 Git 的世界里，`git merge`（合并）就像是**将两条流动的河流汇聚到一起**。它的核心作用是将一个分支的修改集成到另一个分支中。

通常的操作流程是：**先切换到接收修改的分支（通常是 `main`），然后合并目标分支。**

Bash

```
git checkout main    # 1. 切换到主分支
git merge bugFix     # 2. 将 bugFix 分支的内容合并进来
```

---
## 合并的两种主要模式

Git 会根据分支的历史结构，自动选择不同的合并方式：

#### A. 快进合并 (Fast-Forward)

如果 `main` 分支在创建 `bugFix` 之后没有任何新的提交，Git 只需简单地将 `main` 指针**向前移动**到 `bugFix` 的位置即可。

===向前移动=== Fast-Forward 理解为进度条的快进

- **特点：** 不产生新的提交记录，历史线是一条直线。
    

#### B. 三方合并 (Three-way Merge / Recursive)

如果 `main` 和 `bugFix` 各自都有了新的提交，Git 会寻找两者的**公共祖先**，并将两边的改动整合，自动创建一个新的**“合并提交”（Merge Commit）**。

- **特点：** 会产生一个拥有两个“父亲”的新节点，历史线会出现分叉再合拢的形状。
---

- `git merge`

```
在 Git 中合并两个分支时会产生一个特殊的提交记录，它有两个 parent 节点。翻译成自然语言相当于：“我要把这两个 parent 节点本身及它们所有的祖先都包含进来。”

eg:git merge 被合并节点
即将被合并节点并入HEAD所在节点
```


- `git rebase`
```
Rebase 实际上就是取出一系列的提交记录，“复制”它们，然后在另外一个地方逐个的放下去
线性化提交记录

eg:git rebase 目标节点位置
```

## 如何工作的？

当你执行 `git rebase main` 时，Git 会做以下三件事：

1. **暂存：** 把你当前分支（比如 `bugFix`）上所有独有的提交“拆下来”，临时存放在一个地方。
    
2. **移动：** 把你的当前分支指针直接移动到目标分支（`main`）的最远端。——即为bugfix暂时和main指向同一个位置
    
3. **重放：** 把刚才拆下来的提交，一个接一个地重新“接”在 `main` 的后面。
	 **他们是全新的提交节点**

## 执行结果 ##
- **`main` 的位置：** 纹丝不动，依然停留在它原来的那个“最远端”节点。
    
- **`bugFix` 的位置：** 移动到了 `main` 的**上方**。它现在包含了 `main` 的所有历史，并且在 `main` 的基础上多出了几个它自己的新提交。
    
- **视觉效果：** 在 _Learn Git Branching_ 里，你会看到 `main` 标签在一个圆圈上，而 `bugFix` 标签在它上面的几个圆圈上。它们现在排成了一条直线。

![[屏幕截图 2026-03-02 205542 1.png]]