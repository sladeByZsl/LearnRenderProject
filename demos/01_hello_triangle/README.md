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

### Q: `glfwWindowHint` 设置 OpenGL 3.3 Core Profile 是什么意思？

A: 这几行是在创建窗口之前告诉 GLFW：**我希望你帮我创建一个 OpenGL 3.3 Core Profile 的上下文**。OpenGL Context 可以理解成“当前程序使用 OpenGL 的运行环境”；版本和 Profile 会决定哪些 OpenGL 功能可用。

这三行分别是：

```cpp
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

- `GLFW_CONTEXT_VERSION_MAJOR, 3`：主版本号要 3。
- `GLFW_CONTEXT_VERSION_MINOR, 3`：次版本号要 3。
- `GLFW_OPENGL_CORE_PROFILE`：使用 Core Profile，也就是现代 OpenGL 风格。

OpenGL 3.3 Core Profile 的意思是：使用 OpenGL 3.3，并且不要旧式固定管线 API。比如早期 OpenGL 里可以用 `glBegin` / `glEnd` 直接画东西，但 Core Profile 更强调 VAO、VBO、Shader 这一套现代渲染流程。LearnOpenGL 从这里开始，是为了让我们一上来就学现在更常见的写法。

macOS 这段：

```cpp
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
```

只在 Apple 平台编译时生效。macOS 创建 OpenGL 3.2 以上 Core Profile 上下文时要求加 `GLFW_OPENGL_FORWARD_COMPAT`，意思是创建一个向前兼容的上下文，不包含已经废弃的旧功能。你可以先把它记成：**在 macOS 上申请现代 OpenGL 上下文时必须加的兼容设置**。

整体顺序也很重要：这些 `glfwWindowHint` 必须写在 `glfwCreateWindow` 之前，因为它们是在“下单前提要求”。窗口创建出来以后，再改这些 hint 就不会影响已经创建好的窗口了。

### Q: OpenGL 为什么连窗口都不管？

A: 因为 OpenGL 的职责边界很窄：它只定义“怎么把图形命令交给 GPU、怎么渲染”，不定义“应用窗口怎么创建”。窗口、鼠标、键盘、菜单栏、全屏、显示器选择这些东西都属于操作系统和窗口系统，不同平台差异很大。

比如：

- Windows 有 Win32 窗口和 WGL。
- Linux 常见有 X11 / Wayland，以及 GLX / EGL。
- macOS 有 Cocoa / NSWindow，以及自己的 OpenGL 上下文创建方式。

如果 OpenGL 自己把窗口也管了，它就必须把每个操作系统的应用框架都包进去，API 会变得又大又复杂，也很难保持跨平台稳定。所以 OpenGL 选择只做一件事：**提供渲染接口**。

这也是为什么真实工程通常会有分工：

- GLFW / SDL：创建窗口、处理输入、创建 OpenGL Context。
- GLAD：加载当前平台上的 OpenGL 函数地址。
- OpenGL：真正负责渲染命令，比如清屏、传顶点、画三角形。

可以把它想成拍电影：GLFW 负责搭影棚和开机位，GLAD 负责接好设备线路，OpenGL 负责真正把画面拍出来。第一天只要记住一句话就够了：**OpenGL 只管画，不管窗口。**

### Q: `glfwCreateWindow` 和 `glfwMakeContextCurrent` 是做什么的？

A: 这两步是在把“系统窗口”和“OpenGL 渲染环境”接起来。

```cpp
GLFWwindow* window = glfwCreateWindow(
    SCR_WIDTH,
    SCR_HEIGHT,
    "LearnOpenGL - Hello Triangle",
    nullptr,
    nullptr
);
```

`glfwCreateWindow` 会创建一个窗口，并根据前面的 `glfwWindowHint` 申请一个 OpenGL Context。返回值 `GLFWwindow* window` 可以理解成这个窗口的句柄，后面 GLFW 需要靠它知道你在操作哪个窗口。

参数含义是：

- `SCR_WIDTH`：窗口宽度。
- `SCR_HEIGHT`：窗口高度。
- `"LearnOpenGL - Hello Triangle"`：窗口标题。
- 第 4 个参数 `nullptr`：不创建全屏窗口。如果传入某个显示器，就可以创建全屏窗口。
- 第 5 个参数 `nullptr`：不和其他窗口共享 OpenGL 资源。高级用法里可以让多个窗口共享纹理、缓冲等资源。

创建失败时返回 `nullptr`，所以代码要检查：

```cpp
if (window == nullptr)
{
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
}
```

这表示窗口或 OpenGL Context 没创建成功，程序继续跑也没有意义，所以释放 GLFW 资源并退出。

接着这句很关键：

```cpp
glfwMakeContextCurrent(window);
```

它的意思是：**把这个窗口的 OpenGL Context 设为当前线程正在使用的 Context**。OpenGL 的很多函数不是传 `window` 参数的，比如 `glClearColor`、`glDrawArrays` 都没有告诉它要画到哪个窗口；它们默认作用在“当前 Context”上。

所以如果没有调用 `glfwMakeContextCurrent(window)`，后面的 OpenGL 调用就不知道该作用到哪个渲染环境，GLAD 也没法正确加载 OpenGL 函数。第一课可以这样记：

- `glfwCreateWindow`：创建窗口和 OpenGL Context。
- `glfwMakeContextCurrent`：把这个 Context 激活，告诉当前线程“接下来 OpenGL 都画到这里”。
