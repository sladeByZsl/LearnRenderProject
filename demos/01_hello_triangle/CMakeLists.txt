cmake_minimum_required(VERSION 3.16)

project(hello_triangle LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(unity_style STATIC
    ../../unity_style/src/UnityStyle.cpp
)
target_include_directories(unity_style PUBLIC
    ../../unity_style/include
)
target_link_libraries(unity_style PUBLIC glad)

add_executable(hello_triangle src/main.cpp)
target_link_libraries(hello_triangle PRIVATE glad glfw OpenGL::GL)

add_executable(two_triangles src/two_triangles.cpp)
target_link_libraries(two_triangles PRIVATE glad glfw OpenGL::GL)

add_executable(unity_style_triangles src/unity_style_triangles.cpp)
target_link_libraries(unity_style_triangles PRIVATE unity_style glfw OpenGL::GL)

if(APPLE)
    target_compile_definitions(hello_triangle PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_definitions(two_triangles PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_definitions(unity_style_triangles PRIVATE GL_SILENCE_DEPRECATION)
endif()
