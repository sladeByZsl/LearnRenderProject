# learnOpenGL

这个目录是 OpenGL / 图形学轻学习的工作区。

学习节奏：

- 每天只聚焦 1 个核心概念。
- 每天做一个 1 小时以内的最小 demo 或观察目标。
- 每天只写 3-5 句话学习笔记。
- 每 7 天把本周笔记整理成 1 篇公众号文章。

整体路线：

1. 第 1 周：OpenGL 基础管线，理解 VAO / VBO / Shader / Draw Call。
2. 第 2 周：Texture / UV，理解图片如何变成 fragment shader 的颜色。
3. 第 3 周：Transform / Model Matrix，对应 Unity 的 position / rotation / scale。
4. 第 4 周：Camera / MVP，对应 Unity Camera；Unity 里写 UV 滚动、UV 扭曲、Mask。
5. 第 5 周：Depth / 3D 空间，理解前后遮挡和 z-buffer；Unity 里写 Outline / Highlight。
6. 第 6 周：Lighting，理解 normal、diffuse、specular；Unity 里写 Rim / Toon / MatCap。
7. 第 7 周：Material / 小场景，把 Mesh、Texture、Shader、Transform、Camera、Light 整合起来；Unity 里写 NormalMap / Dissolve / Flow。
8. 第 8-9 周：PBR 入口，先补 Cubemap / Environment，再进入 metallic / roughness / Cook-Torrance；Unity 里写 Fresnel / Water / Glass。
9. PBR 以后：IBL、Shadow、HDR、Post Processing、Deferred、Transparent、性能调试、URP / HDRP 对照。

核心原则：先会用，再理解；先看到结果，再补原理。

从第 4 周开始采用双线：

- OpenGL 线：继续理解底层管线和渲染原理。
- Unity Shader 线：Shader 效果实战都在 Unity 里写，便于直接迁移到项目。

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
- [整体学习路线图](notes/roadmap/README.md)
- [旧笔记索引](notes/README.md)
- [学习 TODO](notes/learning_todo.md)

## Q&A

### Q: 什么时候能学到 PBR？

A: 按当前节奏，PBR 建议在第 55-65 天左右开始。先完成 Camera / MVP、Depth、基础光照、Material、小场景和 Cubemap / Environment Map，再进入 PBR 会更稳。过早学 PBR 容易变成背公式；晚一点进入，才能把 `albedo / metallic / roughness / ao` 和真实渲染管线对应起来。

### Q: Shader 效果实战在哪里写？

A: Shader 效果实战放到 Unity 里写。OpenGL demo 继续用来理解底层管线；Unity demo 用来实现 UV 动画、Rim、Toon、Dissolve、Flow、Fresnel、Water、Glass、Bloom、Outline、Shield 等可见效果。

## Framework

- [Framework](demos/Framework/README.md)
- [Framework Python 绑定](demos/Framework/python_binding/README.md)

后续 demo 默认优先使用 `demos/Framework/` 里的封装来写：demo 代码关注 `Mesh`、`Transform2D`、`Material`、`GameObject`，底层 VAO/VBO/Shader Program 细节放在封装层里。

如果想用 Python 写上层 demo 逻辑，可以先编译 `lr_framework` 绑定模块，再运行 `demos/01_py_demo/` 里的示例。
