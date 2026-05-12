#include <Framework.hpp>

#include <memory>

#ifndef SHADER_DIR
#define SHADER_DIR "shaders"
#endif

class FrameworkTrianglesDemo : public lr::Application
{
public:
    FrameworkTrianglesDemo()
        : lr::Application({800, 600, "LearnOpenGL - Framework Triangles", SHADER_DIR})
    {
    }

protected:
    void onStart() override
    {
        setClearColor({0.2f, 0.3f, 0.3f, 1.0f});

        shader = loadShader("framework_triangle.vert", "framework_triangle.frag");
        triangleMesh = lr::Mesh::triangle();

        smallTriangle = std::make_unique<lr::GameObject>(
            *triangleMesh,
            lr::Vec2{-0.45f, -0.10f},
            lr::Vec2{0.55f, 0.55f},
            lr::Color{1.0f, 0.5f, 0.2f, 1.0f}
        );

        tallTriangle = std::make_unique<lr::GameObject>(
            *triangleMesh,
            lr::Vec2{0.35f, 0.05f},
            lr::Vec2{0.75f, 1.10f},
            lr::Color{0.2f, 0.7f, 1.0f, 1.0f}
        );
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
