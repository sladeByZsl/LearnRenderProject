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
