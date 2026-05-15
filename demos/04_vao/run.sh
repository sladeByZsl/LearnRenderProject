#!/bin/bash
set -e  # 任意命令失败立刻退出，不要静默继续

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"  # 脚本所在目录的绝对路径
BUILD_DIR="$SCRIPT_DIR/build"                # 构建目录，固定放在脚本同级

# 首次需要 configure（只判断目录，不判断可执行文件）
if [ ! -d "$BUILD_DIR" ]; then
    cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
fi

# 每次都 build，CMake 自动增量编译；有代码改动时，下次 run.sh 会立即生效
cmake --build "$BUILD_DIR" -j "$(sysctl -n hw.logicalcpu)"

case "${1:-basic}" in
    basic)
        "$BUILD_DIR/vao"
        ;;
    color)
        "$BUILD_DIR/vao_color_attributes"
        ;;
    split)
        "$BUILD_DIR/vao_split_buffers"
        ;;
    *)
        echo "Usage: ./run.sh [basic|color|split]"
        exit 1
        ;;
esac
