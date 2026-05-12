# Framework

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

- `Application`
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

## Demo 写法

以后 demo 优先继承 `lr::Application`：

```cpp
class MyDemo : public lr::Application
{
public:
    MyDemo()
        : lr::Application({800, 600, "My Demo"})
    {
    }

protected:
    void onStart() override
    {
        // 创建 shader、mesh、game object。
    }

    void onUpdate() override
    {
        // 每帧更新逻辑，可选。
    }

    void onRender() override
    {
        // 每帧绘制。
    }

    void onShutdown() override
    {
        // 在 OpenGL Context 销毁前释放 GPU 资源。
    }
};

int main()
{
    MyDemo app;
    return app.run();
}
```

`Application` 已经负责：

- `glfwInit`
- OpenGL 3.3 Core Profile 窗口参数
- macOS `GLFW_OPENGL_FORWARD_COMPAT`
- 创建窗口和 OpenGL Context
- `glfwMakeContextCurrent`
- 初始化 GLAD
- 主循环
- 清屏
- Esc 退出
- 窗口尺寸变化时更新 `glViewport`

demo 类只需要关心“创建什么对象、每帧怎么更新、每帧画什么”。
