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

运行两个三角形封装示例：

```bash
cmake --build build
./build/two_triangles
```

运行更接近 Unity 写法的示例：

```bash
cmake --build build
./build/unity_style_triangles
```

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

### Q: GLAD 是什么，为什么要在创建 Context 后初始化？

A: GLAD 是一个 OpenGL 函数加载器。它不负责画图，也不负责创建窗口；它的工作是：**帮 C/C++ 程序找到当前机器、当前显卡驱动、当前 OpenGL Context 里那些 OpenGL 函数的真实地址**。

第一课可以先这样理解：

- `glfwCreateWindow`：把窗口和 OpenGL Context 创建出来。
- `glfwMakeContextCurrent`：激活这个 Context。
- `gladLoadGLLoader(...)`：根据当前 Context，把 OpenGL 函数入口找出来。
- 后面的 `glClearColor`、`glCreateShader`、`glGenBuffers` 等函数才能正常调用。

为什么不能直接调用 OpenGL 函数？因为 OpenGL 很多函数不是普通静态库里固定写死的函数。不同操作系统、不同显卡驱动、不同 OpenGL 版本支持的函数可能不同，程序运行时需要问驱动：“这个函数在哪里？当前环境能不能用？”GLAD 就是帮我们做这件事的工具。

这句代码：

```cpp
gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)
```

可以拆成两部分看：

- `glfwGetProcAddress`：GLFW 提供的查询函数，用来向当前 OpenGL Context 查询某个 OpenGL 函数地址。
- `gladLoadGLLoader(...)`：GLAD 拿着这个查询函数，把它需要的 OpenGL 函数地址一个个加载进来。

为什么必须在 `glfwMakeContextCurrent(window)` 之后？因为 OpenGL 函数地址依赖当前 Context。还没创建并激活 Context 时，GLFW 也不知道应该向哪个 OpenGL 环境查询函数地址。

失败检查也很重要：

```cpp
if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
{
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
}
```

如果 GLAD 初始化失败，说明 OpenGL 函数入口没加载好，继续调用 `gl*` 函数很可能崩溃或没有效果，所以直接退出。

可以用一个简单比喻记住：**GLFW 帮你打开画室，OpenGL 是画画能力，GLAD 是把画笔、颜料、工具的位置登记清楚。登记完成以后，你才知道每个工具在哪里，后面才能真正开画。**

### Q: OpenGL 是谁实现的，为什么不同机器上可能不一样？

A: OpenGL 本身更像一份“接口规范”，不是某一个固定的库文件。规范由 Khronos Group 维护，它规定了函数名字、参数、行为和渲染规则；真正把这些规则变成可执行代码的，是各个平台和显卡驱动厂商。

可以分成两层看：

- OpenGL 规范：规定“应该有什么函数、这些函数应该做什么”。
- OpenGL 实现：显卡驱动和系统图形层真正提供这些函数的代码。

所以在不同机器上，实现者可能不一样：

- NVIDIA 显卡：很多 OpenGL 功能由 NVIDIA 驱动实现。
- AMD 显卡：很多 OpenGL 功能由 AMD 驱动实现。
- Intel 显卡：很多 OpenGL 功能由 Intel 驱动实现。
- macOS：OpenGL 还要经过 Apple 提供的系统图形框架。

这就是为什么“支持的 OpenGL 版本、扩展能力、函数入口地址”可能不同。你的程序写的是同一套 OpenGL 调用，但运行时面对的可能是不同驱动、不同显卡、不同系统提供的实现。

为什么不能像普通 C++ 函数一样直接调用？因为很多 OpenGL 函数，尤其是较新的函数和扩展函数，不一定在编译时就能从一个固定库里链接到。程序需要在运行时根据当前 Context 去问驱动：“`glCreateShader` 在哪里？`glGenBuffers` 在哪里？当前环境支持吗？”

GLAD 做的就是这件事：它拿到 GLFW 提供的 `glfwGetProcAddress` 查询能力，然后把当前环境里可用的 OpenGL 函数地址加载出来。加载完成后，我们写的 `glCreateShader(...)` 这类调用，才会跳到当前机器真正的驱动实现里。

第一天可以这样记：

- OpenGL 规范：菜单，规定有哪些菜。
- 显卡驱动：厨房，真正把菜做出来。
- GLAD：服务员，帮程序确认这家厨房能做哪些菜、每道菜该找谁下单。

### Q: 三角形顶点数组里的顺序是什么意思？

A: 这个数组是按“每 3 个 float 表示 1 个顶点”的顺序排的。因为我们后面会告诉 OpenGL：一个顶点由 3 个 `float` 组成，分别是 `x, y, z`。

```cpp
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
```

所以它实际表示 3 个点：

| 顶点 | x | y | z | 位置 |
|------|---|---|---|------|
| 第 1 个点 | `-0.5` | `-0.5` | `0.0` | 左下 |
| 第 2 个点 | `0.5` | `-0.5` | `0.0` | 右下 |
| 第 3 个点 | `0.0` | `0.5` | `0.0` | 上方中间 |

OpenGL 的标准化设备坐标可以先记成：

- `x = -1` 靠左，`x = 1` 靠右。
- `y = -1` 靠下，`y = 1` 靠上。
- 这里 `z = 0`，表示先不考虑深度，第一课只看一个平面三角形。

后面这句：

```cpp
glDrawArrays(GL_TRIANGLES, 0, 3);
```

会告诉 OpenGL：从第 0 个顶点开始，连续取 3 个顶点，并按 `GL_TRIANGLES` 的方式画出来。也就是用这 3 个点组成一个三角形。

顶点顺序本身也有意义。这里是“左下 -> 右下 -> 上方”的逆时针顺序。以后讲背面剔除时会用到：OpenGL 可以根据顶点是顺时针还是逆时针，判断一个三角形的正面和背面。第一课先不用急着理解这个，只要知道：**数组里每 3 个数字是一组坐标，3 组坐标组成 1 个三角形。**

### Q: VBO、VAO、`glBindBuffer`、`glBufferData` 这一段是什么意思？

A: 这一段是在做两件事：**创建 GPU 里的对象**，然后**把 CPU 内存里的顶点数组复制到 GPU**。

先看这几行：

```cpp
unsigned int VBO;
unsigned int VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
```

`VBO` 和 `VAO` 都只是一个 `unsigned int` 编号。真正的对象在 OpenGL / GPU 那边，C++ 这里拿到的是“对象 ID”。

- `glGenVertexArrays(1, &VAO)`：生成 1 个 VAO，把编号写到 `VAO` 变量里。
- `glGenBuffers(1, &VBO)`：生成 1 个 Buffer，把编号写到 `VBO` 变量里。

可以先这样区分：

- `VBO`：Vertex Buffer Object，保存顶点数据本身，比如 `vertices` 里的 `x, y, z`。
- `VAO`：Vertex Array Object，保存“这些顶点数据应该怎么读取”的配置，比如每个顶点几个 float、从哪里开始读。

接着：

```cpp
glBindVertexArray(VAO);
```

意思是：把这个 VAO 设为当前正在配置的 VAO。后面和顶点属性有关的设置，会记录到这个 VAO 里。

再看 VBO：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

`glBindBuffer(GL_ARRAY_BUFFER, VBO)` 的意思是：把 `VBO` 绑定到 `GL_ARRAY_BUFFER` 这个位置上。`GL_ARRAY_BUFFER` 可以理解成 OpenGL 专门接收“顶点数组数据”的插槽。

`glBufferData(...)` 才是真正上传数据：

- `GL_ARRAY_BUFFER`：把数据传到当前绑定的顶点缓冲，也就是刚刚绑定的 `VBO`。
- `sizeof(vertices)`：要传多少字节。
- `vertices`：CPU 内存里的顶点数组起始地址。
- `GL_STATIC_DRAW`：告诉 OpenGL，这份数据上传后大概率不会频繁修改，主要用于绘制。

第一课可以这样记：

- `glGen*`：生成对象 ID。
- `glBind*`：选中接下来要操作的对象。
- `glBufferData`：把顶点数据从 CPU 复制到 GPU。
- `VAO` 记读取规则，`VBO` 存顶点数据本体。

### Q: 怎么整体理解 VAO、VBO 和“绑定”？

A: 可以把 OpenGL 想成一台状态机。很多 OpenGL 函数不会直接把“我要操作哪个对象”写在参数里，而是先用 `glBind*` 选中一个对象，再对当前选中的对象执行后续操作。

`绑定` 就是：**把某个对象设为当前操作对象**。

比如：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

第一句先说：“现在 `GL_ARRAY_BUFFER` 这个插槽里放的是 `VBO`。”  
第二句再说：“把这份数据上传到当前 `GL_ARRAY_BUFFER` 插槽里的那个 Buffer。”  
因为前面绑定了 `VBO`，所以数据就进了 `VBO`。

再看 VAO：

```cpp
glBindVertexArray(VAO);
```

这句是说：“接下来我配置的顶点读取规则，都记录到这个 `VAO` 里。”

所以后面的配置会被 VAO 记住：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

它们合起来的意思是：

- 当前顶点数据来自哪个 VBO。
- 0 号顶点属性从这个 VBO 里读。
- 每个顶点属性由 3 个 float 组成。
- 这 3 个 float 要交给 shader 里的 `layout (location = 0) in vec3 aPos;`。
- 启用 0 号顶点属性。

于是绘制时只要：

```cpp
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

OpenGL 就能根据 VAO 里记录的规则，找到 VBO 里的顶点数据，并把它们送进 shader。

一个更稳的心智模型：

- `VBO`：仓库，里面存放原始顶点数据。
- `VAO`：取货说明书，记录从哪个仓库取、每几个数字是一组、每组数字怎么解释。
- `glBindBuffer`：选中当前要操作的仓库。
- `glBindVertexArray`：选中当前要编辑或使用的说明书。
- `glVertexAttribPointer`：往说明书里写“怎么读仓库里的数据”。
- `glDrawArrays`：按当前说明书开始绘制。

第一课最重要的一句话：**VBO 解决“数据放哪”，VAO 解决“数据怎么读”，绑定解决“我现在正在操作谁”。**

### Q: VAO、VBO 和绑定能不能用更形象的方式理解？

A: 可以把这套东西想成“快递仓库发货”。

CPU 这边的 `vertices` 数组像你手里的一张纸，上面写着三角形三个点：

```text
左下点：-0.5, -0.5, 0.0
右下点： 0.5, -0.5, 0.0
上方点： 0.0,  0.5, 0.0
```

但是 GPU 画图不想每一帧都回 CPU 这里拿纸看，它希望数据先放到 GPU 自己附近。所以我们创建 VBO：

```cpp
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

这就像：

1. `glGenBuffers`：开一个 GPU 仓库，拿到仓库编号 `VBO`。
2. `glBindBuffer`：把这个仓库推到当前装货口。
3. `glBufferData`：把纸上的顶点数据搬进这个仓库。

所以 VBO 是：**装顶点原始数据的 GPU 仓库**。

但仓库里只有一串数字：

```text
-0.5, -0.5, 0.0, 0.5, -0.5, 0.0, 0.0, 0.5, 0.0
```

GPU 还不知道这串数字该怎么分组。是每 2 个数字一个点？每 3 个数字一个点？第一个数字是位置还是颜色？所以还需要 VAO。

VAO 像一张“取货单 / 读货说明书”，它记录：

- 去哪个 VBO 仓库取数据。
- 每 3 个 `float` 是一个顶点。
- 这 3 个 `float` 要送到 shader 的 `location = 0`。
- 这个属性是启用状态。

于是：

```cpp
glBindVertexArray(VAO);
```

就像把某张取货单放到当前操作台上。后面设置的读取规则，都会写进这张取货单。

`绑定` 可以理解成“切换当前操作台”：

- `glBindBuffer(GL_ARRAY_BUFFER, VBO)`：当前装货口选中这个 VBO 仓库。
- `glBindVertexArray(VAO)`：当前说明书选中这个 VAO。

最后绘制时：

```cpp
glBindVertexArray(VAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

意思就是：拿出这张取货单，按它记录的规则去仓库取 3 个顶点，然后画一个三角形。

最形象的一句话：

**VBO 是装数字的仓库，VAO 是告诉 GPU 怎么拆这些数字的说明书，bind 是把某个仓库或说明书放到当前操作台。**

### Q: 要画两个大小或位置不同的三角形，VAO/VBO 能封装吗？

A: 可以。新示例 [two_triangles.cpp](src/two_triangles.cpp) 没有改原来的 `main.cpp`，而是单独创建了一个 `TriangleMesh` 类来封装 VAO/VBO。

核心想法是：一个三角形对象自己负责一套 VAO/VBO。

```cpp
class TriangleMesh
{
public:
    explicit TriangleMesh(const std::array<float, 9>& vertices);
    void draw() const;

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
};
```

构造函数里做初始化：

- 创建 VAO/VBO。
- 绑定 VAO。
- 绑定 VBO。
- 把传入的顶点数据上传到 GPU。
- 设置 `glVertexAttribPointer`，告诉 VAO 怎么读 VBO。

绘制时只需要：

```cpp
leftMesh.draw();
rightMesh.draw();
```

每个 `TriangleMesh` 内部会绑定自己的 VAO，然后调用：

```cpp
glDrawArrays(GL_TRIANGLES, 0, 3);
```

所以画两个三角形时，只要准备两份不同的顶点数据：

```cpp
const std::array<float, 9> leftTriangle = { ... };
const std::array<float, 9> rightTriangle = { ... };

TriangleMesh leftMesh(leftTriangle);
TriangleMesh rightMesh(rightTriangle);
```

位置和大小的差异来自顶点坐标本身。比如左边三角形的 x 坐标整体偏负，就会出现在左侧；右边三角形的 x 坐标整体偏正，就会出现在右侧；坐标之间的距离越大，三角形越大。

第一课可以先记住：**如果两个三角形的顶点数据不同，可以创建两个 VBO；如果它们的读取规则也各自保存，就让每个三角形对象有自己的 VAO。**

### Q: 能不能写得更像 Unity 一点？

A: 可以。示例 [unity_style_triangles.cpp](src/unity_style_triangles.cpp) 使用根目录 [unity_style](../../unity_style/README.md) 里的封装，把 OpenGL 的 VAO/VBO 藏进 `Mesh`，把位置和缩放放进 `Transform2D`，再用 `GameObject` 组合起来。

使用时就会更像 Unity：

```cpp
Mesh triangleMesh({
    {-0.5f, -0.5f, 0.0f},
    { 0.5f, -0.5f, 0.0f},
    { 0.0f,  0.5f, 0.0f}
});

GameObject smallTriangle(triangleMesh);
smallTriangle.transform.position = {-0.45f, -0.10f};
smallTriangle.transform.scale = {0.55f, 0.55f};
smallTriangle.material.color = {1.0f, 0.5f, 0.2f, 1.0f};

GameObject tallTriangle(triangleMesh);
tallTriangle.transform.position = {0.35f, 0.05f};
tallTriangle.transform.scale = {0.75f, 1.10f};
tallTriangle.material.color = {0.2f, 0.7f, 1.0f, 1.0f};
```

这里的思路是：

- `Mesh`：类似 Unity 的 Mesh，内部负责 VAO/VBO 和顶点数据上传。
- `Transform2D`：类似 Unity 的 Transform，负责位置和缩放。
- `Material`：保存颜色。
- `GameObject`：把 Mesh、Transform、Material 组合起来。
- `ShaderProgram`：类似材质背后的 shader，负责把 `position`、`scale`、`color` 传给 GPU。

这样就不用每画一个三角形都手写一遍 VAO/VBO。你只需要关心“这个物体用哪个 Mesh、放在哪里、多大、什么颜色”。

不过要注意：这只是学习阶段的薄封装，不是完整引擎。它的价值是让 Unity 开发者更容易建立映射：

- Unity 里改 `transform.position`，这里改 `GameObject::transform.position`。
- Unity 里多个对象可以共用一个 Mesh，这里两个 `GameObject` 也共用同一个 `triangleMesh`。
- Unity 底层帮你处理 GPU Buffer，这里我们自己在 `Mesh` 里处理 VAO/VBO。

以后新的 demo 会优先采用这种写法：底层 OpenGL 细节放在 `unity_style/`，每个 demo 文件尽量像 Unity 场景脚本一样组织对象。

### Q: GLFW 初始化、窗口创建、GLAD 初始化这些能不能也放进基类？

A: 可以。现在 `unity_style/` 里新增了 `lr::Application` 基类。它负责初始化 GLFW、设置 OpenGL 3.3 Core Profile、创建窗口、激活 Context、初始化 GLAD、处理主循环、清屏、Esc 退出和窗口缩放。

demo 代码只需要继承它：

```cpp
class UnityStyleTrianglesDemo : public lr::Application
{
public:
    UnityStyleTrianglesDemo()
        : lr::Application({800, 600, "LearnOpenGL - Unity Style Triangles"})
    {
    }

protected:
    void onStart() override
    {
        // 创建 Mesh / Shader / GameObject。
    }

    void onRender() override
    {
        // 绘制 GameObject。
    }
};
```

最后：

```cpp
int main()
{
    UnityStyleTrianglesDemo app;
    return app.run();
}
```

这样以后 demo 文件就不用反复写 `glfwInit()`、`glfwWindowHint(...)`、`glfwCreateWindow(...)`、`gladLoadGLLoader(...)` 这些模板代码了。
