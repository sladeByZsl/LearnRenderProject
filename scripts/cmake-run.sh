#!/usr/bin/env bash
# 通用 CMake：配置 + 编译（可选运行可执行文件）
#
# 用法示例：
#   ./scripts/cmake-run.sh
#   ./scripts/cmake-run.sh -B out -j 8
#   ./scripts/cmake-run.sh -S demos/01_hello_triangle --run hello_triangle
#   cmake-run.sh -- -DCMAKE_BUILD_TYPE=Release   # 额外参数传给 cmake 配置阶段
#
# 全局使用（任选其一）：
#   ln -s "$(pwd)/scripts/cmake-run.sh" ~/bin/cmake-run && chmod +x ~/bin/cmake-run
#   或把本 scripts 目录加入 PATH
#
# 也可用环境变量设默认：CMAKE_RUN_SOURCE、CMAKE_RUN_BUILD、CMAKE_RUN_JOBS

# 不用 set -u：macOS 自带 Bash 3.2 在 -u 下对空数组 "${arr[@]}" 会误报 unbound
set -eo pipefail

SOURCE="${CMAKE_RUN_SOURCE:-.}"
BUILD="${CMAKE_RUN_BUILD:-build}"
JOBS="${CMAKE_RUN_JOBS:-}"
RUN_EXE=""
EXTRA_CMAKE=()

usage() {
  cat <<'EOF'
用法: cmake-run.sh [选项] [-- 传给 cmake 配置的额外参数]

  -S, --source DIR   源码目录（含 CMakeLists.txt），默认 . 或环境变量 CMAKE_RUN_SOURCE
  -B, --build DIR    构建目录，默认 build 或 CMAKE_RUN_BUILD
  -j, --jobs N       cmake --build 并行数，或设 CMAKE_RUN_JOBS
  --run NAME         编译成功后运行 BUILD/NAME（相对构建目录的可执行文件名）
  -h, --help         显示本说明

示例:
  cmake-run.sh -S demos/01_hello_triangle --run hello_triangle
  cmake-run.sh -j 8 -- -DCMAKE_BUILD_TYPE=Release
EOF
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    -h|--help)
      usage
      exit 0
      ;;
    -S|--source)
      SOURCE="${2:?缺少 -S 目录参数}"
      shift 2
      ;;
    -B|--build)
      BUILD="${2:?缺少 -B 目录参数}"
      shift 2
      ;;
    -j|--jobs)
      JOBS="${2:?缺少 -j 并行数}"
      shift 2
      ;;
    --run)
      RUN_EXE="${2:?缺少 --run 可执行文件名}"
      shift 2
      ;;
    --)
      shift
      EXTRA_CMAKE+=("$@")
      break
      ;;
    *)
      echo "未知参数: $1" >&2
      usage >&2
      exit 1
      ;;
  esac
done

if [[ ! -f "$SOURCE/CMakeLists.txt" ]]; then
  echo "错误: 在 \"$SOURCE\" 下未找到 CMakeLists.txt" >&2
  exit 1
fi

if ((${#EXTRA_CMAKE[@]} > 0)); then
  cmake -S "$SOURCE" -B "$BUILD" "${EXTRA_CMAKE[@]}"
else
  cmake -S "$SOURCE" -B "$BUILD"
fi

build_args=()
if [[ -n "$JOBS" ]]; then
  build_args+=(--parallel "$JOBS")
fi
if ((${#build_args[@]} > 0)); then
  cmake --build "$BUILD" "${build_args[@]}"
else
  cmake --build "$BUILD"
fi

if [[ -n "$RUN_EXE" ]]; then
  exe="$BUILD/$RUN_EXE"
  if [[ ! -x "$exe" ]] && [[ -f "$exe.exe" ]]; then
    exe="$BUILD/$RUN_EXE.exe"
  fi
  if [[ ! -f "$exe" ]]; then
    echo "错误: 找不到可执行文件 \"$exe\"（Windows 下可尝试带 .exe）" >&2
    exit 1
  fi
  exec "$exe"
fi
