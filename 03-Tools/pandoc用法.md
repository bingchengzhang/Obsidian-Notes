
---
# 🛠️ Pandoc：文档转换的“瑞士军刀”

Pandoc 是计算机领域最强大的文档转换工具，它可以将 Markdown (MD) 转换为 Word、PDF、HTML 等多种格式。

## 1. 核心转换命令

基本逻辑：`pandoc [源文件] -o [输出文件]`

| **目标格式** | **命令**                                               | **备注**                     |
| -------- | ---------------------------------------------------- | -------------------------- |
| **Word** | `pandoc input.md -o output.docx`                     | **无需**额外引擎，直接生成            |
| **PDF**  | `pandoc input.md -o output.pdf --pdf-engine=xelatex` | **需要**安装 LaTeX (如 TexLive) |
| **HTML** | `pandoc input.md -o output.html`                     | 适合发布到网页                    |
| **PPT**  | `pandoc input.md -o output.pptx`                     | 自动根据标题分幻灯片                 |

## 2. 进阶神技：样式模板 (`--reference-doc`)

不想每次转完 Word 都手动调字体？

1. 先准备一个格式调好的 `template.docx`。
    
2. 使用命令：
    
    Bash
    
    ```
	    pandoc input.md --reference-doc=template.docx -o output.docx
    ```
    
    _Pandoc 会自动把 MD 的内容“塞进”模板的样式里。_

### 详细代码展开   
- 如果样板文件也就是template不在要转化的笔记的同一个文件夹里——必须用绝对路径找

`pandoc pandoc用法.md --reference-doc="/mnt/c/Users/lenovo/Desktop/template.docx" -o "/mnt/c/Users/lenovo/Desktop/pandoc用法.docx"`

- ==错误==：`pandoc pandoc用法.md --reference-doc=template.docx -o "/mnt/c/Users/lenovo/Desktop/pandoc用法.docx"`
*template没有标明在哪，系统按照相对路径去算*
-  **绝对路径**：以 `/` 开头（比如 `/mnt/c/...`），无论你在哪个文件夹，都能顺着这根线找到文件。
    
- **相对路径**：直接写文件名（比如 `template.docx`），电脑只会在你**当前所在的文件夹**（即 `My Notes`）里找。


## 3. 处理图片与公式

- **图片**：MD 中的 `![描述](path)` 会自动嵌入 Word/PDF。
    
- **公式**：Pandoc 会自动将 LaTeX 公式 `$E=mc^2$` 转换为 Word 专用的公式对象，效果非常清晰。
    

## 4. 常见错误排查

- **参数错误**：例如 `--word-engine` 是不存在的。Word 转换是内置的，只有 PDF 需要指定 `--pdf-engine`。
    
- **路径问题**：如果文件名有空格，记得加引号：`pandoc "my note.md" -o out.docx`。
    
- **中文乱码**：转 PDF 时务必确保系统安装了中文字体，并在命令中指定支持中文的引擎。

## 5.定位Obsidian笔记路径

#### 方法 A：如果你当前就在笔记文件夹下

1. 在 Obsidian 里右键点击笔记，选择 `Show in explorer`。
    
2. 在地址栏输入 `powershell` 并回车。
    
3. 输入命令：
    
    PowerShell
    
    ```
    pandoc "pandoc用法.md" -o "C:\Users\lenovo\Desktop\pandoc用法.docx"
    ```
    
    _注意：因为路径中包含空格（如 `My Notes`），文件名必须用双引号包裹_

#### 方法 B：如果你在任意位置（使用绝对路径）

PowerShell

```
pandoc "C:\Users\lenovo\Documents\Obsidian Vault\My Notes\pandoc用法.md" -o "C:\Users\lenovo\Desktop\output.docx"
```



