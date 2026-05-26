# learnOpenGL

这个目录是 OpenGL / 图形学轻学习的工作区。

学习节奏：

- 每天只聚焦 1 个核心概念。
- 每天做一个 1 小时以内的最小 demo 或观察目标。
- 每天只写 3-5 句话学习笔记。
- 每 7 天把本周笔记整理成 1 篇公众号文章。

第一周路线：

1. 第 01 天：只跑出三角形。
2. 第 02 天：只理解顶点数据。
3. 第 03 天：只理解 VBO。
4. 第 04 天：只理解 VAO。
5. 第 05 天：只理解 Shader。
6. 第 06 天：改颜色、改位置。
7. 第 07 天：整理第一周笔记。

核心原则：先会用，再理解；先看到结果，再补原理。

## Q&A 记录规则

以后学习过程中提出的问题，都按问答形式沉淀到对应 README：

- 项目通用问题：记录到本 README。
- 某一节课的问题：记录到对应 demo 的 README。
- 问题格式：`Q: ...` / `A: ...`。
- 回答尽量保留当时的上下文、原因和最小可执行结论，方便之后复习。

## Demos

- [01 Hello Triangle](demos/01_hello_triangle/README.md)
- [02 Vertex Data](demos/02_vertex_data/README.md)
- [03 VBO](demos/03_vbo/README.md)
- [04 VAO](demos/04_vao/README.md)
- [05 Shader Attributes](demos/05_shader_attributes/README.md)
- [06 Uniform](demos/06_uniform/README.md)

后续每天开始学习时，默认先创建当天独立 demo 目录、README 和 `run.sh`。当天 demo 尽量只服务一个核心概念，避免把新知识挤在同一个文件里。

`run.sh` 统一遵循：脚本同级放 `build` 目录；首次运行只在 `build` 目录不存在时执行 CMake configure；之后每次都执行 `cmake --build` 做增量编译，再运行目标程序。这样改完代码后再次执行 `./run.sh` 会立即生效。

## 复盘

- [学习复盘](notes/recaps/README.md)
- [旧笔记索引](notes/README.md)
- [学习 TODO](notes/learning_todo.md)

## Framework

- [Framework](demos/Framework/README.md)
- [Framework Python 绑定](demos/Framework/python_binding/README.md)

后续 demo 默认优先使用 `demos/Framework/` 里的封装来写：demo 代码关注 `Mesh`、`Transform2D`、`Material`、`GameObject`，底层 VAO/VBO/Shader Program 细节放在封装层里。

如果想用 Python 写上层 demo 逻辑，可以先编译 `lr_framework` 绑定模块，再运行 `demos/01_py_demo/` 里的示例。
