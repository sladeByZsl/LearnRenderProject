#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

static const unsigned int SCR_WIDTH = 800;
static const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int compileShader(unsigned int type, const char* source);
unsigned int createShaderProgram();
void setupTriangleVAO(unsigned int& VAO, unsigned int& VBO, const float* vertices, unsigned int byteSize);

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.95, 0.72, 0.25, 1.0);
}
)";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL - 04 VAO", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int shaderProgram = createShaderProgram();

    // 两份顶点数据分别放进两个 VBO。
    // 今天重点观察：两个 VAO 会分别记住自己的 VBO 和读取方式。
    float leftTriangle[] = {
        -0.85f, -0.45f, 0.0f,
        -0.25f, -0.45f, 0.0f,
        -0.55f,  0.45f, 0.0f
    };

    float rightTriangle[] = {
         0.25f, -0.45f, 0.0f,
         0.85f, -0.45f, 0.0f,
         0.55f,  0.45f, 0.0f
    };

    unsigned int leftVAO;
    unsigned int leftVBO;
    unsigned int rightVAO;
    unsigned int rightVBO;

    setupTriangleVAO(leftVAO, leftVBO, leftTriangle, sizeof(leftTriangle));
    setupTriangleVAO(rightVAO, rightVBO, rightTriangle, sizeof(rightTriangle));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.11f, 0.13f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // 渲染阶段只绑定 VAO。VAO 已经记住：
        // 1. 顶点属性 0 是否启用
        // 2. 属性 0 来自哪个 VBO
        // 3. 每个顶点属性如何按 stride / offset 读取
        glBindVertexArray(leftVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(rightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &leftVAO);
    glDeleteVertexArrays(1, &rightVAO);
    glDeleteBuffers(1, &leftVBO);
    glDeleteBuffers(1, &rightVBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void setupTriangleVAO(unsigned int& VAO, unsigned int& VBO, const float* vertices, unsigned int byteSize)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 关键顺序：
    // 先绑定 VAO，再绑定 VBO，再调用 glVertexAttribPointer。
    // 这样 VAO 才能记录“这个属性来自当前绑定的 VBO，以及应该怎么读”。
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, byteSize, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

unsigned int compileShader(unsigned int type, const char* source)
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

unsigned int createShaderProgram()
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
