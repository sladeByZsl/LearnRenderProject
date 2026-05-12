#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/../01_hello_triangle/build"

# 若模块不存在则先构建
if [ ! -d "$BUILD_DIR/python" ] || [ -z "$(ls "$BUILD_DIR/python"/lr_framework*.so 2>/dev/null)" ]; then
    echo ">>> 首次构建 lr_framework..."
    cmake -S "$SCRIPT_DIR/../01_hello_triangle" -B "$BUILD_DIR"
    cmake --build "$BUILD_DIR" -j "$(sysctl -n hw.logicalcpu)"
fi

PYTHONPATH="$BUILD_DIR/python" python3 "$SCRIPT_DIR/one.py"
