# PBR 复习周

日期：2026-06-22 起

本周不讲新内容。目标是把 Day 22-31 已经出现过的 PBR 概念复述清楚，并补齐薄弱点。

## 复习原则

```text
不追进度
不加新术语
不推新公式
每天只复习一个旧概念
每次只写 3-5 句话
```

## 本周复习顺序

### Day R1：PBR 总体问题

今日文件：[R1：PBR 为什么让材质更稳定](r1_pbr_stability/README.md)

复习：

- Day 22：PBR Intro
- Day 26：Energy Conservation

要说清：

```text
PBR 不是自动真实，而是让材质参数更稳定、更可复用。
能量守恒防止 diffuse 和 specular 无脑相加导致假亮。
```

Unity 观察：

- 同一个材质球换不同 Skybox / 灯光，观察材质是否仍然可信。

### Day R2：材质参数直觉

今日文件：[R2：Albedo / Metallic / Roughness](r2_material_parameters/README.md)

复习：

- Day 23：Albedo
- Day 24：Metallic
- Day 25：Roughness

要说清：

```text
Albedo 是本色，不应包含明显光照。
Metallic 改变金属 / 非金属的光照分工。
Roughness 改变高光集中程度和反射清晰度。
```

Unity 观察：

- 做一个 3x3 材质球矩阵：横轴 roughness，纵轴 metallic。

### Day R3：BRDF 结构

今日文件：[R3：BRDF 结构](r3_brdf_structure/README.md)

复习：

- Day 27：Disney BRDF
- Day 28：Cook-Torrance / F-D-G

要说清：

```text
Disney BRDF 的价值是参数设计。
Cook-Torrance 可以先看成 F、D、G 三个问题。
F 看视角，D 看微表面方向分布，G 看遮挡。
```

Unity 观察：

- 固定材质和灯光，改变观察角度，观察边缘反射变化。

### Day R4：半球积分直觉

今日文件：[R4：半球积分直觉](r4_hemisphere_integral/README.md)

复习：

- Day 28：立体角
- Day 29：Irradiance
- Day 30：cosθ 投影项

要说清：

```text
立体角描述 3D 方向范围。
Irradiance 是表面点从半球方向收到的光总量。
cosθ 表示斜着来的光会摊开，单位面积贡献更少。
```

Unity 观察：

- 用 Directional Light 照平面，旋转光照角度，观察亮度变化。

### Day R5：Diffuse IBL

今日文件：[R5：Diffuse IBL / Irradiance Map](r5_diffuse_ibl/README.md)

复习：

- Day 29：Irradiance
- Day 31：Irradiance Map

要说清：

```text
Diffuse IBL 是环境光从半球方向的加权累加。
Irradiance Map 是把这个累加提前算好，运行时按 normal 查表。
```

Unity 观察：

- 切换 Skybox / Reflection Probe，观察暗部环境色变化。

## 周末输出

周末只做一个轻量总结：

```text
标题：Unity 开发者如何理解 PBR 的第一层

1. PBR 解决什么问题
2. 材质参数：albedo / metallic / roughness
3. 光照结构：diffuse / specular / energy conservation
4. BRDF 结构：F / D / G
5. IBL 初印象：irradiance / irradiance map
```

## 公众号轻量提纲

标题：

```text
Unity 开发者如何理解 PBR 的第一层
```

结构：

1. 为什么要学 PBR：不是自动真实，而是材质稳定。
2. 三个最容易先记住的参数：albedo、metallic、roughness。
3. 光照分工：diffuse / specular 和能量守恒。
4. BRDF 第一层结构：Disney 参数设计，Cook-Torrance 的 F / D / G。
5. IBL 初印象：环境光不是一盏灯，而是一整个半球的贡献。
6. 本周留下的问题：哪些地方还需要在 Unity 里用材质球矩阵继续观察。

## 复习检查题

1. 为什么 albedo 里不应该画明显高光？
2. 为什么 metallic 不是“让材质更亮”的按钮？
3. 为什么 roughness 高时高光更大、更散、更糊？
4. 为什么斜着来的光要乘 `cosθ`？
5. 为什么 diffuse irradiance map 是模糊的？
