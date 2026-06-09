# OpenGL 学习 TODO

这个文件记录当前学习节奏和每日任务拆解。自动提醒负责每天告诉今天学什么；本 TODO 负责让仓库里始终能看到当前进度。

## 当前状态

- 当前阶段：第三周收口，准备进入 Camera / MVP。
- 当前任务：第 21 天，整理 Transform / Model Matrix。
- 当前原则：按计划继续推进；每天先简短总结上一天，如果上一天可能没做就直接说明，然后继续给当天新任务。

## 每日固定流程

1. 看当天学习包，只聚焦 1 个核心概念。
2. 运行当天 demo 的 `./run.sh`。
3. 改一个最小可见点，确认结果变化。
4. 写 3-5 句话学习笔记。
5. 有问题就用 Q&A 形式补到对应 README。

## 整体路线

目标不是把 LearnOpenGL 每一页硬啃完，而是按 Unity 开发者最容易建立迁移关系的顺序推进：

```text
OpenGL 基础管线
-> Texture / UV
-> Transform / Model Matrix
-> Camera / View / Projection
-> Depth / 3D 空间
-> Lighting
-> Material / 小场景
-> Cubemap / Environment
-> PBR
```

## 阶段计划

### 第 1 周：最小渲染管线

目标：知道一个三角形从 CPU 数据到屏幕输出的大概路径。

```text
GLFW / Context
-> NDC 顶点
-> VBO 上传数据
-> VAO 记录读法
-> Shader 使用属性
-> Uniform 传 draw 级参数
-> Draw Call 输出到屏幕
```

最小产出：跑通三角形，能说清 VAO / VBO / Shader / Draw Call 各自负责什么。

### 第 2 周：Texture / UV

目标：理解 Unity 里的贴图在 OpenGL 里如何拆成图片数据、Texture Object、UV、Sampler。

```text
图片文件
-> CPU 加载像素
-> glTexImage2D 上传到 GPU
-> UV 决定采样位置
-> sampler2D 取颜色
-> fragment shader 输出颜色
```

最小产出：画出带贴图的矩形，理解 Wrapping / Filtering / Mix 的直觉。

### 第 3 周：Transform / Model Matrix

目标：把 Unity 的 `Transform.position / rotation / localScale` 对应到 OpenGL 的矩阵。

```text
local vertices
-> translate / rotate / scale
-> model matrix
-> world space
```

最小产出：同一份顶点数据，通过矩阵实现移动、旋转、缩放，并理解变换顺序。

### 第 4 周：Camera / MVP

目标：理解 Unity Camera 背后的 `view` 和 `projection`。

```text
model: local -> world
view: world -> camera
projection: camera -> clip / NDC
```

最小产出：画一个 3D cube，用 camera 观察它。

### 第 5 周：Depth / 3D 空间

目标：理解多个 3D 物体为什么能正确前后遮挡。

```text
depth test
z-buffer
near / far
多个物体前后关系
```

最小产出：多个 cube 在不同深度位置显示，并能解释谁挡住谁。

### 第 6 周：Lighting 入门

目标：理解基础光照不是“贴图颜色直接显示”，而是颜色会被法线和光方向影响。

```text
normal
light direction
ambient
diffuse
specular
Blinn-Phong
```

最小产出：一个 cube 在光照下随角度变化出现明暗。

### 第 7 周：Material / 小场景整合

目标：把 Mesh、Texture、Shader、Transform、Camera、Light 放到一个小场景里。

```text
多个 object
多个 material
texture + lighting
RenderDoc 观察 draw call
```

最小产出：一个简单 3D 场景，可以用 RenderDoc 看懂一次 draw call 里的 buffer、texture、uniform。

### 第 8-9 周：PBR 合理入口

目标：在基础光照和材质上下文足够以后，再进入 PBR。

```text
cubemap / environment map
reflection
albedo
metallic
roughness
ao
Cook-Torrance 最小结构
```

最小产出：跑通一个最小 PBR shader，先理解参数和画面变化，不急着推完整公式。

## PBR 时间判断

PBR 建议在第 55-65 天左右开始。原因是它依赖这些前置概念：

```text
MVP / Camera
Depth
Normal
基础 Lighting
Material 参数
Texture 采样
Cubemap / Environment Map
```

过早学 PBR 容易变成背公式；先把场景、光照、材质和环境贴图走通，理解会稳很多。

## 周总结节奏

每 7 天做一次周总结：

```text
第 07 天：OpenGL 基础管线总结
第 14 天：Texture / UV 总结
第 21 天：Transform / Model Matrix 总结
第 28 天：Camera / MVP 总结
第 35 天：Depth / 3D 总结
第 42 天：Lighting 总结
第 49 天：Material / 小场景总结
第 56 天：PBR 预备总结
```

## 自动提醒规则

- 每天应该告诉“今天学什么”。
- 每天开头先自动总结上一天的主题、可能产出和关键一句话。
- 如果上一天可能没做，直接说“昨天可能没做，没关系”，不要卡住进度。
- 总结上一天之后，继续给当天的新任务、3-4 条 TODO 和最小输出目标。
- 如果当天是第 7 天、第 14 天这类节点，优先做周总结。
- 每个新 demo 默认包含 `README.md`、`CMakeLists.txt`、`run.sh`、`src/main.cpp`。
- 复盘图片、截图、卡片按课程放到 `notes/recaps/dayXX_*` 目录。
