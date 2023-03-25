cmake_minimum_required(VERSION 3.2)
project(gl VERSION 0.1.0)

add_executable(gl main.cpp load_shaders.cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

set_property(TARGET gl PROPERTY CXX_STANDARD 23)

target_link_libraries(gl OpenGL::GL glfw GLEW::GLEW)
