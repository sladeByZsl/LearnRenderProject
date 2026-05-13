cmake_minimum_required(VERSION 3.16)

project(vertex_data LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(glad STATIC ../01_hello_triangle/external/glad/src/glad.c)
target_include_directories(glad PUBLIC ../01_hello_triangle/external/glad/include)

add_executable(vertex_data src/main.cpp)
target_link_libraries(vertex_data PRIVATE glad glfw OpenGL::GL)

if(APPLE)
    target_compile_definitions(vertex_data PRIVATE GL_SILENCE_DEPRECATION)
endif()
