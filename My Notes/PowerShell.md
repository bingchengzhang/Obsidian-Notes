## 1. 文件与目录操作

| **命令**         | **用法**                   | **对应 CMD/大白话**          |
| -------------- | ------------------------ | ----------------------- |
| `ls` (或 `dir`) | `ls`                     | 查看当前目录下有哪些文件            |
| `cd`           | `cd D:\lazy`             | 切换文件夹（PS 切换盘符不需要加 `/d`） |
| `pwd`          | `pwd`                    | 显示你当前到底在哪个路径（防迷路）       |
| `mkdir`        | `mkdir lab1`             | 新建一个名为 `lab1` 的文件夹      |
| `rm`           | `rm test.txt`            | 删除文件（**慎用**，不进回收站）      |
| `cp`           | `cp file.txt D:\backup\` | 复制文件                    |
| `mv`           | `mv old.txt new.txt`     | 移动文件或**重命名**            |
|                |                          |                         |
## 2. 内容查看与处理（查课件/查代码）

- **`cat`**：直接在终端里打印文件内容。
    
    - _场景_：想快速瞄一眼代码或者 `config.yaml`。
        
    - `cat config.yaml`
        
- **`echo`**：在屏幕上打印一段话，或者写入文件。
    
    - `echo "hello" > test.txt` (覆盖写入)
        
    - `echo "new line" >> test.txt` (追加写入)
        
- **`grep` (PS 里叫 `Select-String`)**：搜索文件里的关键字。
    
    - _场景_：在几百个课件名里找包含“离散”的文件。
        
    - `ls | Select-String "离散"`
        

---

## 3. 系统与网络（排查 Cookie 失效）

- **`curl` (或 `iwr`)**：发送网络请求。
    
    - _场景_：测试你的网络能不能连上浙大内网。
        
    - `curl [https://course.zju.edu.cn](https://course.zju.edu.cn)`
        
- **`ps` (Get-Process)**：查看当前运行的程序。
    
    - _场景_：`lazytui` 卡死关不掉时。
        
    - `ps | Select-String "lazy"`
        
- **`kill` (Stop-Process)**：强制结束进程。
    
    - `kill -Name lazytui`
        

---

## 4. 只有 PowerShell 才有的小妙招

- **`start .`**：在当前位置打开 Windows 资源管理器窗口（鼠标点点点爱好者的救星）。
    
- **`code .`**：用 VS Code 打开当前整个文件夹（前提是装了 VS Code）。
    
- **`$PROFILE`**：你的“魔法书”。
    
    - 输入 `notepad $PROFILE` 就能添加你喜欢的快捷命令（比如之前的 `zju`）。
        
- **Tab 键自动补全**：
    
    - 输入 `cd D:\la` 然后按 **Tab**，它会自动补全成 `cd D:\lazy\`。**这是防止敲错命令的最强手段。**
        

---

## 5. “管道符” `|`

这是 PowerShell 最强大的地方。它能把前一个命令的结果传给后一个。

- **例子**：查看当前下载文件夹里，所有包含 "Assignment" 的文件。
    
    - `ls D:\lazy\downloads | Select-String "Assignment"`