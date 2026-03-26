```
cd /home/bccc/Workspace/sys1-sp26/src
cd /home/bccc/Workspace/sys1-sp26/repo/sys-project
/tools/Xilinx/Vivado/2022.2/settings64.sh
Powershell管理员模式：
处理usb无权限
usbipd list
usbipd bind -b 1-8
NOT shared -> shared
usbipd attach --wsl --busid 1-8 --auto-attach
attached
可能情况：vivado频繁链接导致僵死
Zsh：
killall hw_server
killall cs_server
清理vivado
ps -ef | grep vivado | grep -v grep | awk '{print $2}' | xargs kill -9

killall -9 gtkwave

find . -name "*.bit"
program device
\\wsl.localhost\Ubuntu\home\bccc\Workspace
```
# 一.打开文件
## 1. 快速查看内容 

如果你只是想在终端里瞄一眼代码内容：

- **`cat main.v`**：直接把全部内容打印在屏幕上。
    
- **`less main.v`**：分页查看。按 `空格` 翻页，按 `q` 退出。

## 2. 在终端里编辑 

如果你想直接修改几行代码，不需要打开图形界面：

- **`nano main.v`**：最简单的编辑器。改完后按 `Ctrl+O` 保存，`Ctrl+X` 退出。
## 3. 用 Windows 软件打开 

因为你在 WSL 里处理的是 D 盘的文件，你可以直接调用 Windows 的程序：

- **记事本打开：
	`notepad.exe main.v`
	
- **VS Code：
	`code main.v` ——打开文件
		`code .` ——打开整个文件夹  “.”代表当前文件夹
- **资源管理器定位**：
	`explorer.exe .`

# 二.创建文件
## 1.`touch` 命令

如果你只想创建一个空文件，以后再填内容，这是最快的方法。

Bash

```
touch filename.v
```

- **用法**：`touch` 后面跟文件名。
    
- **特点**：如果文件不存在，它会新建一个；如果文件已存在，它只会更新文件的“最后修改时间”，不会覆盖内容。
    

---

## 2. `code` 或 `nano`

这是你目前最实用的操作。

- **VS Code 方式 (强烈推荐)**：
    
    Bash
    
    ```
    code my_new_file.v
    ```
    
    VS Code 会直接在 Windows 窗口弹出来。你写完内容，按 `Ctrl+S`，文件就存到了你当前所在的目录下。
    
- **轻量级终端编辑 (nano)**：
    
    Bash
    
    ```
    nano readme.txt
    ```
    
    它会在**窗口**里直接变出一个简易编辑器。
    
    - **保存**：按 `Ctrl + O`，然后按回车。
        
    - **退出**：按 `Ctrl + X`。
        

---

## 3. 想把一段话直接“塞”进文件：`echo` 和重定向

如果你只想快速往文件里写一行话，连编辑器都不想开：

Bash

```
echo "module main(); endmodule" > main.v
```

- **`>`**：这个符号叫“重定向”。它会把左边的文字覆盖写入右边的文件。
    
- **`>>`**：如果你想在文件末尾**追加**一行，而不是覆盖掉旧内容，就用两个大于号。
    

---

## 4. 连文件夹一起新建：`mkdir` + `touch`

如果你想在一个还没创建的文件夹里建文件，直接 `touch` 会报错。你需要先建目录：

Bash

```
# -p 参数代表“如果中间的文件夹不存在，也一并建好”
mkdir -p project/src/verilog 
touch project/src/verilog/test.v
```

---

## 如何确认文件建好了？

建完之后，别傻等着，用 **`ls`** (List) 看一眼：

- 输入 `ls`：列出当前文件夹所有文件。
    
- 输入 `ls -l`：看到文件的详细信息（大小、创建时间）。
    
# 三.Wsl and windows

WSL 和 Windows 的文件系统是互通的：

- 打开 Windows 资源管理器，输入路径 `\\wsl.localhost\Ubuntu-24.04` 即可进入 WSL 的文件系统
    - e.g. WSL 的 home 在 `\\wsl.localhost\Ubuntu-24.04\home\<username>`
- 在 WSL 中，Windows 的文件系统挂载在 `/mnt` 下，例如 C 盘就是 `/mnt/c`
    - e.g. 你的桌面的路径是 `/mnt/c/Users/<username>/Desktop`
# 四.转移文件
## 方法 1：Linux 命令行
`cp "lab0-1 report.md" /mnt/d/Users/lenovo/Desktop/`
-  注意：如果路径中有空格（比如 Program Files），记得用引号括起来或者用反斜杠转义。`
## 方法 2：资源管理器

这是最推荐的方法，操作起来就像在 Windows 文件夹之间拖拽一样。

1. 打开 Windows 的 **资源管理器** (File Explorer)。
    
2. 在左侧导航栏的最下方，寻找一个 **Linux** 鲸鱼图标。
    
3. 点击进入，你会看到你的发行版（如 `Ubuntu`），点击进去就能看到 `/home/bccc/...` 等所有 Linux 目录。
    
4. 直接将文件 **复制 (Ctrl+C)** 并 **粘贴 (Ctrl+V)** 到你的 Windows 桌面或文件夹中。
    

> **快捷技巧**：在 WSL 终端里输入 `explorer.exe .`（注意后面有个点），会直接弹出当前 Linux 路径对应的 Windows 窗口。
## 方法 3：VS Code 

如果你已经在用 VS Code 编写代码，这是最方便的：

1. 在 VS Code 的左侧文件树中找到你的文件。
    
2. **右键** 点击该文件，选择 **Download...** (下载)。
    
3. 在弹出的对话框中选择你想保存到的 Windows 路径即可。
# 五.删除文件
- **`rm`**: 仅针对文件。
    
- **`rmdir`**: 仅针对**空目录**（如果文件夹里有东西，这个命令也会报错）。
    
- **`rm -r`**: 针对目录及其包含的所有子文件/子文件夹。
# 六.查找

## 一、 找“文件本身”的终极利器

#### 1. `find`：地毯式搜索的“苦力”

这是 Linux 里最强大、最底层的查找命令。它会真正地去遍历硬盘上的每一个目录和文件。

- **优点**：绝对准确，且支持按名称、大小、修改时间、甚至权限来搜索。
    
- **缺点**：因为是实时遍历硬盘，如果从根目录 `/` 开始找，速度会比较慢。
    
- **典型用法**：
    
    - 按名字找：`find /tools -name "settings64.sh"` (在 /tools 目录下找)
        
    - 忽略大小写：`find ~ -iname "makefile"` (在你的家目录下找，不分大小写)
        
    - 按类型找：`find . -type d -name "build"` (在当前目录下只找名为 build 的**文件夹**)
        

#### 2. `locate`：依赖索引的“快枪手”

如果你嫌 `find` 太慢，`locate` 是最好的替代品。它不会去翻硬盘，而是去查系统后台每天自动生成的一本“电话簿”（数据库）。

- **优点**：极快，瞬间出结果。
    
- **缺点**：如果你刚刚新建或下载了一个文件，它可能查不到，因为“电话簿”还没更新。
    
- **典型用法**：`locate vivado.tcl`
    
- **避坑指南**：如果查不到刚建的文件，可以先手动运行一次 `sudo updatedb` 来刷新数据库，然后再 `locate`。
    

---

## 二、 找“命令和程序”的专属雷达

#### 1. `which`：只看环境变量的“近视眼”
**`which` 命令的用法**：`which` 是用来查找**可执行程序**（比如 `ls`, `gcc`, `python`）在系统哪个路径下的。`Makefile` 是一个**文本文件**，不是程序，所以 `which` 找不到它。
正如我们之前讨论的，它**只**在 
找同名的可执行文件。

- **典型用法**：`which python3` 或 `which vivado`
    
- **适用场景**：当你敲了一个命令却报错，或者你想确认当前运行的 `gcc` 到底是哪个版本时使用。
    

#### 2. `whereis`：找程序全家桶的“向导”

比 `which` 视野稍微广一点。它不仅找可执行文件，还会顺便帮你把这个程序的源代码文件和帮助文档（man page）的位置一起揪出来。

- **典型用法**：`whereis bash`
    

#### 3. `type`：看透本质的“照妖镜”

强烈建议用它来替代 `which`。它能准确告诉你系统是如何理解你敲下的这段字符的。

- **典型用法**：
    
    - 输入 `type cd`，它会说 `cd is a shell builtin`（内建命令）。
        
    - 输入 `type ll`，它会说 `ll is aliased to 'ls -alF'`（别名映射）。
        
    - 输入 `type vivado`，它会给出具体的路径。
        

---

## 三、 找“文件里的内容”

#### `grep`：文本内容扫描仪

有时候你忘记了文件名，只记得文件里写了什么（比如代码里定义了某个变量 `TOP_MODULE`），这时候就要用 `grep`。

- **优点**：直接穿透文件名，搜索文件内部的文本。
    
- **典型用法**：
    
    - 当前目录搜：`grep "TOP_MODULE" Makefile`
        
    - 整个项目搜：`grep -rn "TOP_MODULE" /home/bccc/Workspace/sys1-sp26/` (`-r` 递归子目录，`-n` 显示行号)
        

---

## 总结对照表

|你的需求|应该用哪个命令？|核心特点|
|---|---|---|
|**我只记得文件名，想找到它在哪**|`find` 或 `locate`|`find` 最准，`locate` 最快|
|**命令敲了没反应，想看它在哪**|`type` 或 `which`|`type` 是首选，能识别别名和内建|
|**我想找写了某行代码的那个文件**|`grep`|穿透文件查文本|
# 七.重启及状态检查
```
重启：wsl --shutdown
状态检查：wsl --list --verbose
```