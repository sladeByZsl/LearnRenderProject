# 学习复盘

这里记录每天的学习复盘。复盘会尽量包含三件事：

- 今天学到的核心知识点。
- 和原计划相比的进度判断。
- 一张适合保存的学习卡片、脑图或流程图。

## 2026-05-12：OpenGL Day 01

![OpenGL Day 01 复盘](assets/opengl-day-01-recap.png)

### 今日核心知识

- GLFW：负责创建窗口、处理输入、创建 OpenGL Context。
- GLAD：负责运行时加载当前环境里的 OpenGL 函数入口。
- Context：OpenGL 调用默认作用在当前线程绑定的上下文上。
- VAO / VBO：VBO 保存顶点数据，VAO 保存顶点数据的读取规则。
- Shader：顶点着色器处理顶点，片元着色器处理像素颜色。

### 今日产出

- 跑通 LearnOpenGL 第一课 Hello Triangle。
- 写出两个三角形 demo。
- 把顶点 shader 和片元 shader 拆成独立文件。
- 建立轻量 `Framework`，封装 `Application`、`Mesh`、`GameObject`、`Scene` 等概念。
- 加入 Python 绑定 `lr_framework`，为后续用 Python 写上层 demo 留出入口。

### 和计划对比

原计划第 01 天只是跑出三角形。今天实际完成了第一课核心概念梳理、多个 demo、轻量 Framework 雏形和 Python 上层绑定，进度明显超前。

下一步建议放慢一点：第 02 天专注顶点数据、坐标顺序和形状变化，让封装背后的基础概念更稳。

## 图片资产

- [OpenGL 第一周学习路线](assets/opengl-week-01-roadmap.png)
- [OpenGL Day 01 复盘](assets/opengl-day-01-recap.png)
- [平台 NDC 坐标差异速记（生成版）](assets/ndc-platform-differences.png)
- [平台 NDC 坐标差异速记（修正版 SVG）](assets/ndc-platform-differences.svg)
