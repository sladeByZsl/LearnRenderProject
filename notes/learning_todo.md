# OpenGL 学习 TODO

这个文件记录当前学习节奏和每日任务拆解。自动提醒负责每天告诉今天学什么；本 TODO 负责让仓库里始终能看到当前进度。

## 当前状态

- 当前阶段：PBR-first 加速路线。
- 当前任务：直接进入 PBR 原理，从材质参数和能量直觉开始。
- 当前原则：按计划继续推进；每天先简短总结上一天，如果上一天可能没做就直接说明，然后继续给当天新任务。

## 每日固定流程

1. 看当天学习包，只聚焦 1 个核心概念。
2. 运行当天 demo 的 `./run.sh`。
3. 改一个最小可见点，确认结果变化。
4. 写 3-5 句话学习笔记。
5. 有问题就用 Q&A 形式补到对应 README。

## 整体路线

目标不是把 LearnOpenGL 每一页硬啃完，而是按 Unity 开发者最容易建立迁移关系的顺序推进。

从第 4 周开始使用“双线计划”：

```text
OpenGL 线：理解底层渲染管线、数据、状态、矩阵、光照。
Unity Shader 线：把可见效果实战写在 Unity 里，沉淀成项目能复用的 shader demo。
```

当前改为 PBR-first 加速路线。前面的 Camera、Depth、Lighting、Cubemap 不再作为硬性前置，而是遇到 PBR 需要时再补。

PBR-first 主线：

```text
PBR 解决什么问题
-> Albedo / Metallic / Roughness / AO
-> 能量守恒直觉
-> Diffuse / Specular 分工
-> BRDF 是什么
-> Cook-Torrance 结构
-> Fresnel / NDF / Geometry
-> IBL / Environment
-> Unity PBR Shader 实战
```

Unity Shader 实战线：

```text
UV 动画 / 遮罩
-> Rim Light / Toon / MatCap
-> Normal Map / Dissolve / Flow
-> Fresnel / Water / Glass
-> Bloom / Outline / Fullscreen Effect
-> Shield / Fire / Transparent Effect
```

## 阶段计划

### PBR-first 加速计划

目标：直接从 PBR 原理开始，不等完整 Camera / Lighting 路线学完。

核心策略：

```text
先建立 PBR 材质参数直觉
-> 再看公式模块分别解决什么问题
-> Unity 里做调参和效果观察
-> OpenGL / RenderDoc 用来理解底层数据和状态
```

第一阶段：PBR 参数直觉

```text
albedo：物体本色，不包含光照
metallic：金属和非金属的分界
roughness：表面微观粗糙度，决定高光宽窄和反射清晰度
ao：环境遮蔽，近似缝隙和角落里环境光更弱
```

最小产出：Unity 里做一组材质球，横轴 roughness，纵轴 metallic，观察画面差异。

第二阶段：PBR 光照结构

```text
最终颜色 = diffuse + specular
diffuse 负责非金属的漫反射
specular 负责高光和反射
金属通常没有明显 diffuse，本色更多体现在 specular 里
```

最小产出：用同一盏灯观察金属球、塑料球、粗糙球、光滑球。

第三阶段：Cook-Torrance 只看结构

```text
F: Fresnel，视角越擦边反射越强
D: NDF，微表面法线有多少朝向半程向量
G: Geometry，微表面之间的遮挡和自遮蔽
```

最小产出：能说清 Cook-Torrance 三个模块各自管什么，不要求推导公式。

第四阶段：IBL / 环境光照

```text
直接光：来自灯
间接光：来自环境
diffuse IBL：模糊环境光
specular IBL：按 roughness 采样不同模糊程度的环境反射
```

最小产出：一个金属球在 skybox 下有环境反射，并能通过 roughness 改变反射清晰度。

第五阶段：Unity PBR Shader 实战

```text
Fresnel
Clear Coat 初印象
Water
Glass
Anisotropy 初印象
材质调参卡片
```

最小产出：一组 Unity PBR 材质 demo 和对应截图。

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

Unity Shader 实战：UV 滚动、UV 扭曲、圆形遮罩。重点对应 Unity 里的 `_Time`、Tiling / Offset、Mask。

### 第 5 周：Depth / 3D 空间

目标：理解多个 3D 物体为什么能正确前后遮挡。

```text
depth test
z-buffer
near / far
多个物体前后关系
```

最小产出：多个 cube 在不同深度位置显示，并能解释谁挡住谁。

Unity Shader 实战：边缘高亮、简单 Outline、深度相关观察。重点先建立“屏幕上的遮挡和深度有关”的直觉。

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

Unity Shader 实战：Lambert、Half-Lambert、Blinn-Phong、Rim Light、Toon Ramp、MatCap。

### 第 7 周：Material / 小场景整合

目标：把 Mesh、Texture、Shader、Transform、Camera、Light 放到一个小场景里。

```text
多个 object
多个 material
texture + lighting
RenderDoc 观察 draw call
```

最小产出：一个简单 3D 场景，可以用 RenderDoc 看懂一次 draw call 里的 buffer、texture、uniform。

Unity Shader 实战：Normal Map、Emission、Dissolve、Flow Map、Detail Texture。

### 原第 8-9 周：PBR 合理入口

原计划是在基础光照和材质上下文足够以后再进入 PBR。现在改为 PBR-first，这一阶段提前执行。

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

Unity Shader 实战：Metallic / Roughness 调参球、Fresnel、简单水面、简单玻璃。

### PBR 之后：现代渲染效果线

目标：从“会写一个材质模型”进入“理解现代渲染器怎么组织画面”。

```text
IBL / Environment
-> Shadow
-> HDR / Tone Mapping
-> Post Processing
-> Deferred Rendering
-> Transparent / Blending
-> Performance / RenderDoc / Unity Frame Debugger
-> URP / HDRP 对照
```

Unity Shader 实战继续放在 Unity 项目里写：

```text
Bloom
Outline
Fullscreen Blur
Edge Detection
Shield
Fire
Water
Glass
大量物体 Instancing
```

## PBR 时间判断

当前决定：直接开始 PBR 原理。

原本建议第 55-65 天开始，是为了降低公式和前置概念压力。现在采用加速路线后，前置概念按需补：

```text
MVP / Camera
Depth
Normal
基础 Lighting
Material 参数
Texture 采样
Cubemap / Environment Map
```

执行原则：PBR 主线先走，遇到看不懂的依赖再补，不把补课变成前置门槛。

PBR 之后继续学：

```text
第 10 周：IBL / 环境光照
第 11 周：Shadow / 阴影
第 12 周：HDR / Tone Mapping / Gamma
第 13 周：Post Processing / 后处理
第 14 周：Deferred Rendering / 延迟渲染
第 15 周：Transparent / Blending / 排序
第 16 周：性能、Instancing、RenderDoc / Unity Frame Debugger
第 17 周以后：URP / HDRP 源码和实际项目对照
```

## 周总结节奏

每 7 天做一次周总结：

```text
第 07 天：OpenGL 基础管线总结
第 14 天：Texture / UV 总结
第 21 天：Transform / Model Matrix 总结
第 28 天：PBR 参数直觉总结
第 35 天：Cook-Torrance 结构总结
第 42 天：IBL / Environment 总结
第 49 天：Unity PBR Shader 实战总结
```

## 自动提醒规则

- 每天应该告诉“今天学什么”。
- 每天开头先自动总结上一天的主题、可能产出和关键一句话。
- 如果上一天可能没做，直接说“昨天可能没做，没关系”，不要卡住进度。
- 总结上一天之后，继续给当天的新任务、3-4 条 TODO 和最小输出目标。
- 如果当天是第 7 天、第 14 天这类节点，优先做周总结。
- 每个新 demo 默认包含 `README.md`、`CMakeLists.txt`、`run.sh`、`src/main.cpp`。
- 复盘图片、截图、卡片按课程放到 `notes/recaps/dayXX_*` 目录。
- OpenGL demo 继续放在 `demos/NN_topic`。
- Shader 效果实战默认在 Unity 里写；本仓库只记录计划、复盘、截图和必要的 shader 笔记，除非后续创建独立 Unity 工程目录。
