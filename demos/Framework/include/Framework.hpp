#ifndef LEARN_RENDER_FRAMEWORK_HPP
#define LEARN_RENDER_FRAMEWORK_HPP

#include <vector>

namespace lr
{

struct ApplicationConfig
{
    int width = 800;
    int height = 600;
    const char* title = "LearnRender";
    int glMajor = 3;
    int glMinor = 3;
};

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

struct Material
{
    Color color = {1.0f, 0.5f, 0.2f, 1.0f};
};

class Mesh
{
public:
    explicit Mesh(const std::vector<Vec3>& localVertices);

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    ~Mesh();

    void draw() const;

private:
    std::vector<Vec3> vertices;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
};

class ShaderProgram
{
public:
    ShaderProgram(const char* vertexSource, const char* fragmentSource);

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ~ShaderProgram();

    void use() const;
    void setVec2(const char* name, const Vec2& value) const;
    void setColor(const char* name, const Color& value) const;

private:
    static unsigned int compileShader(unsigned int type, const char* source);

    unsigned int id = 0;
};

class GameObject
{
public:
    Transform2D transform;
    Material material;

    explicit GameObject(const Mesh& mesh);

    void draw(const ShaderProgram& shader) const;

private:
    const Mesh& mesh;
};

class Application
{
public:
    explicit Application(const ApplicationConfig& config = {});

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    virtual ~Application() = default;

    int run();

protected:
    virtual void onStart() {}
    virtual void onUpdate() {}
    virtual void onRender() {}
    virtual void onShutdown() {}

    void setClearColor(const Color& color);
    void close();
    void* nativeWindow() const;

private:
    static void framebufferSizeCallback(void* window, int width, int height);
    void processDefaultInput();

    ApplicationConfig config;
    Color clearColor = {0.2f, 0.3f, 0.3f, 1.0f};
    void* window = nullptr;
};

} // namespace lr

#endif
