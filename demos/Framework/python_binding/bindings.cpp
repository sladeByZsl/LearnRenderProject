#include "Framework.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstdint>
#include <memory>

namespace py = pybind11;

namespace
{

class PyApplication : public lr::Application
{
public:
    using lr::Application::Application;

    using lr::Application::loadShader;
    using lr::Application::setClearColor;
    using lr::Application::close;
    using lr::Application::deltaTime;
    using lr::Application::time;
    using lr::Application::nativeWindow;

protected:
    void onStart() override
    {
        PYBIND11_OVERRIDE_NAME(void, lr::Application, "on_start", onStart);
    }

    void onUpdate() override
    {
        PYBIND11_OVERRIDE_NAME(void, lr::Application, "on_update", onUpdate);
    }

    void onRender() override
    {
        PYBIND11_OVERRIDE_NAME(void, lr::Application, "on_render", onRender);
    }

    void onShutdown() override
    {
        PYBIND11_OVERRIDE_NAME(void, lr::Application, "on_shutdown", onShutdown);
    }
};

} // namespace

PYBIND11_MODULE(lr_framework, m)
{
    m.doc() = "LearnOpenGL Framework: window + OpenGL from C++, logic from Python.";

    py::class_<lr::ApplicationConfig>(m, "ApplicationConfig")
        .def(py::init<>())
        .def_readwrite("width", &lr::ApplicationConfig::width)
        .def_readwrite("height", &lr::ApplicationConfig::height)
        .def_readwrite("title", &lr::ApplicationConfig::title)
        .def_readwrite("shader_dir", &lr::ApplicationConfig::shaderDir)
        .def_readwrite("gl_major", &lr::ApplicationConfig::glMajor)
        .def_readwrite("gl_minor", &lr::ApplicationConfig::glMinor);

    py::class_<lr::Vec2>(m, "Vec2")
        .def(py::init<>())
        .def(py::init<float, float>(), py::arg("x"), py::arg("y"))
        .def(py::init([](py::sequence s) {
            return lr::Vec2{s[0].cast<float>(), s[1].cast<float>()};
        }), py::arg("xy"))
        .def_readwrite("x", &lr::Vec2::x)
        .def_readwrite("y", &lr::Vec2::y);
    py::implicitly_convertible<py::tuple, lr::Vec2>();
    py::implicitly_convertible<py::list, lr::Vec2>();

    py::class_<lr::Vec3>(m, "Vec3")
        .def(py::init<>())
        .def(py::init<float, float, float>(), py::arg("x"), py::arg("y"), py::arg("z"))
        .def(py::init([](py::sequence s) {
            return lr::Vec3{s[0].cast<float>(), s[1].cast<float>(), s[2].cast<float>()};
        }), py::arg("xyz"))
        .def_readwrite("x", &lr::Vec3::x)
        .def_readwrite("y", &lr::Vec3::y)
        .def_readwrite("z", &lr::Vec3::z);
    py::implicitly_convertible<py::tuple, lr::Vec3>();
    py::implicitly_convertible<py::list, lr::Vec3>();

    py::class_<lr::Color>(m, "Color")
        .def(py::init<>())
        .def(py::init<float, float, float, float>(), py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a"))
        .def(py::init([](py::sequence s) {
            return lr::Color{s[0].cast<float>(), s[1].cast<float>(),
                             s[2].cast<float>(), s[3].cast<float>()};
        }), py::arg("rgba"))
        .def_readwrite("r", &lr::Color::r)
        .def_readwrite("g", &lr::Color::g)
        .def_readwrite("b", &lr::Color::b)
        .def_readwrite("a", &lr::Color::a);
    py::implicitly_convertible<py::tuple, lr::Color>();
    py::implicitly_convertible<py::list, lr::Color>();

    py::class_<lr::Transform2D>(m, "Transform2D")
        .def(py::init<>())
        .def_readwrite("position", &lr::Transform2D::position)
        .def_readwrite("scale", &lr::Transform2D::scale)
        .def_readwrite("rotation", &lr::Transform2D::rotation);

    py::class_<lr::Material>(m, "Material")
        .def(py::init<>())
        .def_readwrite("color", &lr::Material::color);

    py::class_<lr::Mesh, std::unique_ptr<lr::Mesh>>(m, "Mesh")
        .def(py::init<const std::vector<lr::Vec3>&>(), py::arg("vertices"))
        .def_static(
            "triangle",
            []() { return lr::Mesh::triangle(); }
        )
        .def("draw", &lr::Mesh::draw);

    py::class_<lr::ShaderProgram, std::unique_ptr<lr::ShaderProgram>>(m, "ShaderProgram")
        .def(
            py::init(
                [](const std::string& vertexSource, const std::string& fragmentSource) {
                    return std::make_unique<lr::ShaderProgram>(vertexSource.c_str(), fragmentSource.c_str());
                }
            ),
            py::arg("vertex_source"),
            py::arg("fragment_source")
        )
        .def_static(
            "from_files",
            [](const std::string& vertexPath, const std::string& fragmentPath) {
                return lr::ShaderProgram::fromFiles(vertexPath.c_str(), fragmentPath.c_str());
            }
        )
        .def("use", &lr::ShaderProgram::use)
        .def("set_vec2", &lr::ShaderProgram::setVec2, py::arg("name"), py::arg("value"))
        .def("set_color", &lr::ShaderProgram::setColor, py::arg("name"), py::arg("value"));

    py::class_<lr::GameObject>(m, "GameObject")
        .def_readwrite("transform", &lr::GameObject::transform)
        .def_readwrite("material", &lr::GameObject::material)
        .def("draw", &lr::GameObject::draw, py::arg("shader"));

    py::class_<lr::Scene>(m, "Scene")
        .def(py::init<>())
        .def(
            "create_game_object",
            static_cast<lr::GameObject& (lr::Scene::*)(const lr::Mesh&)>(&lr::Scene::createGameObject),
            py::arg("mesh"),
            py::return_value_policy::reference_internal
        )
        .def(
            "create_game_object",
            static_cast<lr::GameObject& (lr::Scene::*)(const lr::Mesh&, const lr::Vec2&, const lr::Vec2&, const lr::Color&)>(&lr::Scene::createGameObject),
            py::arg("mesh"),
            py::arg("position"),
            py::arg("scale"),
            py::arg("color"),
            py::return_value_policy::reference_internal
        )
        .def("draw", &lr::Scene::draw, py::arg("shader"))
        .def("clear", &lr::Scene::clear);

    py::class_<lr::Application, PyApplication, std::shared_ptr<lr::Application>>(m, "Application")
        .def(
            py::init<const lr::ApplicationConfig&>(),
            py::arg("config") = lr::ApplicationConfig{}
        )
        .def("run", &lr::Application::run)
        .def(
            "load_shader",
            [](lr::Application& base, const char* vertexFile, const char* fragmentFile) {
                return static_cast<PyApplication&>(base).loadShader(vertexFile, fragmentFile);
            },
            py::arg("vertex_file"),
            py::arg("fragment_file"),
            py::return_value_policy::take_ownership
        )
        .def(
            "set_clear_color",
            [](lr::Application& base, const lr::Color& color) {
                static_cast<PyApplication&>(base).setClearColor(color);
            },
            py::arg("color")
        )
        .def(
            "close",
            [](lr::Application& base) {
                static_cast<PyApplication&>(base).close();
            }
        )
        .def("delta_time", &PyApplication::deltaTime)
        .def("time", &PyApplication::time)
        .def(
            "native_window",
            [](const PyApplication& self) {
                return reinterpret_cast<std::uintptr_t>(self.nativeWindow());
            }
        );
}
