# SIGGRAPH 2012 PBR Shading Course 阅读计划

这个计划不是为了把课程资料从头到尾硬读完，而是为了服务当前的 PBR-first 学习路线：

```text
先建立 PBR 直觉
-> 再理解关键公式在解决什么问题
-> 最后回到 Unity / RenderDoc / OpenGL 里观察现象
```

建议节奏：每次 45-60 分钟。每次只输出 3-5 句话笔记，不要求当天完全懂。

## 总体读法

优先读 PDF 和 notes：

- `slides.pdf` 用来看图、抓结构。
- `notes.pdf` 用来补解释。
- `.pptx` / `.key` 只是备用源文件，不作为日常阅读主入口。
- `.nb` / notebook 暂时不深读，等公式和曲线想看得更细时再打开。

每次阅读都按这个顺序：

1. 先看标题和图，猜这节在解决什么问题。
2. 只圈出 1 个今天最重要的概念。
3. 用自己的话写一句“它在 Unity 里大概对应什么”。
4. 最后写一个问题，留给后面反复遇见。

## 第 1 轮：建立 PBR 直觉

目标：知道 PBR 为什么出现，以及它想把材质从“玄学调参”变成“相对稳定的规则”。

### Day A：为什么需要 PBR

阅读：

- `01_hill_importance_slides.pdf`

只看：

- 前半部分关于 physically based shading 重要性的页面。
- 重点看“为什么老的材质模型难以在不同光照下保持一致”。

输出：

- 用 3 句话解释：PBR 不是为了更高级，而是为了让材质在不同环境里更稳定。
- 写一个 Unity 关联：同一个材质球换 HDRI / 灯光时，PBR 参数应该仍然比较可信。

### Day B：PBR 的物理直觉

阅读：

- `02_hoffman_physics_math_slides.pdf`
- `02_hoffman_physics_math_notes.pdf`

只看：

- 反射、折射、吸收、散射相关的小节。
- 暂时跳过复杂推导。

输出：

- 画一个简单图：光打到物体表面后，一部分反射，一部分进入物体，一部分被吸收。
- 用自己的话解释：为什么金属和非金属看起来不一样。

### Day C：材质参数的生产意义

阅读：

- `03_mcauley_farcry3_slides.pdf`
- `03_mcauley_farcry3_notes.pdf`

只看：

- Far Cry 3 里材质校准、artist-facing workflow 相关部分。
- 重点看他们如何让美术按规则制作材质。

输出：

- 总结 albedo / roughness / metallic 为什么要有“合理范围”。
- 对照 Unity Standard / URP Lit 材质面板，写出这些参数在哪里。

## 第 2 轮：理解 BRDF 结构

目标：不急着推公式，先知道一个 PBR shader 通常由哪些模块组成。

### Day D：从 Blinn-Phong 过渡到微表面

阅读：

- `04_gotanda_beyond_blinn_slides.pdf`
- `04_gotanda_beyond_blinn_notes.pdf`

只看：

- Blinn-Phong 的限制。
- 为什么要引入更物理的 specular 模型。

输出：

- 用一句话说清：Blinn-Phong 更像“调出一个高光”，PBR 更像“描述表面如何反射光”。
- 找一张旧 shader 高光和 PBR roughness 高光的对比图，放进当天 recap 文件夹。

### Day E：Disney BRDF 总览

阅读：

- `06_burley_disney_brdf_slides.pdf`
- `06_burley_disney_brdf_notes.pdf`

只看：

- Disney BRDF 的设计目标。
- base color / metallic / roughness / specular / clearcoat 相关部分。

输出：

- 写一张“Unity 材质参数 -> Disney BRDF 直觉”的对照表。
- 不要求理解每个公式，只要知道参数控制的视觉结果。

### Day F：Cook-Torrance 三件套

阅读：

- `02_hoffman_physics_math_notes.pdf`
- `06_burley_disney_brdf_notes.pdf`

只看：

- Fresnel / Distribution / Geometry 相关解释。

输出：

```text
F: 看角度，越擦边越亮
D: 看微表面朝向，决定高光形状
G: 看遮挡，粗糙表面内部会互相挡光
```

再写 1 句话：这三个东西一起决定 specular 长什么样。

## 第 3 轮：回到游戏和 Unity

目标：把课程里的 PBR 理论变成可观察的 Unity 实验。

### Day G：Far Cry 3 材质校准

阅读：

- `03_mcauley_farcry3_notes.pdf`
- `03_mcauley_farcry3_calibration_tool.avi`

只看：

- calibration / reference material / lighting consistency 相关部分。

输出：

- 在 Unity 做一个材质球场景：塑料、金属、粗糙、光滑各 1 个。
- 截图保存到当天 recap 文件夹。
- 写 3 句话：哪些参数变化最明显，哪些参数目前还看不懂。

### Day H：Pixar / Film 的差异

阅读：

- `07_smits_pixar_model_slides.pdf`

只看：

- Pixar shading model 的目标和参数设计。

输出：

- 写 3 句话比较：游戏 PBR 更关心实时和统一工作流，电影 shading 更关心可控性和表现力。
- 不要求深读 notebook。

### Day I：OSL 只看大局

阅读：

- `05_martinez_osl_slides.pdf`

只看：

- OSL 为什么存在，以及它怎样描述 shader。

输出：

- 写一句话：OSL 更偏电影/离线渲染的 shader 表达；Unity Shader 更偏实时渲染管线里的效果实现。

## 第 4 轮：复盘和沉淀

目标：把课程变成自己的 PBR 知识卡，而不是停留在“看过”。

### Day J：做一页 PBR 总图

产出：

- 一张 PBR 思维导图或卡片。
- 内容只放：
  - light
  - material
  - diffuse
  - specular
  - BRDF
  - IBL

### Day K：做一个 Unity PBR 对照实验

产出：

- 横轴：roughness 从 0 到 1。
- 纵轴：metallic 从 0 到 1。
- 每个格子一个材质球。
- 截图保存到 recap 文件夹。

### Day L：用 12 岁小孩版本解释 PBR

输出模板：

```text
PBR 就是让电脑画东西时，尽量按真实光的习惯来。
光照到物体后，有些会被弹回来，有些会钻进去，有些会被吃掉。
金属更像镜子，非金属更像有本色的塑料或石头。
roughness 决定表面是不是磨砂，越粗糙，高光越散。
```

解释完后检查：

- 有没有用了没解释的术语？
- 有没有把 albedo 说成最终颜色？
- 有没有忘记环境光和反射？

## 推荐顺序

如果只读最少资料，按这个顺序：

1. `01_hill_importance_slides.pdf`
2. `02_hoffman_physics_math_notes.pdf`
3. `06_burley_disney_brdf_notes.pdf`
4. `03_mcauley_farcry3_notes.pdf`

如果想继续深入，再读：

5. `04_gotanda_beyond_blinn_notes.pdf`
6. `07_smits_pixar_model_slides.pdf`
7. `05_martinez_osl_slides.pdf`

## 暂时可以跳过

这些资料先不作为主线：

- `02_hoffman_physics_math_notebook.nb`
- `02_hoffman_physics_math_notebook.pdf`
- `07_smits_pixar_model_notebook.nb`
- `07_smits_pixar_model_notebook.pdf`

原因：它们更适合验证公式和曲线。当前阶段先抓“公式在解决什么问题”，不先陷进推导。

## 每次阅读笔记模板

```markdown
## 日期

今天看的资料：

今天只抓一个概念：

我用自己的话解释：

它在 Unity 里对应：

还没懂的问题：
```

## Q&A

### Q：这套课程应该全部读完吗？

A：不需要一口气读完。当前最重要的是先读懂 PBR 的参数直觉和 BRDF 大结构，能在 Unity 里观察到 albedo / metallic / roughness / Fresnel / IBL 的变化。公式和 notebook 可以等你已经有画面经验后再回头看。

### Q：为什么先读 Disney BRDF，而不是先读所有数学？

A：因为你的目标是从 Unity 开发者角度补图形学。Disney BRDF 更接近现代材质面板背后的设计思想，能更快把理论和 Unity 材质参数连起来。数学部分很重要，但先用来解释现象，不先当考试题做。
