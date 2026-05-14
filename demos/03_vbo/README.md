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
