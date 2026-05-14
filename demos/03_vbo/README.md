# 03 VBO

第三节课目标：只理解一件事，**VBO 是把顶点数据放到 GPU 里的缓冲区**。

昨天已经记住：`vertices[]` 里传的是 NDC 坐标，不是像素坐标。今天继续往后走一步：这些坐标一开始在 CPU 内存里，调用 `glBufferData` 后会被复制到 GPU 端的 VBO 里。

## 学习资料

- [LearnOpenGL - Hello Triangle](https://learnopengl.com/getting-started/hello-triangle)：只看 **Vertex input** 里 `glGenBuffers`、`glBindBuffer`、`glBufferData` 这一小段。
- 回看 [02 Vertex Data](../02_vertex_data/README.md)：只确认昨天的 NDC 坐标结论。

## 编译运行

```bash
./run.sh
```

也可以手动执行：

```bash
cmake -S . -B build
cmake --build build
./build/vbo
```

## 今天只需要记住

- `vertices[]` 是 CPU 侧的一段数组。
- `VBO` 是 GPU 侧的一块顶点数据缓冲区。
- `glBindBuffer(GL_ARRAY_BUFFER, VBO)` 表示：接下来操作的顶点数组缓冲区就是这个 VBO。
- `glBufferData(...)` 表示：把 CPU 侧的 `vertices[]` 复制到当前绑定的 VBO。

## 今日练习

只围绕 VBO 做 3 件小事：

1. 打开 [src/main.cpp](src/main.cpp)，找到 `glGenBuffers`、`glBindBuffer`、`glBufferData`。
2. 先不要改逻辑，只读注释，确认“申请 ID -> 绑定 -> 上传数据”的顺序。
3. 改一两个顶点坐标，运行 `./run.sh`，观察改动是否生效。
4. 在 README 写 3-5 句话，说明你对 VBO 的理解。

最小完成标准：能说出 **VBO 保存的是顶点数据本身，`glBufferData` 是把 CPU 数组上传到 GPU**。

## Q&A

### Q: VBO 到底是什么？

A: VBO 是 Vertex Buffer Object，可以先理解成 GPU 里的“顶点数据仓库”。`vertices[]` 这个数组一开始在 CPU 内存里，OpenGL 真正绘制时更适合让 GPU 直接读自己的显存，所以我们要创建一个 VBO，把顶点数据上传进去。

### Q: 为什么需要先 `glBindBuffer`，再 `glBufferData`？

A: OpenGL 很多 API 是“状态机”风格。`glBindBuffer(GL_ARRAY_BUFFER, VBO)` 的意思是：把这个 VBO 放到 `GL_ARRAY_BUFFER` 这个当前操作位上。之后调用 `glBufferData(GL_ARRAY_BUFFER, ...)` 时，OpenGL 就知道要把数据上传到刚才绑定的那个 VBO。

可以先用一句话记：**绑定是选择当前要操作谁，上传是把数据交给它。**

### Q: OpenGL 很多 API 是“状态机”风格，这是什么意思？

A: 状态机风格的意思是：OpenGL 内部会记住一堆“当前状态”，后面的函数调用默认会作用在这些当前状态上，而不是每次都把所有对象作为参数传进去。

比如 VBO 这里：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

第一句不是上传数据，而是在告诉 OpenGL：**当前 `GL_ARRAY_BUFFER` 位置上使用的是这个 VBO**。第二句没有直接写 `VBO`，只写了 `GL_ARRAY_BUFFER`，所以它会把数据上传到“当前绑定在 `GL_ARRAY_BUFFER` 上的那个 VBO”。

可以把它想成 Unity Inspector 里先选中一个对象，然后你改 Transform 面板，改动就会作用到当前选中的对象上。OpenGL 的 `bind` 就像“选中对象”，后续调用就会作用到被选中的对象上。

这种设计的好处是 API 形式比较统一；坏处是代码阅读时要记住“当前绑定的是谁”。所以学习 OpenGL 时，经常要顺着 `glBind*` 往下看，确认后面的 `gl*` 调用到底在改哪个对象。

### Q: 可以同时绑定两个 `GL_ARRAY_BUFFER` 吗？

A: 不能。`GL_ARRAY_BUFFER` 是一个绑定点，同一时刻只能有一个 VBO 绑定在这个位置上。

比如：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO1);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, VBO2);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
```

第二次 `glBindBuffer` 会把当前 `GL_ARRAY_BUFFER` 从 `VBO1` 切换成 `VBO2`。这不代表 `VBO1` 被删除，也不代表它的数据没了；只是“当前选中对象”换成了 `VBO2`。

可以理解成：你可以有很多个 VBO 仓库，但 `GL_ARRAY_BUFFER` 这个操作台一次只能摆一个仓库。你要改哪个仓库，就先把哪个仓库搬到操作台上。

后面会遇到一个更重要的点：`glVertexAttribPointer` 会把“当前绑定的 `GL_ARRAY_BUFFER`”记录进 VAO。也就是说，配置顶点属性时，绑定的是哪个 VBO，VAO 就会记住那个 VBO。这个留到 VAO 那天再细拆。

### Q: “绑定点”是什么？

A: 绑定点可以理解成 OpenGL 里预先规定好的“操作入口”或“插槽”。你不能随便对一个 GPU 对象调用所有操作，而是要先把对象绑定到某个对应的绑定点上，再通过这个绑定点操作它。

比如：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

这里 `GL_ARRAY_BUFFER` 就是绑定点。它的含义是：当前要操作的是“顶点数组缓冲区”。`VBO` 是具体对象，`GL_ARRAY_BUFFER` 是 OpenGL 提供的操作位置。

可以用这个模型记：

```text
具体对象：VBO1、VBO2、VBO3
绑定点：GL_ARRAY_BUFFER
当前状态：GL_ARRAY_BUFFER -> VBO2
后续操作：glBufferData(GL_ARRAY_BUFFER, ...) 实际改的是 VBO2
```

绑定点不是对象本身，而是对象被接到 OpenGL 管线上的位置。换句话说：**对象负责存数据，绑定点负责告诉 OpenGL 当前要按哪种用途操作这个对象。**

后面还会看到很多类似绑定点：

- `GL_ARRAY_BUFFER`：绑定 VBO，表示顶点属性数据。
- `GL_ELEMENT_ARRAY_BUFFER`：绑定 EBO，表示索引数据。
- `GL_TEXTURE_2D`：绑定 2D 纹理。
- `GL_FRAMEBUFFER`：绑定帧缓冲。

所以现在先记一句话：**绑定点是 OpenGL 的操作插槽，绑定对象以后，后续 API 通过这个插槽找到真正要操作的对象。**

### Q: 我能看到 OpenGL 的当前状态吗？

A: 能看到一部分，但 OpenGL 没有一个统一的“打印全部当前状态”按钮。通常有两种方式：

第一种是用 `glGet*` 查询某个具体状态。例如想知道当前 `GL_ARRAY_BUFFER` 绑定的是哪个 VBO，可以这样查：

```cpp
int currentArrayBuffer = 0;
glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentArrayBuffer);
std::cout << "Current GL_ARRAY_BUFFER = " << currentArrayBuffer << std::endl;
```

如果前面执行过：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
```

那么 `currentArrayBuffer` 打印出来的值通常就会等于 `VBO` 的 ID。

常见查询例子：

```cpp
GL_ARRAY_BUFFER_BINDING          // 当前 GL_ARRAY_BUFFER 绑定的 VBO
GL_VERTEX_ARRAY_BINDING          // 当前绑定的 VAO
GL_CURRENT_PROGRAM               // 当前使用的 shader program
GL_VIEWPORT                      // 当前 viewport
```

第二种是用图形调试工具，比如 RenderDoc、Xcode GPU Frame Capture。这类工具可以抓一帧，看到 draw call 当时绑定了哪些 buffer、shader、texture、uniform，比自己到处写 `glGet*` 更适合排查复杂渲染问题。

学习阶段可以先记：**可以查，但要按状态项一个一个查；真正工程调试通常会用图形调试器看整帧状态。**

### Q: 我这个工程能抓 GPU 帧吗？

A: 当前工程是 macOS + GLFW + OpenGL。结论要分情况：

- 在当前 macOS 环境下，不建议把“抓完整 GPU 帧”作为今天的学习目标。Apple 已经弃用 OpenGL，Xcode 的现代 GPU Frame Capture 主要面向 Metal；当前这种 GLFW/OpenGL 小工程，用 Xcode 抓帧不一定稳定、入口也不一定可用。
- RenderDoc 很适合学习图形调试，但官方支持主要是 Windows / Linux / Android。它支持 OpenGL 3.2 - 4.6 Core Profile；如果以后把这个工程放到 Windows 或 Linux 上跑，当前 OpenGL 3.3 Core Profile demo 就很适合用 RenderDoc 抓帧。
- 现在最实用的做法，是先在代码里用 `glGet*` 查询关键状态，比如当前 VBO、VAO、shader program。等后面想系统学习图形调试时，再专门安排一天用 RenderDoc 或 Xcode GPU 工具。

参考：

- [RenderDoc README - API Support](https://github.com/baldurk/renderdoc#api-support)
- [Apple Developer - Capturing a Metal workload in Xcode](https://developer.apple.com/documentation/metal/frame_capture_debugging_tools/capturing_a_frame_using_a_breakpoint)
