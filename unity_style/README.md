# Unity Style Layer

这个目录放一层面向 Unity 开发者的轻量封装。

目标不是做完整引擎，而是把 OpenGL 第一阶段最容易干扰理解的 VAO、VBO、Shader Program 等细节包起来，让 demo 更像这样写：

```cpp
lr::Mesh triangleMesh({
    {-0.5f, -0.5f, 0.0f},
    { 0.5f, -0.5f, 0.0f},
    { 0.0f,  0.5f, 0.0f}
});

lr::GameObject triangle(triangleMesh);
triangle.transform.position = {0.0f, 0.0f};
triangle.transform.scale = {1.0f, 1.0f};
triangle.material.color = {1.0f, 0.5f, 0.2f, 1.0f};
triangle.draw(shader);
```

当前封装：

- `Vec2` / `Vec3` / `Color`
- `Transform2D`
- `Mesh`
- `Material`
- `ShaderProgram`
- `GameObject`

设计原则：

- demo 层尽量像 Unity：关注 Mesh、Transform、Material、GameObject。
- OpenGL 层仍然透明：需要学习时可以打开实现看 VAO/VBO/Shader 如何工作。
- 只封装当前课程用到的最小功能，不提前做复杂引擎架构。
