# 02 Vertex Data

第二节课目标：只理解一件事，**顶点坐标如何决定三角形形状**。

今天先不要急着深入 VAO、VBO、Shader。它们会继续出现在代码里，但今天的观察重点只有 `vertices[]`。

## 学习资料

- [LearnOpenGL - Hello Triangle](https://learnopengl.com/getting-started/hello-triangle)：只看 **Vertex input** 里 `float vertices[]` 和 Normalized Device Coordinates 这一小段。
- 回看上一节 [01 Hello Triangle](../01_hello_triangle/README.md)：只看 `vertices[]` 附近的解释。

## 编译运行

以后每个每日 demo 工程都会带一个 `run.sh`，优先用它完成“配置、编译、运行”：

```bash
./run.sh
```

也可以手动执行：

```bash
cmake -S . -B build
cmake --build build
./build/vertex_data
```

如果一切正常，会出现一个窗口，里面有一个**不是默认形状**的橙色三角形。

## 今天只需要记住

- 一个三角形由 3 个顶点组成。
- 现在每个顶点有 3 个数字：`x, y, z`。
- `x` 控制左右，`y` 控制上下，`z` 今天先保持 `0.0f`。
- OpenGL 第一课里用的是 NDC 坐标，屏幕中心是 `(0, 0)`，大致范围是 `-1` 到 `1`。
- 传给 shader 的顶点位置不是像素坐标，而是 NDC 坐标。

## 今日笔记

- 今天记住了：`vertices[]` 里传的是 NDC 坐标，不是窗口像素坐标。
- `(-1, -1)` 是可见区域左下角，`(1, 1)` 是右上角，`(0, 0)` 是中心。
- 一个点在左下角，不代表整个三角形都在左下角；三角形的位置和形状由三个点一起决定。
- 改完代码后运行 `./run.sh`，脚本会先做增量编译，再启动最新程序。

## 今日练习

只改 [src/main.cpp](src/main.cpp) 里的 `vertices[]`：

1. 把三角形改宽。
2. 把三角形改高。
3. 把三角形倒过来。
4. 保留一个你最容易理解的形状，并给每个点写注释。

最小完成标准：能说出“我改了哪几个点，所以三角形变成了这个样子”。

## 补充观察工具

- [NDC 坐标系对比](../ndc_comparison.html)：这是额外观察页面，包含 OpenGL / Vulkan / DirectX 的 NDC 差异。今天只需要先看 OpenGL 那一列，其他 API 后面再回头理解。
- [平台 NDC 坐标差异速记图](../../notes/recaps/assets/ndc-platform-differences.svg)：修正版图片，适合以后复习 OpenGL / Vulkan / DirectX 的 NDC 差异。

## Q&A

### Q: LearnOpenGL 的 Vertex input 这一段主要讲什么？

A: 这一段在讲：OpenGL 画三角形前，必须先给它一组三维顶点坐标。即使现在看起来是 2D 三角形，每个点也写成 `x, y, z` 三个数，只是 `z` 先都写 `0.0f`。

第一课里的坐标使用 NDC，也就是标准化设备坐标。可以先理解成 OpenGL 的可见区域：`x = -1` 靠左，`x = 1` 靠右，`y = -1` 靠下，`y = 1` 靠上，`(0, 0)` 在屏幕中心。超出这个范围的内容通常会被裁掉，看不到。

所以今天最重要的一句话是：**三角形不是用像素画出来的，而是由 3 个 NDC 顶点决定的；OpenGL 后面会把这些坐标转换成屏幕像素。**

### Q: `(-1, -1)` 是不是左下角？为什么我看到三角形铺到很大一片？

A: 是的，在 OpenGL 的 NDC 里，`(-1, -1)` 就是可见区域的左下角。不过它只是其中一个顶点的位置，不代表整个三角形都在左下角。

三角形最终长什么样，要看三个点一起连起来。例如：

```cpp
float vertices[] = {
    -1.0f, -1.0f, 0.0f, // 左下角
     1.0f,  1.0f, 0.0f, // 右上角
     0.0f,  1.0f, 0.0f  // 上边界中间
};
```

这里第一个点在左下角，另外两个点都在上边界，所以三角形会从左下角斜着铺到屏幕上方，看起来面积很大。

如果想让三角形整体待在左下角，三个点都要靠近左下角：

```cpp
float vertices[] = {
    -1.0f, -1.0f, 0.0f,
    -0.4f, -1.0f, 0.0f,
    -1.0f, -0.4f, 0.0f
};
```
