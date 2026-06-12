# 04 Beyond Blinn：费曼学习笔记

对应资料：

- `../04_gotanda_beyond_blinn_slides.pdf`
- `../04_gotanda_beyond_blinn_notes.pdf`

本节只抓一个核心问题：

```text
为什么传统 Blinn-Phong 高光不够用，PBR 要换成更物理的反射模型？
```

## 一句话答案

Blinn-Phong 更像是在“画一个看起来像高光的亮斑”，而 PBR 里的微表面模型更像是在描述“这个表面为什么会这样反射光”。

## 讲给 12 岁小孩听

假设你要画一个玻璃球上的亮点。

旧方法像是你直接拿白笔在球上点一个亮点，然后觉得不够亮就涂大一点。

PBR 方法会先问：

```text
这个球表面光滑吗？
光从哪里来？
眼睛从哪里看？
表面的小镜子是不是都朝差不多的方向？
```

它不是单纯画亮点，而是根据表面和光的关系，算出亮点应该在哪里、多大、多亮、多模糊。

## Blinn-Phong 大概在做什么

Blinn-Phong 是经典的高光模型。它通常会算一个半程向量，然后根据表面法线和半程向量的接近程度，决定高光强弱。

你可以先不用记公式，只记效果：

```text
法线越接近高光方向 -> 越亮
shininess 越高 -> 高光越小越锐
shininess 越低 -> 高光越大越糊
```

它的优点是：

- 简单。
- 便宜。
- 很适合早期实时渲染。
- 能快速做出“有高光”的感觉。

但它的问题是：参数更像美术旋钮，不太像真实材料描述。

## Blinn-Phong 的问题

### 1. 参数不够像真实材料

`shininess` 可以控制高光大小，但它不像 `roughness` 那样直接对应“表面微观粗糙程度”。

所以你调的是：

```text
这个亮斑大一点还是小一点？
```

而不是：

```text
这个材料的微表面到底有多乱？
```

### 2. 容易不守能量

真实世界里，物体反射出去的光不能比收到的光还多。

旧模型如果参数乱调，可能会出现：

```text
diffuse 很亮
specular 也很亮
加起来超过真实材料应该反射的能量
```

这就会让材质换光照后不稳定。

### 3. 和材质工作流不够统一

现代 PBR 希望材质参数更统一：

```text
base color
metallic
roughness
normal
ao
```

而传统高光模型里的 diffuse color、specular color、shininess 往往更依赖项目约定，不同项目之间不容易迁移。

## PBR 的微表面思路

PBR 会把表面想象成无数个很小很小的小镜子。

这些小镜子的方向决定反射长什么样：

```text
小镜子排列整齐 -> 光滑 -> 高光小而亮 -> 反射清晰
小镜子方向混乱 -> 粗糙 -> 高光大而散 -> 反射模糊
```

这就把你前面问过的 roughness 串起来了。

roughness 不是随便控制高光大小，而是在描述：

```text
微表面法线方向分布有多分散。
```

## 从 Blinn-Phong 到 PBR 的心智变化

可以这样记：

```text
Blinn-Phong：我想要一个怎样的高光？
PBR：这个材料应该怎样反射光？
```

旧模型更像画效果。

PBR 更像描述材料。

## 和 Unity 的对应关系

如果你以前在 Unity 或其他引擎里见过旧 shader，可能会看到：

```text
Diffuse Color
Specular Color
Gloss / Shininess
```

而现代 Unity Lit / Standard 工作流更常见的是：

```text
Base Map / Base Color
Metallic
Smoothness 或 Roughness
Normal
Occlusion
```

其中：

```text
Smoothness = 1 - Roughness
```

也就是说，Unity 里 smoothness 越高，表面越光滑，高光越集中；roughness 越高，表面越粗糙，高光越分散。

## 本节容易误解的地方

### 误解 1：Blinn-Phong 是错的，不能用

不对。Blinn-Phong 不是“错”，而是更简单、更经验化。它在一些风格化 shader、简单项目、教学场景里仍然有价值。

### 误解 2：PBR 一定比 Blinn-Phong 好看

不一定。PBR 更稳定、更统一，但好不好看还取决于美术、灯光、贴图、曝光、后处理。PBR 只是给你更可靠的规则。

### 误解 3：roughness 就是 Blinn-Phong 的 shininess 换个名字

不准确。它们都能影响高光大小，但 roughness 背后对应微表面分布，和能量守恒、BRDF、IBL 都能接上；shininess 更像单独控制高光形状的经验参数。

## 你今天只需要记住

```text
Blinn-Phong 是调高光，PBR 是描述表面如何反射光。
```

## 30 秒复述版

Blinn-Phong 可以很便宜地做出高光，但它更像在画一个亮斑，参数不够像真实材料。PBR 的微表面模型会把表面看成很多小镜子，roughness 描述这些小镜子的方向有多乱。这样高光大小、清晰度、反射模糊程度就能和材料本身联系起来，而不是只靠美术旋钮硬调。

## 费曼检查

请你试着回答：

1. 为什么说 Blinn-Phong 更像“画高光”？
2. PBR 里的 roughness 和旧模型里的 shininess 有什么相似和不同？
3. 为什么能量守恒对材质稳定性有帮助？

## Q&A

### Q：我学 PBR 还需要知道 Blinn-Phong 吗？

A：需要知道大概思想，但不用深挖。它能帮你理解 PBR 是从什么问题升级来的：旧模型可以做出高光，但材质参数不够稳定，也不够容易跨场景复用。

### Q：Unity 的 Smoothness 和 Roughness 是什么关系？

A：它们方向相反。`Smoothness = 1 - Roughness`。Smoothness 越高，越光滑，高光越集中；Roughness 越高，越粗糙，高光越分散。
