cmake_minimum_required(VERSION 3.16)

project(vao LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(glad STATIC ../01_hello_triangle/external/glad/src/glad.c)
target_include_directories(glad PUBLIC ../01_hello_triangle/external/glad/include)

add_executable(vao src/main.cpp)
target_link_libraries(vao PRIVATE glad glfw OpenGL::GL)

add_executable(vao_color_attributes src/vao_color_attributes.cpp)
target_link_libraries(vao_color_attributes PRIVATE glad glfw OpenGL::GL)

add_executable(vao_split_buffers src/vao_split_buffers.cpp)
target_link_libraries(vao_split_buffers PRIVATE glad glfw OpenGL::GL)

if(APPLE)
    target_compile_definitions(vao PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_definitions(vao_color_attributes PRIVATE GL_SILENCE_DEPRECATION)
    target_compile_definitions(vao_split_buffers PRIVATE GL_SILENCE_DEPRECATION)
endif()
