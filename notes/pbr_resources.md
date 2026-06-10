# PBR 学习资源路线

这份清单按 PBR-first 路线整理：先在 Unity 里建立材质参数直觉，再看实时渲染实现，最后补论文、书和完整数学。

## 推荐选择顺序

1. Unity / Marmoset / Adobe 的 PBR 材质参数说明：先理解 `albedo / metallic / roughness / ao` 是什么。
2. LearnOpenGL PBR：用代码把 Cook-Torrance、IBL 和贴图流程连起来。
3. Google Filament 文档：看现代实时 PBR 引擎如何做工程取舍。
4. SIGGRAPH / Disney / Frostbite notes：理解生产级 PBR 为什么这么设计。
5. Real-Time Rendering / PBRT：作为长期参考书，不作为第一入口。

## 1. 书籍

### Real-Time Rendering, 4th Edition

- 适合阶段：已经知道 shader、光照、texture、PBR 参数，想系统理解实时渲染的人。
- 为什么值得学：它是实时渲染百科级参考，适合把 PBR 放进更大的管线里理解。
- 从哪开始：不要从第一页开始啃，先查 Physically Based Shading、BRDF、IBL、Tone Mapping 相关章节。
- 注意：这是参考书，不是每日教程。用来查概念，不要试图一次读完。

### Physically Based Rendering: From Theory to Implementation

- 适合阶段：想深入路径追踪、渲染方程、采样、BSDF 的中后期学习。
- 为什么值得学：全书免费在线，理论和实现绑在一起，是 PBR / path tracing 的硬核源头资料。
- 从哪开始：先只看目录和 BRDF / reflection 相关章节，不急着读采样和积分实现。
- 注意：它偏离线渲染，不是 Unity PBR shader 入门书。

### Fundamentals of Computer Graphics

- 适合阶段：补数学、光照、BRDF、几何和相机基础。
- 为什么值得学：比 PBRT 更适合作为图形学基础教材。
- 从哪开始：相机、光照、反射模型相关章节。
- 注意：不要把它当 PBR 专项资源，它是补地基的。

## 2. 免费课程

### Unity Learn: Creating Physically Based Materials

- 适合阶段：现在立刻开始。
- 为什么值得学：直接对应 Unity 材质面板，适合先建立参数手感。
- 从哪开始：只看 Standard / Lit 材质参数和 Texture properties。
- 注意：它不会解释 Cook-Torrance 公式，目标是先会调材质。

### LearnOpenGL PBR

- 适合阶段：已经能看懂 shader 和 texture 采样，想自己实现 PBR。
- 为什么值得学：理论、OpenGL shader、IBL demo 都连在一起，工程闭环强。
- 从哪开始：PBR Theory，然后 Lighting，再 IBL。
- 注意：不要第一天就硬啃 IBL，先把 `NDF / Geometry / Fresnel` 三块职责分清。

### SIGGRAPH Physically Based Shading courses

- 适合阶段：理解了基础 PBR 后，想看工业界为什么这么设计。
- 为什么值得学：Disney、Frostbite 等生产经验都在这里，是 PBR 从论文走向生产的关键资料。
- 从哪开始：2012 Practical Physically Based Shading，再看 Disney BRDF notes。
- 注意：这是课程论文资料，不适合当第一教程。

### Catlike Coding Rendering

- 适合阶段：Unity 内写 shader，想从普通光照过渡到 PBS。
- 为什么值得学：Unity 语境强，能把法线、光照、能量守恒、metallic workflow 串起来。
- 从哪开始：Rendering 4: The First Light。
- 注意：部分内容基于旧 Built-in 管线，概念仍有价值，代码要按 URP/HDRP 取舍。

## 3. YouTube 频道

### Daniel Ilett

- 适合阶段：Unity Shader / URP 入门到中级。
- 为什么值得学：有 Unity Shader Code Basics，PBR 主题贴近你现在的学习方向。
- 从哪开始：Physically Based Rendering | Unity Shader Code Basics 07。
- 注意：视频适合做第一版 Unity demo，底层公式要配 LearnOpenGL / Filament。

### Freya Holmér

- 适合阶段：shader 基础、数学可视化、技术美术思维。
- 为什么值得学：讲 shader 和数学直觉很清楚，适合补“为什么这个效果这么写”。
- 从哪开始：Shader Basics, Blending & Textures。
- 注意：不是 PBR 专项频道，但能补 shader 直觉。

### Ben Cloward

- 适合阶段：Unity Shader Graph、Unreal Material、技术美术实战。
- 为什么值得学：大量材质和 shader graph 实战，适合做效果库。
- 从哪开始：Shader Graph Basics / Advanced Materials。
- 注意：节点图容易让人忽略底层公式，要和 HLSL / PBR 理论对照。

### Acerola

- 适合阶段：已经能写基础 shader，想看效果型 shader 和图形程序员思路。
- 为什么值得学：视觉效果强，能激发项目实战方向。
- 从哪开始：shader / post-processing 相关视频。
- 注意：不要拿它当 PBR 第一教程，它更适合扩展效果视野。

## 4. 播客

PBR 专项播客很少，播客更适合补“图形程序员 / 技术美术怎么思考问题”。

### Wookash Podcast

- 适合阶段：想听图形、shader、技术美术职业经验。
- 为什么值得学：有 Freya Holmér、graphics programming 相关访谈。
- 从哪开始：Freya Holmér 或 graphics programming 相关集。
- 注意：它不是教程，适合通勤听。

### The Generalists Podcast: shader / technical art episodes

- 适合阶段：对技术美术职业路径、HLSL、shader 实战感兴趣。
- 为什么值得学：能补“怎么学、怎么做作品集”的方向感。
- 从哪开始：Mastering Shader Programming & Technical Art with Harry。
- 注意：听完要转化成 demo，否则容易只获得情绪价值。

### Coffee, Code & Shaders

- 适合阶段：想持续接触 real-time rendering 话题。
- 为什么值得学：主题聚焦 graphics programming / shaders。
- 从哪开始：挑 PBR、shader、rendering pipeline 相关标题。
- 注意：新播客内容质量需要筛选，不作为主资源。

### UCSD CSE 168 Rendering podcast / course recordings

- 适合阶段：想补渲染方程、BRDF、路径追踪等理论。
- 为什么值得学：大学课程节奏比论文友好。
- 从哪开始：BRDF、rendering equation、path tracing 相关课。
- 注意：偏理论，不要在刚开始 PBR 参数阶段投入太多。

## 5. 网站或文档

### Unity Manual / Unity Learn

- 适合阶段：现在立刻用。
- 为什么值得学：直接对应 Unity 材质面板和项目实践。
- 从哪开始：Standard Shader / Lit 材质参数、Physically Based Materials。
- 注意：Unity 的 Smoothness 和很多资料里的 Roughness 是反向概念。

### LearnOpenGL PBR

- 适合阶段：想从 Unity 参数进入 shader 实现。
- 为什么值得学：PBR theory、direct lighting、IBL 都有代码。
- 从哪开始：Theory 页面。
- 注意：先理解公式结构，不要一上来背所有变量。

### Google Filament 文档

- 适合阶段：理解了基础 PBR，想看现代实时引擎实现。
- 为什么值得学：工程解释非常完整，覆盖材质模型、IBL、色彩、曝光等。
- 从哪开始：Physically Based Rendering in Filament 的材质模型和 lighting 部分。
- 注意：信息密度高，适合当第二阶段主文档。

### Marmoset / Adobe PBR Guides

- 适合阶段：PBR 参数入门、材质制作。
- 为什么值得学：从美术和材质制作角度解释 PBR，不容易陷进公式。
- 从哪开始：Marmoset Basic Theory of PBR、Adobe PBR Guide Part 1。
- 注意：它们偏材质工作流，不替代 shader 实现。

### Unreal Physically Based Materials

- 适合阶段：想理解行业通用 PBR 参数，而不局限 Unity。
- 为什么值得学：Unreal 的文档对 Base Color、Metallic、Roughness、Specular 的解释很实用。
- 从哪开始：Physically Based Materials 页面。
- 注意：引擎参数命名和 Unity 不完全一致，要做概念映射。

## 6. 实战项目

### Unity PBR Material Matrix

- 适合阶段：现在立刻做。
- 为什么值得学：最短路径观察 metallic / roughness / albedo 的影响。
- 从哪开始：建 5x5 球阵列，横轴 roughness，纵轴 metallic。
- 注意：固定同一个灯光和环境，否则参数变化会被光照变化干扰。

### LearnOpenGL PBR demo

- 适合阶段：参数直觉建立后。
- 为什么值得学：能看到完整 PBR shader 的输入、公式和输出。
- 从哪开始：先跑 direct lighting，再跑 IBL。
- 注意：不要同时改太多变量；一次只动 roughness 或 metallic。

### Filament sample_full_pbr

- 适合阶段：理解基础实现后看工程级样例。
- 为什么值得学：它是实际 PBR engine 的样例，不只是教学 demo。
- 从哪开始：sample_full_pbr.cpp 和材质文档对照看。
- 注意：先看材质和渲染调用结构，不要一开始追完整引擎架构。

### glTF Sample Viewer

- 适合阶段：想验证 PBR 模型和材质贴图是否符合标准。
- 为什么值得学：glTF 2.0 的 PBR 模型是行业通用交换格式。
- 从哪开始：加载 Khronos sample assets，对照 baseColor / metallicRoughness / normal / occlusion / emissive。
- 注意：这是验证和观察工具，不是教程。

## 当前建议的第一周顺序

1. Day 1：Unity 材质球矩阵，只调 Metallic / Smoothness。
2. Day 2：读 Marmoset / Adobe，理解 PBR 参数。
3. Day 3：LearnOpenGL PBR Theory，只看 BRDF、energy conservation、microfacet。
4. Day 4：Cook-Torrance 只拆 F / D / G 职责。
5. Day 5：Unity 做 Fresnel 观察。
6. Day 6：LearnOpenGL direct lighting shader。
7. Day 7：整理一张 PBR 参数和公式结构脑图。

