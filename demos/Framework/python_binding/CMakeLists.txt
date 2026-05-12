# Python extension lr_framework. When used as a subdirectory, parent must define
# targets: glad, Framework (with same layout as demos/01_hello_triangle).

cmake_minimum_required(VERSION 3.16)

if(NOT TARGET Framework)
    message(FATAL_ERROR "python_binding: add this directory after glad and Framework targets exist (see demos/01_hello_triangle/CMakeLists.txt).")
endif()

find_package(Python3 3.8 COMPONENTS Interpreter Development.Module REQUIRED)

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.12.0
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(lr_framework MODULE bindings.cpp)
target_include_directories(lr_framework PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(lr_framework PRIVATE Framework)

if(APPLE)
    target_compile_definitions(lr_framework PRIVATE GL_SILENCE_DEPRECATION)
endif()

set_target_properties(lr_framework PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/python")

message(STATUS "lr_framework module output: ${CMAKE_BINARY_DIR}/python")
