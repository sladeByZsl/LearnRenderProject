# OpenGL 学习路线图

这里放整体学习计划图，不归到某一天复盘里。

## 当前路线

![Unity 开发者从 0 补图形学整体路线图](opengl-learning-roadmap.png)

- [可编辑 SVG 源文件](opengl-learning-roadmap.svg)

## 阶段计划

| 阶段 | 主题 | 最小产出 |
|------|------|----------|
| 第 1 周 | OpenGL 基础管线 | 跑通三角形，看懂 VAO / VBO / Shader / Draw Call |
| 第 2 周 | Texture / UV | 画出带贴图矩形，理解 UV、Sampler、Wrapping、Filtering |
| 第 3 周 | Transform / Model Matrix | 对应 Unity position / rotation / scale |
| 第 4 周 | Camera / MVP | 画 3D cube，用 camera 观察 |
| 第 5 周 | Depth / 3D 空间 | 多个物体前后遮挡正确 |
| 第 6 周 | Lighting | 用法线和光方向做基础明暗 |
| 第 7 周 | Material / 小场景 | Mesh、Material、Texture、Light、Camera 整合 |
| 第 8-9 周 | PBR 入口 | Cubemap / Environment 后进入最小 PBR shader |

## PBR 时间判断

PBR 合理开始点：第 55-65 天左右。

原因：PBR 依赖 `Camera / Depth / Normal / Lighting / Material / Texture / Environment Map`。这些上下文不够时，PBR 很容易变成背公式。

## 一句话版本

```text
先把 Mesh / Texture / Transform / Camera / Light / Material 这些 Unity 熟悉概念拆到底层，再进入 PBR。
```
