#!/bin/bash
set -e  # 任意命令失败立刻退出，不要静默继续

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"  # 脚本所在目录的绝对路径
BUILD_DIR="$SCRIPT_DIR/../01_hello_triangle/build"

# 首次需要 configure（只判断目录，不判断可执行文件）
if [ ! -d "$BUILD_DIR" ]; then
    cmake -S "$SCRIPT_DIR/../01_hello_triangle" -B "$BUILD_DIR"
fi

# 每次都 build，CMake 自动增量编译；有绑定或 Framework 改动时，下次 run.sh 会立即生效
cmake --build "$BUILD_DIR" -j "$(sysctl -n hw.logicalcpu)"

PYTHONPATH="$BUILD_DIR/python" python3 "$SCRIPT_DIR/one.py"
