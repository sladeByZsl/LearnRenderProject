#include "Framework.hpp"

#include <glad/glad.h>

// GLFW_INCLUDE_NONE 避免 GLFW 内部再 include 系统 OpenGL 头，由 GLAD 统一提供函数指针
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace lr
{

namespace
{

// 把文件完整读入字符串，供 shader 源码加载使用
std::string readTextFile(const char* path)
{
    std::ifstream file(path);
    if (!file)
    {
        std::cout << "Failed to open text file: " << path << std::endl;
        return {};
    }

    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

} // namespace

// ----- Mesh ------------------------------------------------------------------

// 构造函数：把 CPU 内存中的顶点数据上传到 GPU 显存，并记录数据格式。
// 完成后 GPU 端已拥有完整信息，draw() 只需绑定 VAO 就能发起绘制。
//
// 核心概念：
//   VBO（Vertex Buffer Object）
//       GPU 显存中的一段缓冲区，存放原始顶点数据（坐标、颜色、UV 等）。
//       CPU 无法直接在 GPU 上画东西，必须先把数据搬到 VBO。
//
//   VAO（Vertex Array Object）
//       记录"这段 VBO 里的数据应该如何解读"的状态对象。
//       例如：从第 0 字节开始，每 12 字节（sizeof Vec3）是一个顶点，
//             其中 3 个 float 对应着色器 layout(location=0) 的属性。
//       有了 VAO，draw() 时只需 glBindVertexArray 一行，OpenGL 就自动
//       按之前记录的格式读取 VBO，不需要每帧重新描述数据布局。
Mesh::Mesh(const std::vector<Vec3>& localVertices)
    : vertices(localVertices)
{
    // VAO 和 VBO 都是 GPU 端对象，glGen* 向驱动申请一个唯一 ID（句柄）。
    // 此时仅分配了 ID，显存空间还未真正占用。
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定 VAO：从这行起到解绑之前，所有"顶点属性格式"的设置都会被 VAO 记住。
    // 可以把 VAO 理解为一个录像机——按下录制后的每一步格式描述都会被记录下来，
    // 之后 draw() 时再播放，OpenGL 就知道怎么读数据。
    glBindVertexArray(VAO);

    // 把 VBO 绑定到 GL_ARRAY_BUFFER 插槽，后续的 glBufferData / glVertexAttribPointer
    // 都会作用于当前绑定的这个 VBO。
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 把 CPU 内存中的顶点数组复制到 GPU 显存。
    // 参数说明：
    //   GL_ARRAY_BUFFER        — 目标插槽（即上面绑定的 VBO）
    //   vertices.size()*sizeof(Vec3) — 数据总字节数
    //   vertices.data()        — CPU 端数据指针
    //   GL_STATIC_DRAW         — 使用提示：数据上传一次、多次绘制，驱动据此优化存储位置
    //                            （DYNAMIC_DRAW 表示频繁更新，STREAM_DRAW 表示每帧重传）
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3), vertices.data(), GL_STATIC_DRAW);

    // 告诉 GPU 如何从 VBO 中读取顶点属性，这条信息会被当前绑定的 VAO 记录下来。
    // 参数说明：
    //   0          — 属性槽位（location），对应顶点着色器 layout(location = 0)
    //   3          — 每个顶点该属性有 3 个分量（x, y, z）
    //   GL_FLOAT   — 每个分量是 32 位浮点数
    //   GL_FALSE   — 不做归一化（归一化会把整数映射到 [0,1] 或 [-1,1]，float 不需要）
    //   sizeof(Vec3) — 步长（stride）：相邻两个顶点起始位置之间的字节数；
    //                  顶点数据紧密排列时也可填 0 让 OpenGL 自行计算，但显式写更清晰
    //   (void*)0   — 偏移量：该属性在每个顶点数据块内从第 0 字节开始
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);

    // 启用 location=0 的属性槽位；默认是禁用状态，不启用则着色器读不到数据
    glEnableVertexAttribArray(0);

    // 解绑 VBO 和 VAO，防止后续代码误操作当前对象。
    // 注意顺序：先解绑 VBO，再解绑 VAO——
    // glVertexAttribPointer 已经把"用哪个 VBO"记进了 VAO，解绑 VBO 不影响 VAO 内的记录。
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// 析构时释放 GPU 资源。
// 必须在 GL context 仍然有效（即 glfwTerminate 之前）时调用，
// 否则驱动找不到对应 context，glDelete* 会静默失败造成显存泄漏。
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    // 从第 0 个顶点开始，按顶点数量绘制三角形
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertices.size()));
}

// 预置：等边三角形，顶点在本地空间，中心近似在原点
std::unique_ptr<Mesh> Mesh::triangle()
{
    return std::make_unique<Mesh>(std::vector<Vec3>{
        {-0.5f, -0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        { 0.0f,  0.5f, 0.0f}
    });
}

// ----- ShaderProgram ---------------------------------------------------------

// 编译顶点着色器和片元着色器，然后链接成 program
ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    unsigned int vertexShader   = compileShader(GL_VERTEX_SHADER,   vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    int success;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    // shader 对象链接进 program 后就不再需要，释放 GPU 端的中间对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// 从文件读取 GLSL 源码后交给构造函数编译
std::unique_ptr<ShaderProgram> ShaderProgram::fromFiles(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexSource   = readTextFile(vertexPath);
    std::string fragmentSource = readTextFile(fragmentPath);
    return std::make_unique<ShaderProgram>(vertexSource.c_str(), fragmentSource.c_str());
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id);
}

// 绑定此 program，之后的所有绘制调用都使用这套着色器
void ShaderProgram::use() const
{
    glUseProgram(id);
}

// 向 GPU uniform 变量写入 Vec2
void ShaderProgram::setVec2(const char* name, const Vec2& value) const
{
    glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}

// 向 GPU uniform 变量写入 RGBA 颜色（4 个 float）
void ShaderProgram::setColor(const char* name, const Color& value) const
{
    glUniform4f(glGetUniformLocation(id, name), value.r, value.g, value.b, value.a);
}

// 编译单个着色器阶段，失败时打印错误日志
unsigned int ShaderProgram::compileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader compilation failed:\n" << infoLog << std::endl;
    }

    return shader;
}

// ----- GameObject ------------------------------------------------------------

GameObject::GameObject(const Mesh& mesh)
    : mesh(mesh)
{
}

// 便捷构造：同时设置位置、缩放和颜色
GameObject::GameObject(const Mesh& mesh, const Vec2& position, const Vec2& scale, const Color& color)
    : mesh(mesh)
{
    transform.position = position;
    transform.scale    = scale;
    material.color     = color;
}

// 把 transform 和 material 作为 uniform 传给 shader，然后让 Mesh 发出绘制调用
// rotation 存弧度，这里拆成 (cos θ, sin θ) 传给 GPU，避免在 shader 里做三角函数
void GameObject::draw(const ShaderProgram& shader) const
{
    shader.use();
    shader.setVec2("uPosition", transform.position);
    shader.setVec2("uScale",    transform.scale);
    shader.setVec2("uRotation", {std::cos(transform.rotation), std::sin(transform.rotation)});
    shader.setColor("uColor",   material.color);
    mesh.draw();
}

// ----- Scene -----------------------------------------------------------------

GameObject& Scene::createGameObject(const Mesh& mesh)
{
    objects.push_back(std::make_unique<GameObject>(mesh));
    return *objects.back();
}

GameObject& Scene::createGameObject(const Mesh& mesh, const Vec2& position, const Vec2& scale, const Color& color)
{
    objects.push_back(std::make_unique<GameObject>(mesh, position, scale, color));
    return *objects.back();
}

// 依次绘制场景内所有 GameObject
void Scene::draw(const ShaderProgram& shader) const
{
    for (const auto& object : objects)
    {
        object->draw(shader);
    }
}

// 释放所有 GameObject，需在 GL context 销毁前调用（Mesh 析构会调 glDelete*）
void Scene::clear()
{
    objects.clear();
}

// ----- Application -----------------------------------------------------------

Application::Application(const ApplicationConfig& config)
    : config(config)
{
}

int Application::run()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinor);
    // Core Profile：只允许使用现代 OpenGL API，禁用所有已废弃功能
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // macOS 要求显式声明向前兼容，否则拿不到 3.3+ context
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* glfwWindow = glfwCreateWindow(
        config.width, config.height, config.title.c_str(), nullptr, nullptr);
    if (glfwWindow == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    window = glfwWindow;
    // 将 GL context 绑定到当前线程，此线程之后才能调用任何 gl* 函数
    glfwMakeContextCurrent(glfwWindow);
    // 把 this 存进窗口，供静态回调函数取回对象指针
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow* callbackWindow, int width, int height) {
        framebufferSizeCallback(callbackWindow, width, height);
    });

    // 用 GLAD 加载所有 OpenGL 函数指针，必须在 context current 之后调用
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        window = nullptr;
        return -1;
    }

    onStart();

    double previousTime = glfwGetTime();

    // 主循环：每帧更新计时 → 处理输入 → 逻辑更新 → 清屏 → 渲染 → 交换缓冲
    while (!glfwWindowShouldClose(glfwWindow))
    {
        double currentTime = glfwGetTime();
        frameDeltaTime = static_cast<float>(currentTime - previousTime);
        elapsedTime    = static_cast<float>(currentTime);
        previousTime   = currentTime;

        processDefaultInput();
        onUpdate();

        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        onRender();

        // 双缓冲：将后台缓冲呈现到屏幕，避免画面撕裂
        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    onShutdown();

    glfwTerminate();
    window = nullptr;
    return 0;
}

// 拼接 shaderDir 和文件名，加载并编译着色器
std::unique_ptr<ShaderProgram> Application::loadShader(const char* vertexFile, const char* fragmentFile) const
{
    auto shaderPath = [this](const char* fileName) {
        return std::string(config.shaderDir) + "/" + fileName;
    };

    return ShaderProgram::fromFiles(
        shaderPath(vertexFile).c_str(),
        shaderPath(fragmentFile).c_str()
    );
}

void Application::setClearColor(const Color& color)
{
    clearColor = color;
}

// 通知 GLFW 在下一次循环检测时退出主循环
void Application::close()
{
    if (window != nullptr)
    {
        glfwSetWindowShouldClose(static_cast<GLFWwindow*>(window), true);
    }
}

float Application::deltaTime() const
{
    return frameDeltaTime;
}

float Application::time() const
{
    return elapsedTime;
}

void* Application::nativeWindow() const
{
    return window;
}

// 窗口尺寸变化时同步更新 OpenGL 视口，保持渲染区域与窗口一致
void Application::framebufferSizeCallback(void* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

// 按下 Escape 时关闭窗口
void Application::processDefaultInput()
{
    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window);
    if (glfwWindow != nullptr && glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(glfwWindow, true);
    }
}

} // namespace lr
