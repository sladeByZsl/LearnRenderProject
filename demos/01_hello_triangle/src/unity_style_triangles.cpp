#include <UnityStyle.hpp>

#include <memory>

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

class UnityStyleTrianglesDemo : public lr::Application
{
public:
    UnityStyleTrianglesDemo()
        : lr::Application({800, 600, "LearnOpenGL - Unity Style Triangles"})
    {
    }

protected:
    void onStart() override
    {
        setClearColor({0.2f, 0.3f, 0.3f, 1.0f});

        shader = std::make_unique<lr::ShaderProgram>(vertexShaderSource, fragmentShaderSource);

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
    std::unique_ptr<lr::ShaderProgram> shader;
    std::unique_ptr<lr::Mesh> triangleMesh;
    std::unique_ptr<lr::GameObject> smallTriangle;
    std::unique_ptr<lr::GameObject> tallTriangle;
};

int main()
{
    UnityStyleTrianglesDemo app;
    return app.run();
}
