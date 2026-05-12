# 01 Hello Triangle

第一节课目标：按 LearnOpenGL 的路线，用 GLFW 创建窗口，用 GLAD 加载 OpenGL 函数，然后画出第一个三角形。

## 依赖

这个 demo 使用 LearnOpenGL 的经典组合：

- CMake
- GLFW
- GLAD（本工程已内置第一课需要的精简 loader）
- OpenGL 3.3 Core Profile

macOS 可以先安装：

```bash
brew install cmake glfw
```

## 编译运行

```bash
cmake -S . -B build
cmake --build build
./build/hello_triangle
```

如果一切正常，会出现一个窗口，里面有一个橙色三角形。

## 今天只需要记住

- GLFW：负责创建窗口和处理输入。
- GLAD：负责加载 OpenGL 函数。
- VAO：记录顶点数据应该怎么被读取。
- VBO：把顶点数据放到 GPU。
- Shader：告诉 GPU 怎么处理顶点和颜色。
