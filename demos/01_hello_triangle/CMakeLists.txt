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

add_executable(hello_triangle src/main.cpp)
target_link_libraries(hello_triangle PRIVATE glad glfw OpenGL::GL)

add_executable(two_triangles src/two_triangles.cpp)
target_link_libraries(two_triangles PRIVATE glad glfw OpenGL::GL)

add_executable(framework_triangles src/framework_triangles.cpp)
target_link_libraries(framework_triangles PRIVATE Framework glfw OpenGL::GL)

if(APPLE)
    target_compile_definitions(hello_triangle PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_definitions(two_triangles PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_definitions(framework_triangles PRIVATE GL_SILENCE_DEPRECATION)
endif()
