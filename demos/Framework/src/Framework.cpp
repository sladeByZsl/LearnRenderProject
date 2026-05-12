#include "Framework.hpp"

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

namespace lr
{

Mesh::Mesh(const std::vector<Vec3>& localVertices)
    : vertices(localVertices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertices.size()));
}

ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
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

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id);
}

void ShaderProgram::use() const
{
    glUseProgram(id);
}

void ShaderProgram::setVec2(const char* name, const Vec2& value) const
{
    glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}

void ShaderProgram::setColor(const char* name, const Color& value) const
{
    glUniform4f(glGetUniformLocation(id, name), value.r, value.g, value.b, value.a);
}

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

GameObject::GameObject(const Mesh& mesh)
    : mesh(mesh)
{
}

void GameObject::draw(const ShaderProgram& shader) const
{
    shader.use();
    shader.setVec2("uPosition", transform.position);
    shader.setVec2("uScale", transform.scale);
    shader.setColor("uColor", material.color);
    mesh.draw();
}

Application::Application(const ApplicationConfig& config)
    : config(config)
{
}

int Application::run()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* glfwWindow = glfwCreateWindow(config.width, config.height, config.title, nullptr, nullptr);
    if (glfwWindow == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    window = glfwWindow;
    glfwMakeContextCurrent(glfwWindow);
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow* callbackWindow, int width, int height) {
        framebufferSizeCallback(callbackWindow, width, height);
    });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        window = nullptr;
        return -1;
    }

    onStart();

    while (!glfwWindowShouldClose(glfwWindow))
    {
        processDefaultInput();
        onUpdate();

        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        onRender();

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    onShutdown();

    glfwTerminate();
    window = nullptr;
    return 0;
}

void Application::setClearColor(const Color& color)
{
    clearColor = color;
}

void Application::close()
{
    if (window != nullptr)
    {
        glfwSetWindowShouldClose(static_cast<GLFWwindow*>(window), true);
    }
}

void* Application::nativeWindow() const
{
    return window;
}

void Application::framebufferSizeCallback(void* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void Application::processDefaultInput()
{
    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window);
    if (glfwWindow != nullptr && glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(glfwWindow, true);
    }
}

} // namespace lr
