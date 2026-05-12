#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

// 窗口初始大小。后面窗口缩放时，OpenGL 视口会在回调里同步调整。
static const unsigned int SCR_WIDTH = 800;
static const unsigned int SCR_HEIGHT = 600;

// GLFW 在窗口大小变化时调用这个函数。
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 每一帧检查键盘输入。当前只处理 Esc 退出。
void processInput(GLFWwindow* window);

// 编译单个 shader：顶点着色器或片元着色器。
unsigned int compileShader(unsigned int type, const char* source);

// 把顶点着色器和片元着色器链接成一个 shader program。
unsigned int createShaderProgram();

// 顶点着色器：接收 CPU 传来的顶点坐标，并把它交给 OpenGL 后续流水线。
//
// layout (location = 0) 表示这个输入变量绑定到 0 号顶点属性。
// main() 里的 gl_Position 是顶点着色器必须写入的内置输出。
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

// 片元着色器：决定三角形最终显示出来的像素颜色。
//
// 这里输出的是固定橙色：R=1.0, G=0.5, B=0.2, A=1.0。
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

int main()
{
    // 1. 初始化 GLFW。GLFW 负责创建窗口、OpenGL 上下文和处理输入事件。
    glfwInit();

    // 2. 告诉 GLFW：我们要使用 OpenGL 3.3 Core Profile。
    // LearnOpenGL 教程默认从 3.3 Core 开始，因为它足够现代，也兼容性较好。
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // macOS 创建 3.2+ Core Profile 上下文时需要这个提示。
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 3. 创建窗口。窗口创建成功后，GLFW 也会为它准备一个 OpenGL 上下文。
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL - Hello Triangle", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 4. 把这个窗口的 OpenGL 上下文设置为当前线程正在使用的上下文。
    // 后面的 OpenGL 调用都会作用在这个窗口上。
    glfwMakeContextCurrent(window);

    // 5. 注册窗口尺寸变化回调。窗口大小变化时，视口也要跟着更新。
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 6. 初始化 GLAD。OpenGL 函数地址需要在上下文创建后才能加载。
    // 可以把 GLAD 理解成“帮 C++ 找到显卡驱动里 OpenGL 函数入口”的工具。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 7. 编译并链接 shader。之后绘制三角形时，GPU 会按这个程序处理顶点和颜色。
    unsigned int shaderProgram = createShaderProgram();

    // 三角形的 3 个顶点坐标。
    //
    // OpenGL 的标准化设备坐标范围大致是：
    // x: -1 左, +1 右
    // y: -1 下, +1 上
    // z: 这里暂时都写 0，第一课先只看 2D 三角形。
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // VBO: Vertex Buffer Object，保存顶点数据本身。
    // VAO: Vertex Array Object，保存“顶点数据应该怎么读”的配置。
    unsigned int VBO;
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定 VAO 后，后续的顶点属性配置都会记录到这个 VAO 里。
    glBindVertexArray(VAO);

    // 把 VBO 绑定为当前 GL_ARRAY_BUFFER，然后把 vertices 数据复制到 GPU。
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 告诉 OpenGL 如何解释 VBO 里的数据：
    // - location = 0：对应顶点着色器里的 layout (location = 0)
    // - 每个顶点有 3 个 float：x, y, z
    // - 每个顶点之间间隔 3 * sizeof(float)
    // - 第一个坐标从缓冲区开头开始读
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // 启用 0 号顶点属性。只配置不启用，shader 仍然读不到这个属性。
    glEnableVertexAttribArray(0);

    // 解除绑定，避免后面不小心改到当前 VBO/VAO。
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 渲染循环：只要窗口没有请求关闭，就一直绘制新的一帧。
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入，例如按 Esc 退出。
        processInput(window);

        // 清空上一帧画面。glClearColor 设置背景色，glClear 真正执行清屏。
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 使用 shader program 和 VAO，然后发出 draw call。
        // glDrawArrays(GL_TRIANGLES, 0, 3) 表示从第 0 个顶点开始，用 3 个顶点画三角形。
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 双缓冲交换：把刚画好的后台缓冲显示到屏幕上。
        glfwSwapBuffers(window);

        // 处理窗口事件，比如键盘、鼠标、关闭按钮、窗口缩放。
        glfwPollEvents();
    }

    // 释放 OpenGL 对象。小 demo 不释放也会随进程结束回收，但养成习惯很好。
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // 释放 GLFW 使用的资源。
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    // 按下 Esc 时，标记窗口应该关闭。渲染循环下一次判断时会结束。
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;

    // OpenGL 视口决定渲染结果映射到窗口的哪一块区域。
    // 这里让视口始终覆盖整个窗口。
    glViewport(0, 0, width, height);
}

unsigned int compileShader(unsigned int type, const char* source)
{
    // 创建 shader 对象，把源码交给 OpenGL，然后编译。
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // 检查编译是否成功。shader 写错时，错误信息会在 infoLog 里。
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

unsigned int createShaderProgram()
{
    // 分别编译顶点着色器和片元着色器。
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // shader program 是 GPU 真正执行的一组 shader 的组合。
    // 这里把两个 shader 附加上去，然后链接成一个完整程序。
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 检查链接是否成功。比如顶点 shader 输出和片元 shader 输入对不上时会失败。
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    // 链接完成后，单独的 shader 对象就可以删除了。
    // shader program 内部已经保留了链接后的结果。
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
