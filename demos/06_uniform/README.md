# 06 Uniform

第六节课目标：只理解一件事，**uniform 是 CPU 传给 shader、一次 draw 共同使用的数据**。

前面 Day 05 里，颜色是每个顶点自己的 attribute：`aColor`。今天换一种方式：顶点只传位置，颜色由 CPU 在 draw 前通过 `uniform vec4 uColor` 传给 fragment shader。

## 学习资料

- [LearnOpenGL - Shaders](https://learnopengl.com/getting-started/shaders)：只看 **Uniforms** 这一小段。
- 回看 [05 Shader Attributes](../05_shader_attributes/README.md)：只对比 attribute 和 uniform 的区别。

## 编译运行

```bash
./run.sh
```

也可以手动执行：

```bash
cmake -S . -B build
cmake --build build
./build/uniform
```

## 今天只需要记住

- `attribute` 是每个顶点自己的数据。
- `uniform` 是一次 draw 共同使用的数据。
- CPU 用 `glUniform*` 把值传给当前使用的 shader program。
- 设置 uniform 前，通常要先 `glUseProgram(shaderProgram)`。

## 今日练习

1. 运行 `./run.sh`，先看到一个会缓慢变色的三角形。
2. 打开 [src/main.cpp](src/main.cpp)，只看 `uColor`、`glGetUniformLocation`、`glUniform4f`。
3. 改 `glUniform4f` 里的颜色计算或固定颜色，再运行 `./run.sh`。
4. 写 3-5 句话笔记：attribute 是顶点级数据，uniform 是 draw 级数据。

最小完成标准：能说出 **颜色不是顶点属性，而是 CPU 每帧传给 shader 的 uniform**。

## Q&A

### Q: uniform 和 attribute 有什么区别？

A: `attribute` 是每个顶点都可以不同的数据，比如位置、颜色、UV、法线。它来自 VBO，经 VAO 读取后进入 vertex shader。

`uniform` 是一次 draw 期间共享的数据，比如一个整体颜色、时间、矩阵、材质参数。它不是从 VBO 里逐顶点读取，而是 CPU 在 draw 前用 `glUniform*` 设置给 shader。

可以先这样记：

```text
attribute：每个顶点一份
uniform：一次 draw 共用一份
```

### Q: 为什么设置 uniform 前要 `glUseProgram`？

A: 因为 uniform 是设置到“当前正在使用的 shader program”里的。OpenGL 是状态机风格，`glUseProgram(shaderProgram)` 会把当前 program 切到这个 shader；之后 `glUniform4f(...)` 才知道要把值写进哪个 program。

今天代码里的顺序是：

```cpp
glUseProgram(shaderProgram);
int colorLocation = glGetUniformLocation(shaderProgram, "uColor");
glUniform4f(colorLocation, red, green, blue, 1.0f);
```

可以理解成：先选中 shader，再给这个 shader 填参数。
