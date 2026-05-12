# Framework Python 绑定 (`lr_framework`)

在 **C++ 主线程** 中跑 GLFW + OpenGL（与 [../](../) 中 Framework 相同），用 **Python** 写 `on_start` / `on_update` / `on_render` / `on_shutdown`（对应 C++ 的 `onStart` 等）。

## 依赖

- 与 `demos/01_hello_triangle` 相同：CMake、C++17、GLFW3、OpenGL。
- **Python 3.8+**，且需安装对应版本的 **开发头文件**（CMake 组件 `Development.Module`）。  
  - macOS Homebrew：`brew install python` 通常已含。  
  - 若配置时提示未找到，可关闭模块构建：`-DLEARNOGL_BUILD_PYTHON_MODULE=OFF`。

## 构建

在 `demos/01_hello_triangle` 下配置并编译（会拉取 pybind11，并生成 `lr_framework` 扩展）：

```bash
cd demos/01_hello_triangle
cmake -B build
cmake --build build
```

模块默认输出到 **`build/python/`**（例如 `lr_framework.cpython-313-darwin.so`）。

## 运行示例

将 `PYTHONPATH` 指向上面的 `build/python`，再运行双三角 demo：

```bash
export PYTHONPATH="$(pwd)/build/python"
python3 ../Framework/python_binding/framework_two_triangles_demo.py
```

（在 `demos/01_hello_triangle` 目录下执行时，`$(pwd)/build/python` 即为模块目录。）

若从仓库根目录运行，可先设置：

```bash
export PYTHONPATH="$PWD/demos/01_hello_triangle/build/python"
python3 demos/Framework/python_binding/framework_two_triangles_demo.py
```

**注意**：扩展模块与解释器版本需一致（例如用 `python3.13` 构建则运行时也应用 3.13）。

## API 概要

- `ApplicationConfig`：字段 `width`、`height`、`title`、`shader_dir`、`gl_major`、`gl_minor`。
- 继承 `Application`，实现 `on_start`、`on_update`、`on_render`、`on_shutdown`（蛇形命名）。
- `load_shader(vertex_file, fragment_file)`：相对 `shader_dir` 的文件名。
- `Mesh.triangle()`、`Scene.create_game_object(...)`、`scene.draw(shader)` 等与 C++ Framework 一致。
