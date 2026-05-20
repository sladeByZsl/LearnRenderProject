# 05 Shader Attributes

第五节课目标：只理解一件事，**Shader 接收并使用 VAO 送来的顶点属性**。

前面已经知道：VBO 保存顶点数据，VAO 记录这些数据怎么读。今天继续往后走一步：VAO 读出来的属性，会进入 vertex shader 的 `layout(location = ...) in ...` 变量。

## 学习资料

- [LearnOpenGL - Shaders](https://learnopengl.com/getting-started/shaders)：只看开头到 **Vertex shader** / **Fragment shader** 的介绍。
- 回看 [04 VAO](../04_vao/README.md)：只看 `color` demo 里 `aPos`、`aColor`、`vertexColor`。

## 编译运行

```bash
./run.sh
```

也可以手动执行：

```bash
cmake -S . -B build
cmake --build build
./build/shader_attributes
```

## 今天只需要记住

- `layout(location = 0) in vec3 aPos` 接收位置属性。
- `layout(location = 1) in vec3 aColor` 接收颜色属性。
- Vertex shader 可以把颜色通过 `out` 传给 fragment shader。
- Fragment shader 决定最终像素颜色。

## 今日练习

1. 运行 `./run.sh`，先看到三角形颜色渐变。
2. 打开 [src/main.cpp](src/main.cpp)，只看 `vertexShaderSource` 和 `fragmentShaderSource`。
3. 修改 `vertices[]` 里每个顶点后 3 个颜色值，重新运行 `./run.sh`。
4. 写 3-5 句话笔记：`location 0` 是位置，`location 1` 是颜色，shader 负责使用这些输入。

最小完成标准：能说出 **VAO 负责读数据，Shader 负责用数据**。

## Q&A

### Q: `layout(location = 0)` 和 `layout(location = 1)` 是什么？

A: 它们是 shader 输入变量的位置编号。VAO 里配置的 attribute 编号，要和 shader 里的 `location` 对上。

代码里：

```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
```

shader 里：

```glsl
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
```

所以对应关系是：

```text
attribute 0 -> aPos
attribute 1 -> aColor
```

### Q: 为什么三角形中间会渐变？

A: 因为三个顶点各自有颜色。Vertex shader 把每个顶点的颜色输出给后续阶段，光栅化阶段会在三角形内部自动插值，fragment shader 收到的是插值后的颜色。

今天可以先不深究插值算法，只记一句：**顶点有不同颜色，三角形内部会自动过渡出渐变**。

### Q: `glVertexAttribPointer` 和 `vertices[]` 是怎么对应的？

A: 当前 `vertices[]` 里，一个顶点占 6 个 `float`：

```text
x y z r g b
```

前三个是位置，后三个是颜色：

```cpp
float vertices[] = {
    // position             // color
    -0.65f, -0.45f, 0.0f,   1.0f, 0.20f, 0.15f,
     0.65f, -0.45f, 0.0f,   0.15f, 0.95f, 0.35f,
     0.0f,   0.65f, 0.0f,   0.20f, 0.45f, 1.0f
};
```

所以 `stride` 是：

```cpp
const int stride = 6 * sizeof(float);
```

第一条属性：

```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
glEnableVertexAttribArray(0);
```

表示：

```text
attribute 0
每次读 3 个 float
从每个顶点的开头读
读到的是 x y z
对应 shader 的 aPos
```

第二条属性：

```cpp
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

表示：

```text
attribute 1
每次读 3 个 float
先跳过前 3 个 float
读到的是 r g b
对应 shader 的 aColor
```

可以画成：

```text
| x | y | z | r | g | b |
|--- position ---|--- color ---|
^                ^
offset 0         offset 3 * sizeof(float)

stride = 6 * sizeof(float)
```

今天这节课最重要的结论是：**VBO 保存数字，VAO 保存怎么分段读这些数字，Shader 通过 `location` 接收读出来的属性。**
