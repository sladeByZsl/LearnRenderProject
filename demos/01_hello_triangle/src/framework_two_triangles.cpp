#include <Framework.hpp>

#include <memory>

#ifndef SHADER_DIR
#define SHADER_DIR "shaders"
#endif

class FrameworkTwoTrianglesDemo : public lr::Application
{
public:
    FrameworkTwoTrianglesDemo()
        : lr::Application({800, 600, "LearnOpenGL - Framework Two Triangles", SHADER_DIR})
    {
    }

protected:
    void onStart() override
    {
        setClearColor({0.2f, 0.3f, 0.3f, 1.0f});

        shader = loadShader("framework_triangle.vert", "framework_triangle.frag");
        triangleMesh = lr::Mesh::triangle();

        leftTriangle = &scene.createGameObject(
            *triangleMesh,
            lr::Vec2{-0.45f, -0.10f},
            lr::Vec2{0.55f, 0.55f},
            lr::Color{1.0f, 0.5f, 0.2f, 1.0f}
        );

        rightTriangle = &scene.createGameObject(
            *triangleMesh,
            lr::Vec2{0.35f, 0.08f},
            lr::Vec2{0.75f, 1.10f},
            lr::Color{0.2f, 0.7f, 1.0f, 1.0f}
        );
    }

    void onRender() override
    {
        scene.draw(*shader);
    }

    void onUpdate() override
    {
        leftTriangle->transform.rotation += deltaTime() * 1.4f;
        rightTriangle->transform.rotation -= deltaTime() * 1.1f;
    }

    void onShutdown() override
    {
        scene.clear();
        triangleMesh.reset();
        shader.reset();
    }

private:
    std::unique_ptr<lr::ShaderProgram> shader;
    std::unique_ptr<lr::Mesh> triangleMesh;
    lr::Scene scene;
    lr::GameObject* leftTriangle = nullptr;
    lr::GameObject* rightTriangle = nullptr;
};

int main()
{
    FrameworkTwoTrianglesDemo app;
    return app.run();
}
