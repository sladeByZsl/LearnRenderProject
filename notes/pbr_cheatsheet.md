# PBR 一页速查表

用途：打印或放在屏幕旁边。先看直觉，再看公式；先观察材质球，再回 shader。

## 30 秒速查区

```text
PBR = 用更稳定、更接近物理的方式描述“材质如何反射光”。

最终颜色大致来自：
直接光 + 环境光
直接光里最核心：
Diffuse + Specular

四个常用参数：
Albedo    = 物体本色，不含光照和阴影
Metallic  = 0 非金属，1 金属
Roughness = 越大越糙，高光越宽，反射越模糊
AO        = 环境遮蔽，缝隙和角落更暗

Cook-Torrance 三件事：
F = Fresnel，擦边角反射更强
D = NDF，微表面朝向分布
G = Geometry，微表面互相遮挡
```

## 1. 核心概念

| 概念 | 记法 | 作用 |
|------|------|------|
| PBR | Physically Based Rendering | 用物理直觉约束材质和光照，减少“凭感觉调高光” |
| Albedo / Base Color | 本色 | 材质固有颜色，不应该包含光照、阴影、高光 |
| Metallic | 金属度 | 决定材质更像塑料/石头，还是金属 |
| Roughness | 粗糙度 | 决定高光宽窄、环境反射清晰度 |
| Smoothness | 光滑度 | Unity 常用，通常约等于 `1 - roughness` |
| AO | Ambient Occlusion | 近似环境光在缝隙、角落被遮住 |
| Normal | 法线 | 决定表面朝向，影响光照明暗 |
| BRDF | 反射分布函数 | 描述光从一个方向打来，会如何反射到另一个方向 |
| IBL | Image Based Lighting | 用环境贴图提供间接光和反射 |

## 2. 关键框架

### PBR 颜色框架

```text
Final Color
= Direct Lighting
+ Indirect Lighting / IBL
```

### 直接光框架

```text
Direct Lighting
= Diffuse
+ Specular
```

### Cook-Torrance Specular 框架

```text
Specular BRDF = (D * F * G) / (4 * NdotL * NdotV)
```

只先记职责：

```text
D：有多少微表面方向合适
F：这个角度反射有多强
G：这些微表面有没有被挡住
```

### 金属 / 非金属直觉

```text
非金属：
Diffuse 明显
Specular 通常较弱，多为白色高光

金属：
Diffuse 很弱或接近没有
Specular 很强，并带有金属本身颜色
```

## 3. 常见错误

| 错误 | 后果 | 修正 |
|------|------|------|
| Albedo 贴图里带阴影或高光 | 光照重复，材质发脏 | Albedo 只放本色 |
| 把 Smoothness 当 Roughness | 粗糙/光滑效果反了 | Unity Smoothness 通常是 `1 - Roughness` |
| 金属度半灰乱用 | 材质像脏塑料 | 大多数真实材质 metallic 接近 0 或 1 |
| 忘记线性空间 | 颜色和能量不对 | Unity 项目优先使用 Linear Color Space |
| 法线方向不对 | 光照怪、阴影怪 | 检查 normal map 类型、切线空间和模型法线 |
| Roughness 太低 | 所有东西像湿塑料 | 大多数材质需要合理粗糙度 |
| AO 乘太重 | 画面脏、细节死黑 | AO 只弱化环境光，不要过度压直接光 |
| 只看单灯，不看环境 | 金属材质显得黑 | 金属强依赖环境反射 / IBL |

## 4. 必须记住的公式 / 步骤 / 方法

### 必记公式

```text
Smoothness ~= 1 - Roughness
```

```text
Specular = (D * F * G) / (4 * NdotL * NdotV)
```

```text
NdotL = max(dot(N, L), 0)
NdotV = max(dot(N, V), 0)
```

### 必记步骤

```text
1. 准备材质参数：
   Albedo / Metallic / Roughness / AO / Normal

2. 准备方向：
   N = normal
   L = light direction
   V = view direction
   H = normalize(L + V)

3. 计算直接光：
   Diffuse + Specular

4. 加环境光：
   Diffuse IBL + Specular IBL

5. 做颜色输出：
   HDR -> Tone Mapping -> Gamma / sRGB
```

### 必记观察方法

```text
固定灯光和环境，只改一个参数。

先改 roughness：
看高光从窄变宽，反射从清楚变模糊。

再改 metallic：
看 diffuse 变弱，specular 变强并带材质颜色。
```

## 5. Unity 快速对照

| PBR 概念 | Unity 常见名字 | 注意 |
|----------|----------------|------|
| Albedo | Base Map / Base Color | 不要带光照 |
| Metallic | Metallic | 多数材质用 0 或 1 |
| Roughness | Smoothness 反向 | Smoothness 越高越光滑 |
| AO | Occlusion Map | 主要影响环境光 |
| Normal | Normal Map | 注意导入类型 |
| IBL | Skybox / Reflection Probe | 金属材质很依赖它 |
| Direct Light | Directional / Point / Spot Light | 负责直接照明 |

## 现在该做的最小实验

```text
Unity 里做 5x5 材质球矩阵：

横轴：roughness 0 -> 1
纵轴：metallic 0 -> 1

固定：
同一灯光
同一 skybox
同一 camera
同一 albedo

观察：
roughness 改高光形状
metallic 改反射和颜色分配
```

一句话结论：

```text
PBR 不是“更复杂的高光”，而是用一组稳定参数描述材质如何分配漫反射、镜面反射和环境反射。
```
