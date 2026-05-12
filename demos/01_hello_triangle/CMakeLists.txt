cmake_minimum_required(VERSION 3.16)

project(hello_triangle LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(Framework STATIC
    ../Framework/src/Framework.cpp
)
target_include_directories(Framework PUBLIC
    ../Framework/include
)
target_link_libraries(Framework PUBLIC glad glfw OpenGL::GL)

option(LEARNOGL_BUILD_PYTHON_MODULE "Build lr_framework pybind11 module" ON)
if(LEARNOGL_BUILD_PYTHON_MODULE)
    find_package(Python3 3.8 COMPONENTS Interpreter Development.Module QUIET)
    if(Python3_Development.Module_FOUND)
        add_subdirectory(../Framework/python_binding ${CMAKE_CURRENT_BINARY_DIR}/lr_framework_py)
    else()
        message(STATUS "Python3 Development.Module not found; skipping lr_framework (install python3-dev or use -DLEARNOGL_BUILD_PYTHON_MODULE=OFF)")
    endif()
endif()

add_executable(hello_triangle src/main.cpp)
target_link_libraries(hello_triangle PRIVATE glad glfw OpenGL::GL)

add_executable(two_triangles src/two_triangles.cpp)
target_link_libraries(two_triangles PRIVATE glad glfw OpenGL::GL)

add_executable(framework_triangles src/framework_triangles.cpp)
target_link_libraries(framework_triangles PRIVATE Framework glfw OpenGL::GL)
target_compile_definitions(framework_triangles PRIVATE
    SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
)

add_executable(framework_two_triangles src/framework_two_triangles.cpp)
target_link_libraries(framework_two_triangles PRIVATE Framework glfw OpenGL::GL)
target_compile_definitions(framework_two_triangles PRIVATE
    SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
)

if(APPLE)
    target_compile_definitions(hello_triangle PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_definitions(two_triangles PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_definitions(framework_triangles PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_definitions(framework_two_triangles PRIVATE GL_SILENCE_DEPRECATION)
endif()
