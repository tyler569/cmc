#define GL_SILENCE_DEPRECATION

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

#include "load_shaders.h"

using std::string;

void error_exit(string message);
bool glfw_started = false;

int main() {
    glewExperimental = true;

    if (!glfwInit()) {
        error_exit("Failed to initialize GLFW");
    }
    glfw_started = true;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Demo", NULL, NULL);
    if (!window) {
        error_exit("Failed to create window");
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        error_exit("Failed to initialize GLEW");
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // {
    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    static constexpr GLfloat vertex_buffer_data[] = {
        -1.f, -1.f, 0.f,
        1.f, -1.f, 0.f,
        0.f, 1.f, 0.f,
    };

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    // }

    GLuint shader_program = load_shaders("../shader_vertex.glsl", "../shader_fragment.glsl");

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
}

void error_exit(string message) {
    std::cerr << message << "\n";
    std::exit(1);
    if (glfw_started) {
        glfwTerminate();
    }
}