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

## CMake 是什么

**CMake** 是一个「跨平台的构建系统生成器」：你写一份与具体 IDE/编译器无关的描述文件（本目录下的 `CMakeLists.txt`），CMake 根据你的环境生成 **Makefile、Ninja、Xcode 工程** 等，再由真正的编译器（如 Clang、MSVC）去编译链接。

这样做的常见好处：

- **一份配置，多机可用**：换机器或换系统时，只要装好依赖，重新 `cmake` 一遍即可。
- **声明依赖与目标**：找系统里的 OpenGL、GLFW，把 `glad` 编成静态库，再把可执行文件和库链在一起，都写在脚本里，避免手写一长串编译命令。
- **和编辑器解耦**：你可以在终端用 Ninja/Make，也可以在 CLion / VS Code 里打开同一个 `CMakeLists.txt`。

下面两条是本 README「编译运行」里会用到的命令在做什么：

| 命令 | 作用 |
|------|------|
| `cmake -S . -B build` | **S**ource 是当前目录 `.`，在 **B**uild 目录 `build` 里生成构建文件（首次会检测编译器、查找 `OpenGL`、`glfw3` 等）。 |
| `cmake --build build` | 在 `build` 里调用底层构建工具，实际编译出 `hello_triangle`（以及本工程里的 `libglad.a`）。 |

改代码后一般只需再执行 `cmake --build build`；只有改了 `CMakeLists.txt`、换了依赖路径或想换生成器时，才需要重新跑一遍 `cmake -S . -B build`。

## 本 demo 的 CMakeLists.txt 在做什么

与本仓库 `CMakeLists.txt` 一一对应的大意如下：

1. **`cmake_minimum_required` / `project`**：要求 CMake 至少 3.16，工程名为 `hello_triangle`，启用 C 与 C++。
2. **`CMAKE_CXX_STANDARD 17` 等**：强制用 C++17、关闭编译器对标准的不必要扩展，减少「在本机能编、换环境就炸」的差异。
3. **`find_package(OpenGL REQUIRED)`**：找系统 OpenGL，后面用 `OpenGL::GL` 这个「导入目标」来链接，路径由 CMake 处理。
4. **`find_package(glfw3 REQUIRED)`**：找已安装的 GLFW 3；macOS 上通常来自 `brew install glfw`。
5. **`add_library(glad STATIC ...)`**：把内置的 GLAD 源码编成静态库 `glad`，并把 `external/glad/include` 作为公共头文件路径，供可执行目标包含 `glad/glad.h`。
6. **`add_executable(hello_triangle src/main.cpp)`**：生成可执行文件目标。
7. **`target_link_libraries(... glad glfw OpenGL::GL)`**：把 GLAD、GLFW、OpenGL 链到 `hello_triangle` 上；`PRIVATE` 表示这些依赖不传递给依赖 `hello_triangle` 的其他目标（本 demo 里没有别的目标，写 `PRIVATE` 是常见习惯）。
8. **`if(APPLE)` 里的 `GL_SILENCE_DEPRECATION`**：在 macOS 上屏蔽 Apple 将 OpenGL 标为弃用时的编译告警，便于安静编译；不影响运行逻辑。

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

## Q&A

### Q: 什么是 GLFW？

A: GLFW 是一个帮 OpenGL 程序处理“窗口和输入”的轻量库。OpenGL 本身只负责画图，不负责创建窗口、接收键盘鼠标、管理渲染上下文这些平台相关的事情；GLFW 就是补上这一层，让我们可以用同一套代码在 macOS、Windows、Linux 上创建窗口并拿到 OpenGL 上下文。

在第一课里，GLFW 主要做三件事：

- 创建一个能显示 OpenGL 画面的窗口。
- 创建并绑定 OpenGL Context，让后面的 `gl*` 调用知道要画到哪个窗口。
- 处理输入和窗口事件，比如按 Esc 退出、窗口尺寸变化、关闭按钮。

可以先把它理解成：**GLFW 负责“开画布和收输入”，OpenGL 负责“往画布上画东西”。**
