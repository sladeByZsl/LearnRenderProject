#include <Framework.hpp>

#include <memory>
#include <string>

#ifndef SHADER_DIR
#define SHADER_DIR "shaders"
#endif

class FrameworkTrianglesDemo : public lr::Application
{
public:
    FrameworkTrianglesDemo()
        : lr::Application({800, 600, "LearnOpenGL - Framework Triangles"})
    {
    }

protected:
    void onStart() override
    {
        setClearColor({0.2f, 0.3f, 0.3f, 1.0f});

        shader = lr::ShaderProgram::fromFiles(
            shaderPath("framework_triangle.vert").c_str(),
            shaderPath("framework_triangle.frag").c_str()
        );

        triangleMesh = std::make_unique<lr::Mesh>(std::vector<lr::Vec3>{
            {-0.5f, -0.5f, 0.0f},
            { 0.5f, -0.5f, 0.0f},
            { 0.0f,  0.5f, 0.0f}
        });

        smallTriangle = std::make_unique<lr::GameObject>(*triangleMesh);
        smallTriangle->transform.position = {-0.45f, -0.10f};
        smallTriangle->transform.scale = {0.55f, 0.55f};
        smallTriangle->material.color = {1.0f, 0.5f, 0.2f, 1.0f};

        tallTriangle = std::make_unique<lr::GameObject>(*triangleMesh);
        tallTriangle->transform.position = {0.35f, 0.05f};
        tallTriangle->transform.scale = {0.75f, 1.10f};
        tallTriangle->material.color = {0.2f, 0.7f, 1.0f, 1.0f};
    }

    void onRender() override
    {
        smallTriangle->draw(*shader);
        tallTriangle->draw(*shader);
    }

    void onShutdown() override
    {
        tallTriangle.reset();
        smallTriangle.reset();
        triangleMesh.reset();
        shader.reset();
    }

private:
    static std::string shaderPath(const char* fileName)
    {
        return std::string(SHADER_DIR) + "/" + fileName;
    }

    std::unique_ptr<lr::ShaderProgram> shader;
    std::unique_ptr<lr::Mesh> triangleMesh;
    std::unique_ptr<lr::GameObject> smallTriangle;
    std::unique_ptr<lr::GameObject> tallTriangle;
};

int main()
{
    FrameworkTrianglesDemo app;
    return app.run();
}
