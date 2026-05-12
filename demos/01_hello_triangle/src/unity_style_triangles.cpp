#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>
#include <vector>

static const unsigned int SCR_WIDTH = 800;
static const unsigned int SCR_HEIGHT = 600;

struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct Color
{
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

struct Transform2D
{
    Vec2 position = {0.0f, 0.0f};
    Vec2 scale = {1.0f, 1.0f};
};

class Mesh
{
public:
    explicit Mesh(const std::vector<Vec3>& localVertices)
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

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    ~Mesh()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void draw() const
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertices.size()));
    }

private:
    std::vector<Vec3> vertices;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
};

class ShaderProgram
{
public:
    ShaderProgram(const char* vertexSource, const char* fragmentSource)
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

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ~ShaderProgram()
    {
        glDeleteProgram(id);
    }

    void use() const
    {
        glUseProgram(id);
    }

    void setVec2(const char* name, const Vec2& value) const
    {
        glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
    }

    void setColor(const char* name, const Color& value) const
    {
        glUniform4f(glGetUniformLocation(id, name), value.r, value.g, value.b, value.a);
    }

private:
    static unsigned int compileShader(unsigned int type, const char* source)
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

    unsigned int id = 0;
};

struct Material
{
    Color color = {1.0f, 0.5f, 0.2f, 1.0f};
};

class GameObject
{
public:
    Transform2D transform;
    Material material;

    explicit GameObject(const Mesh& mesh)
        : mesh(mesh)
    {
    }

    void draw(const ShaderProgram& shader) const
    {
        shader.use();
        shader.setVec2("uPosition", transform.position);
        shader.setVec2("uScale", transform.scale);
        shader.setColor("uColor", material.color);
        mesh.draw();
    }

private:
    const Mesh& mesh;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec2 uPosition;
uniform vec2 uScale;

void main()
{
    vec2 scaled = aPos.xy * uScale;
    vec2 moved = scaled + uPosition;
    gl_Position = vec4(moved, aPos.z, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform vec4 uColor;

void main()
{
    FragColor = uColor;
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL - Unity Style Triangles", nullptr, nullptr);
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

    {
        ShaderProgram shader(vertexShaderSource, fragmentShaderSource);

        Mesh triangleMesh({
            {-0.5f, -0.5f, 0.0f},
            { 0.5f, -0.5f, 0.0f},
            { 0.0f,  0.5f, 0.0f}
        });

        GameObject smallTriangle(triangleMesh);
        smallTriangle.transform.position = {-0.45f, -0.10f};
        smallTriangle.transform.scale = {0.55f, 0.55f};
        smallTriangle.material.color = {1.0f, 0.5f, 0.2f, 1.0f};

        GameObject tallTriangle(triangleMesh);
        tallTriangle.transform.position = {0.35f, 0.05f};
        tallTriangle.transform.scale = {0.75f, 1.10f};
        tallTriangle.material.color = {0.2f, 0.7f, 1.0f, 1.0f};

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            smallTriangle.draw(shader);
            tallTriangle.draw(shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
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
