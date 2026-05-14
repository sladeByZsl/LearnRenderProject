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

### Q: “绑定点”是什么？

A: 绑定点可以理解成 OpenGL 里预先规定好的“操作入口”或“插槽”。你不能随便对一个 GPU 对象调用所有操作，而是要先把对象绑定到某个对应的绑定点上，再通过这个绑定点操作它。

比如：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

这里 `GL_ARRAY_BUFFER` 就是绑定点。它的含义是：当前要操作的是“顶点数组缓冲区”。`VBO` 是具体对象，`GL_ARRAY_BUFFER` 是 OpenGL 提供的操作位置。

可以用这个模型记：

```text
具体对象：VBO1、VBO2、VBO3
绑定点：GL_ARRAY_BUFFER
当前状态：GL_ARRAY_BUFFER -> VBO2
后续操作：glBufferData(GL_ARRAY_BUFFER, ...) 实际改的是 VBO2
```

绑定点不是对象本身，而是对象被接到 OpenGL 管线上的位置。换句话说：**对象负责存数据，绑定点负责告诉 OpenGL 当前要按哪种用途操作这个对象。**

后面还会看到很多类似绑定点：

- `GL_ARRAY_BUFFER`：绑定 VBO，表示顶点属性数据。
- `GL_ELEMENT_ARRAY_BUFFER`：绑定 EBO，表示索引数据。
- `GL_TEXTURE_2D`：绑定 2D 纹理。
- `GL_FRAMEBUFFER`：绑定帧缓冲。

所以现在先记一句话：**绑定点是 OpenGL 的操作插槽，绑定对象以后，后续 API 通过这个插槽找到真正要操作的对象。**

### Q: 我能看到 OpenGL 的当前状态吗？

A: 能看到一部分，但 OpenGL 没有一个统一的“打印全部当前状态”按钮。通常有两种方式：

第一种是用 `glGet*` 查询某个具体状态。例如想知道当前 `GL_ARRAY_BUFFER` 绑定的是哪个 VBO，可以这样查：

```cpp
int currentArrayBuffer = 0;
glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentArrayBuffer);
std::cout << "Current GL_ARRAY_BUFFER = " << currentArrayBuffer << std::endl;
```

如果前面执行过：

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
```

那么 `currentArrayBuffer` 打印出来的值通常就会等于 `VBO` 的 ID。

常见查询例子：

```cpp
GL_ARRAY_BUFFER_BINDING          // 当前 GL_ARRAY_BUFFER 绑定的 VBO
GL_VERTEX_ARRAY_BINDING          // 当前绑定的 VAO
GL_CURRENT_PROGRAM               // 当前使用的 shader program
GL_VIEWPORT                      // 当前 viewport
```

第二种是用图形调试工具，比如 RenderDoc、Xcode GPU Frame Capture。这类工具可以抓一帧，看到 draw call 当时绑定了哪些 buffer、shader、texture、uniform，比自己到处写 `glGet*` 更适合排查复杂渲染问题。

学习阶段可以先记：**可以查，但要按状态项一个一个查；真正工程调试通常会用图形调试器看整帧状态。**

### Q: 我这个工程能抓 GPU 帧吗？

A: 当前工程是 macOS + GLFW + OpenGL。结论要分情况：

- 在当前 macOS 环境下，不建议把“抓完整 GPU 帧”作为今天的学习目标。Apple 已经弃用 OpenGL，Xcode 的现代 GPU Frame Capture 主要面向 Metal；当前这种 GLFW/OpenGL 小工程，用 Xcode 抓帧不一定稳定、入口也不一定可用。
- RenderDoc 很适合学习图形调试，但官方支持主要是 Windows / Linux / Android。它支持 OpenGL 3.2 - 4.6 Core Profile；如果以后把这个工程放到 Windows 或 Linux 上跑，当前 OpenGL 3.3 Core Profile demo 就很适合用 RenderDoc 抓帧。
- 现在最实用的做法，是先在代码里用 `glGet*` 查询关键状态，比如当前 VBO、VAO、shader program。等后面想系统学习图形调试时，再专门安排一天用 RenderDoc 或 Xcode GPU 工具。

参考：

- [RenderDoc README - API Support](https://github.com/baldurk/renderdoc#api-support)
- [Apple Developer - Capturing a Metal workload in Xcode](https://developer.apple.com/documentation/metal/frame_capture_debugging_tools/capturing_a_frame_using_a_breakpoint)

### Q: 那 Xcode 呢？这个工程能用 Xcode 抓吗？

A: Xcode 要分成两种用途看：

第一，**用 Xcode 编译、运行、断点调试 C++ 代码是可以的**。这个工程是 CMake 工程，可以生成 Xcode 工程：

```bash
cd /Users/zsl/Documents/work/learnOpenGL/demos/03_vbo
cmake -S . -B build-xcode -G Xcode
```

然后打开：

```bash
open build-xcode/vbo.xcodeproj
```

这适合看 C++ 变量、打断点、单步执行，例如观察 `VBO` 的整数 ID、`vertices[]` 数组内容。

第二，**用 Xcode GPU Frame Capture 抓当前这个 macOS GLFW/OpenGL 工程，不建议作为主要方案**。Apple 当前文档里的 Xcode GPU Frame Capture 主要围绕 Metal；文档也明确提到 GPU Frame Capture 选项捕获的是 Metal 或 OpenGL ES API usage，而 macOS 桌面 OpenGL 已经从 macOS 10.14 开始被弃用。

所以现在最实用的结论是：

- C++ 代码调试：可以用 Xcode。
- OpenGL 状态学习：先用 `glGet*`。
- 完整 GPU 帧调试：以后优先在 Windows / Linux 上用 RenderDoc，或者等学 Metal 时用 Xcode GPU Frame Capture。

参考：

- [Apple Developer - Capturing a Metal workload in Xcode](https://developer.apple.com/documentation/metal/frame_capture_debugging_tools/capturing_a_frame_using_a_breakpoint)
- [Apple Developer - About OpenGL for OS X](https://developer.apple.com/opengl/)

### Q: 我在台式机上用 RenderDoc 抓 Unity，能和现在学的 VBO 对上吗？

A: 能对上，而且这是很好的观察方式。你截图里是 RenderDoc 抓到的 Unity D3D11 帧，左边是 draw call 列表，中间和右边是资源状态。

其中看到类似：

```text
D3D11Device::CreateBuffer(..., D3D11_BIND_VERTEX_BUFFER)
Buffer-1-1048576
Map(Buffer-...)
Unmap(Buffer-...)
GUITexture.Draw
```

可以这样对应：

- `D3D11_BIND_VERTEX_BUFFER`：DirectX 里的顶点缓冲用途，概念上对应 OpenGL 的 VBO。
- `Buffer-*`：GPU 资源对象，类似 OpenGL 里的某个 VBO ID。
- `Map / Unmap`：CPU 临时写入 GPU buffer 的方式，概念上接近“把 CPU 数据传给 GPU buffer”，但 DirectX 的 API 形式和 OpenGL 的 `glBufferData` 不一样。
- `GUITexture.Draw` / `DrawIndexed`：一次绘制调用，对应 OpenGL 里的 `glDrawArrays` / `glDrawElements`。

所以今天的 VBO 概念可以迁移过去：

```text
OpenGL:  glBindBuffer(GL_ARRAY_BUFFER, VBO) + glBufferData(...)
DirectX: CreateBuffer / Map / Unmap + IASetVertexBuffers(...)
共同点：都是把顶点数据放到 GPU buffer，draw call 再读取它
```

注意不要把 API 名字硬对应成一模一样。OpenGL 是绑定点状态机风格，DirectX 更像显式设置管线阶段资源。但底层目标一致：让 GPU 在绘制时能读到顶点数据。

### Q: RenderDoc 里看起来是先绑定一堆 buffer，然后调用一次 draw 吗？

A: 对，基本可以这么理解，但更准确地说是：**先设置一整套管线状态和资源，最后一次 draw call 使用当前这套状态去绘制**。

你截图里选中的事件是：

```text
DrawIndexed(48)
```

它前面有类似这些调用：

```text
PSSetShaderResources(...)
PSSetSamplers(...)
OMSetBlendState(...)
IASetVertexBuffers(...)
IASetIndexBuffer(...)
VSSetShader(...)
PSSetShader(...)
VSSetConstantBuffers(...)
Map(...)
Unmap(...)
DrawIndexed(48)
```

可以这样拆：

- `IASetVertexBuffers`：给 Input Assembler 阶段绑定顶点缓冲，概念上对应 OpenGL 的 VBO。
- `IASetIndexBuffer`：绑定索引缓冲，概念上对应 OpenGL 的 EBO / IBO。
- `VSSetShader` / `PSSetShader`：绑定顶点着色器和像素着色器。
- `VSSetConstantBuffers`：绑定常量缓冲，类似把 uniform 数据给 shader。
- `PSSetShaderResources`：绑定纹理等 shader resource。
- `PSSetSamplers`：绑定采样器。
- `OMSetBlendState`：设置输出合并阶段的混合状态。
- `Map` / `Unmap`：CPU 更新某个 buffer 的内容。
- `DrawIndexed(48)`：真正发出绘制命令，用当前设置好的所有状态画 48 个索引。

所以 draw call 可以理解成“按当前配置开画”。在它之前设置的 buffer、shader、texture、blend state 等，都只是把绘制需要的材料和规则准备好；到了 `DrawIndexed`，GPU 才按照这些状态执行一次绘制。

对应到 OpenGL，结构类似：

```cpp
glUseProgram(shader);
glBindVertexArray(VAO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBindTexture(GL_TEXTURE_2D, texture);
glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
```

前面几行是设置当前状态，最后一行 `glDrawElements` 才是真正的绘制命令。

### Q: 前面很多次 `Map` / `Unmap` 算 draw call 吗？

A: 不算。`Map` / `Unmap` 是资源更新调用，不是绘制调用。

在 DirectX 里：

- `Map(buffer)`：CPU 取得一段 buffer 的可写/可读访问权。
- CPU 写入数据：比如更新顶点、索引、常量 buffer。
- `Unmap(buffer)`：CPU 写完了，把访问权还给 GPU/驱动。

这一步只是“准备数据”，还没有让 GPU 画东西。真正算 draw call 的是：

```text
Draw(...)
DrawIndexed(...)
DrawInstanced(...)
DrawIndexedInstanced(...)
```

你截图里如果看到：

```text
Map(ConstantBuffer-...)
Unmap(ConstantBuffer-...)
DrawIndexed(48)
```

那可以理解成：

```text
Map / Unmap       更新 shader 要用的参数
DrawIndexed(48)   按当前状态真正画一次
```

所以统计 draw call 数量时，不数 `Map` / `Unmap`，只数 `Draw*` 这类绘制命令。

对应到 OpenGL：

- `glBufferData` / `glBufferSubData`：更新 buffer 数据，不算 draw call。
- `glUniform*`：更新 shader 参数，不算 draw call。
- `glDrawArrays` / `glDrawElements`：真正绘制，算 draw call。

### Q: 为什么 RenderDoc 里会有那么多次 `Map` / `Unmap`？

A: 因为一帧里不只画一个东西，也不只更新一份数据。Unity 在渲染 UI、文字、图片、材质参数、相机参数时，可能会频繁更新不同的 GPU buffer，所以 RenderDoc 里会看到很多次 `Map` / `Unmap`。

常见原因：

- **更新常量 buffer**：每个 draw 可能有不同的矩阵、颜色、裁剪区域、材质参数。
- **更新动态顶点/索引 buffer**：UI、文字、动态图形经常每帧重新生成一批顶点和索引。
- **分批绘制不同元素**：不同纹理、不同材质、不同裁剪状态，可能需要拆成多个批次。
- **引擎内部复用大 buffer**：Unity 可能用一块动态 buffer 反复 `Map`、写入一段数据、`Unmap`，再给后面的 draw 使用。

从你截图看，很多事件是 `GUITexture.Draw`，这通常意味着当前帧里有很多 UI/GUI 相关绘制。UI 系统经常会更新小块数据，所以 `Map` / `Unmap` 数量会比较多。

可以这样理解：

```text
Map / Unmap 很多：说明 CPU 正在频繁准备或更新 GPU 要用的数据
Draw / DrawIndexed 很多：说明 GPU 收到了很多次绘制命令
```

这两者相关，但不是同一件事。`Map` / `Unmap` 多，不一定等于 draw call 多；draw call 数量还是要看 `Draw*` 事件。

后面做性能分析时，通常会同时关注：

- draw call 数量是否太多；
- buffer 更新是否太频繁；
- 是否存在很多小批次；
- 是否能通过 batching / instancing / 合并 UI 来减少状态切换和资源更新。

### Q: 在 Windows 上怎么用 RenderDoc 看到我这个程序的各种 buffer？

A: 可以。Windows 是更适合抓这个 OpenGL demo 的环境。RenderDoc 支持 Windows 上的 OpenGL / D3D11 / D3D12 / Vulkan；我们的 demo 是 OpenGL 3.3 Core Profile，适合用 RenderDoc 抓帧。

推荐流程：

1. 在 Windows 上准备工程。

   ```powershell
   git clone git@github.com:sladeByZsl/LearnRenderProject.git
   cd LearnRenderProject\demos\03_vbo
   ```

   需要提前装好 CMake、Visual Studio Build Tools 或 Visual Studio、GLFW。GLFW 可以用 vcpkg、MSYS2 或手动安装，只要 `find_package(glfw3 REQUIRED)` 能找到即可。

2. 先确认程序能独立跑起来。

   ```powershell
   cmake -S . -B build
   cmake --build build --config Release
   .\build\Release\vbo.exe
   ```

   如果生成器不是 Visual Studio，exe 路径可能是：

   ```powershell
   .\build\vbo.exe
   ```

3. 打开 RenderDoc，进 `Launch Application`。

   - `Executable Path` 选择 `vbo.exe`。
   - `Working Directory` 选择 `demos\03_vbo` 或 `vbo.exe` 所在目录。
   - 点击 `Launch`。

4. 程序窗口出现后，在 RenderDoc 里点 `Capture Frame(s) Immediately`，或者按 RenderDoc 覆盖层提示的快捷键抓一帧。

5. 双击打开 capture，看这几个位置：

   - `Event Browser`：选中 `glDrawArrays` 那个 draw call。
   - `Pipeline State`：看当前绑定的 shader、VAO、vertex input / attributes。
   - `Mesh Viewer`：看顶点输入数据，应该能看到类似 `-0.65, -0.45, 0.0` 这些坐标。
   - `Resource Inspector` 或 buffer 相关视图：看 buffer 对象本身。

你截图时优先截这三张：

- 选中 `glDrawArrays` 的 `Event Browser`。
- `Pipeline State` 里 vertex input / buffer / attributes 的位置。
- `Mesh Viewer` 里顶点表格。

这样我就能帮你把 RenderDoc 里的 buffer、attribute、draw call，和代码里的 `VBO`、`glVertexAttribPointer`、`glDrawArrays` 一一对上。

参考：

- [RenderDoc GitHub - API Support](https://github.com/baldurk/renderdoc#api-support)
