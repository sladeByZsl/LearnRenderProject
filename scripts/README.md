# cmake-run.sh

在任意含 `CMakeLists.txt` 的工程里，一条命令完成 **配置 + 编译**，并可选择 **启动生成的可执行文件**。适合本地小 demo、LearnOpenGL 章节工程，或你不想记一长串 `cmake` 命令时使用。

## 依赖

- Bash（macOS / Linux 自带即可）
- 已安装 [CMake](https://cmake.org/)，且工程所需的编译器、库（如 GLFW）已就绪

赋予执行权限（克隆后若未标记可执行时执行一次）：

```bash
chmod +x scripts/cmake-run.sh
```

## 快速开始

在**含有 `CMakeLists.txt` 的目录**下：

```bash
/path/to/learnOpenGL/scripts/cmake-run.sh
```

默认等价于：`cmake -S . -B build`，再 `cmake --build build`。

本仓库从根目录编译并运行第一节课 demo：

```bash
./scripts/cmake-run.sh -S demos/01_hello_triangle --run hello_triangle
```

## 命令行选项

| 选项 | 说明 |
|------|------|
| `-S DIR` / `--source DIR` | 源码目录（其下必须有 `CMakeLists.txt`）。默认 `.` |
| `-B DIR` / `--build DIR` | 构建输出目录。默认 `build` |
| `-j N` / `--jobs N` | 传给 `cmake --build` 的 `--parallel N` |
| `--run NAME` | 编译成功后执行 `BUILD/NAME`（`NAME` 为相对构建目录的可执行文件名，不含路径） |
| `-h` / `--help` | 打印内置帮助 |
| `--` | **之后**的所有参数会原样追加到**配置阶段**的 `cmake -S … -B …` 后面（例如生成器、缓存变量） |

`--run` 在 macOS / Linux 下会尝试 `BUILD/NAME`；若不可执行但存在 `BUILD/NAME.exe`，会改用带 `.exe` 的路径（便于在部分 Windows 环境下使用）。

## 环境变量（默认值）

若未在命令行指定对应选项，可使用：

| 变量 | 作用 |
|------|------|
| `CMAKE_RUN_SOURCE` | 等价于 `-S` |
| `CMAKE_RUN_BUILD` | 等价于 `-B` |
| `CMAKE_RUN_JOBS` | 等价于 `-j` |

命令行选项优先于环境变量。

## 全局当命令用（可选）

把脚本链到个人 `bin`，并保证 `~/bin` 在 `PATH` 中：

```bash
mkdir -p ~/bin
ln -sf /绝对路径/learnOpenGL/scripts/cmake-run.sh ~/bin/cmake-run
chmod +x ~/bin/cmake-run
```

之后在任意 CMake 工程目录可写：

```bash
cmake-run -S . -B build -j 8 --run 你的目标名
```

也可以把 `scripts` 目录加入 `PATH`，用脚本全名调用。

## 示例

仅配置并编译（使用默认 `build/`）：

```bash
cmake-run.sh -S demos/01_hello_triangle
```

指定构建目录与并行数：

```bash
cmake-run.sh -S demos/01_hello_triangle -B out -j 8
```

Release 配置（参数写在 `--` 之后，交给 `cmake` 配置阶段）：

```bash
cmake-run.sh -S demos/01_hello_triangle -- -DCMAKE_BUILD_TYPE=Release
```

使用 Ninja 生成器（需已安装 Ninja）：

```bash
cmake-run.sh -S demos/01_hello_triangle -B build-ninja -- -GNinja
```

## 与手写命令的对应关系

脚本内部等价于：

1. `cmake -S <SOURCE> -B <BUILD> [你写在 -- 后面的参数…]`
2. `cmake --build <BUILD> [--parallel N]`（若指定了 `-j`）

若带了 `--run`，则在第 2 步成功后执行 `<BUILD>/<NAME>`。

## 限制说明

- `--run` 只接受**构建目录下的单个可执行文件名**，不会自动猜测目标名；名称需与 CMake 里 `add_executable(...)` 一致。
- 脚本不做「安装」步骤（`cmake --install`）；若需要安装，请在编译成功后自行执行安装命令。
