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

## Shader 文件

demo 的顶点着色器和片元着色器优先拆成独立文件：

```text
shaders/demo_name.vert
shaders/demo_name.frag
```

Framework 提供从文件创建 shader program 的入口：

```cpp
shader = lr::ShaderProgram::fromFiles(
    shaderPath("demo_name.vert").c_str(),
    shaderPath("demo_name.frag").c_str()
);
```

这样 demo 的 C++ 代码负责场景逻辑，shader 文件负责 GPU 上的顶点和片元逻辑。

如果 demo 继承了 `lr::Application`，优先用更短的写法：

```cpp
shader = loadShader("demo_name.vert", "demo_name.frag");
```

shader 目录通过 `ApplicationConfig` 传入。CMake 可以给 demo 注入绝对路径：

```cmake
target_compile_definitions(my_demo PRIVATE
    SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
)
```

## 还能封装什么

当前已经封装掉了：

- 窗口和主循环：`Application`
- shader 文件加载：`loadShader`
- 标准三角形 Mesh：`Mesh::triangle`
- 物体属性组合：`GameObject(mesh, position, scale, color)`
- VAO/VBO 上传：`Mesh`
- uniform 传参：`GameObject::draw`

后面可以继续封装：

- `Scene`：统一保存和绘制多个 `GameObject`。
- `Time`：提供 `deltaTime`，方便做动画。
- `Input`：提供更像 Unity 的 `getKey` / `getKeyDown`。
- `Camera2D` / `Camera3D`：把坐标变换和视图矩阵收起来。
- `Texture2D`：封装图片加载和纹理绑定。
- `SpriteRenderer` / `MeshRenderer`：让渲染组件更像 Unity。

## 动画

`Application` 提供：

```cpp
float deltaTime() const;
float time() const;
```

可以在 `onUpdate()` 里做逐帧动画：

```cpp
void onUpdate() override
{
    object->transform.rotation += deltaTime() * 1.2f;
}
```

`Transform2D::rotation` 使用弧度。正数和负数方向相反，数值越大转得越快。
