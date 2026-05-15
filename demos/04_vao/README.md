# 04 VAO

第四节课目标：只理解一件事，**VAO 记录“顶点数据应该怎么读”**。

昨天已经用 RenderDoc 看到：`Buffer 50` 是 VBO，`Vertex Array 49` 是 VAO。今天就专门拆 VAO：VBO 保存顶点数据本身，VAO 保存读取这份数据的规则。

## 学习资料

- [LearnOpenGL - Hello Triangle](https://learnopengl.com/getting-started/hello-triangle)：只看 **Vertex Array Object** 这一小段。
- 回看 [03 VBO](../03_vbo/README.md)：只看 RenderDoc 里 `Buffer 50 / Vertex Array 49 / stride = 12` 的对应关系。

## 编译运行

```bash
./run.sh
```

加餐 demo：一个 VAO 记录两个属性，位置 + 颜色：

```bash
./run.sh color
```

也可以手动执行：

```bash
cmake -S . -B build
cmake --build build
./build/vao
./build/vao_color_attributes
```

## 今天只需要记住

- VBO 保存顶点数据。
- VAO 保存“怎么从 VBO 里读顶点属性”。
- `glVertexAttribPointer` 会被当前绑定的 VAO 记录下来。
- 绘制时绑定 VAO，OpenGL 就知道该用哪个 VBO、按什么 stride / offset 读取。

## 今日练习

今天只做 3 件事：

1. 打开 [src/main.cpp](src/main.cpp)，找到 `setupTriangleVAO`。
2. 看清楚顺序：先绑定 VAO，再绑定 VBO，再调用 `glVertexAttribPointer`。
3. 运行 `./run.sh`，观察两个三角形分别来自两个 VAO。

最小完成标准：能说出 **VAO 不是顶点数据本身，而是 VBO 的读取说明书**。

## 加餐：一个 VAO 记录多个属性

如果觉得基础 demo 太少，可以跑：

```bash
./run.sh color
```

这个 demo 里每个顶点有 6 个 float：

```cpp
// position             // color
-0.65f, -0.45f, 0.0f,   1.0f, 0.25f, 0.20f,
 0.65f, -0.45f, 0.0f,   0.25f, 0.95f, 0.35f,
 0.0f,   0.65f, 0.0f,   0.25f, 0.55f, 1.0f
```

VAO 会记录两条读取规则：

```cpp
// location = 0：读位置，从偏移 0 开始
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

// location = 1：读颜色，从偏移 3 * sizeof(float) 开始
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
```

这能提前看到一个更真实的顶点格式：一个顶点不只有位置，也可以有颜色、UV、法线等属性。今天只需要理解位置和颜色两个属性即可。

## Q&A

### Q: VAO 是什么？

A: VAO 是 Vertex Array Object。它可以先理解成“顶点读取说明书”。VBO 里是一串原始 float，VAO 记录这些 float 应该怎样解释成 shader 里的输入属性。

比如今天代码里：

```cpp
glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

这段配置完成后，VAO 会记住：

- 第 0 号属性启用了。
- 第 0 号属性来自当前绑定的 VBO。
- 每个顶点属性是 3 个 `float`。
- 每个顶点之间隔 12 字节。
- 从 buffer 开头开始读。

所以一句话：**VBO 是数据，VAO 是读法。**

### Q: 为什么今天要做两个 VAO？

A: 因为两个 VAO 能更直观地证明：VAO 会记住自己的 VBO 和读取规则。配置阶段分别创建两个 VAO；渲染阶段只需要切换 VAO：

```cpp
glBindVertexArray(leftVAO);
glDrawArrays(GL_TRIANGLES, 0, 3);

glBindVertexArray(rightVAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

如果两个三角形都能画出来，说明每个 VAO 都带着自己的顶点读取配置。

### Q: VAO 只能记录一个属性吗？

A: 不是。VAO 可以记录多个顶点属性。比如加餐 demo 里：

- `location = 0`：位置 `aPos`。
- `location = 1`：颜色 `aColor`。

它们都来自同一个 VBO，但 offset 不同：

```text
一个顶点：x y z r g b
位置属性：从 x 开始读 3 个 float
颜色属性：从 r 开始读 3 个 float
```

所以 VAO 更准确的理解是：**记录一组顶点属性读取规则**。
