---
name: run-opengl-demo
description: Builds and runs LearnOpenGL CMake demos in this repo using scripts/cmake-run.sh. Use when the user attaches a demo .cpp under demos/, asks to run or execute a demo, or mentions cmake-run / framework_triangles / framework_two_triangles / hello_triangle / two_triangles.
disable-model-invocation: true
---

# 运行 OpenGL Demo（learnOpenGL）

## 前提

- 工作区根目录为 **`learnOpenGL` 仓库根**（能看到 `scripts/cmake-run.sh` 与 `demos/`）。
- 需要本机已装 CMake、编译器、GLFW、OpenGL；图形程序需在带桌面的环境运行。

## 要做的事

1. **确定源码目录 `-S`**：用户拖入或提到的 `.cpp` 若在 `demos/<章节>/src/`，则 `-S` 为该章节目录（含 `CMakeLists.txt` 的那一层，例如 `demos/01_hello_triangle`）。
2. **确定可执行文件名 `--run`**：
   - 优先读该章下 `CMakeLists.txt` 里的 `add_executable(目标名 …)`，与用户文件对应的那一行。
   - 快速对照（`01_hello_triangle`）：`main.cpp` → **`hello_triangle`**（不是 `main`）；`two_triangles.cpp` → `two_triangles`；`framework_triangles.cpp` → `framework_triangles`；`framework_two_triangles.cpp` → `framework_two_triangles`。
3. **在仓库根执行**（`cwd` = 仓库根）：

```bash
chmod +x scripts/cmake-run.sh 2>/dev/null || true
./scripts/cmake-run.sh -S "<章节目录>" -B "<章节目录>/build" --run "<目标名>"
```

示例（双三角形 Framework demo）：

```bash
./scripts/cmake-run.sh -S demos/01_hello_triangle -B demos/01_hello_triangle/build --run framework_two_triangles
```

4. **`-B` 说明**：固定用 **`<章节目录>/build`**，避免与仓库根默认 `build/` 混用；若用户已指定其他构建目录则遵从用户。
5. **输出**：简述编译是否成功；若 `--run` 为 GUI，说明窗口应已出现或进程在跑；若有报错，贴关键错误并提示检查 GLFW/CMake 路径。

## 不要做的事

- 不要改 demo 源码，除非用户另要求。
- 不要用 `cd` 进章节目录再手写 `cmake ..`，除非 `cmake-run.sh` 不可用；优先用脚本保持与 `scripts/README.md` 一致。

## 扩展章节

若将来新增 `demos/02_…` 等，**一律先读该目录 `CMakeLists.txt`** 确认 `add_executable` 名称再拼 `--run`。
