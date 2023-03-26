#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fmt/core.h>

#include "load_shaders.h"
#include "cube_data.cpp"

using std::string;

void error_exit(string message);
bool glfw_started = false;

void render_model(GLFWwindow *, GLuint, GLuint, GLuint);

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

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    GLuint color_buffer;
    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);
    // }

    GLuint shader_program = load_shaders("../shader_vertex.glsl", "../shader_fragment.glsl");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.1, 0.2, 0.3, 1.);

    do {
        GLFWvidmode video_info;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_model(window, shader_program, vertex_buffer, color_buffer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
}

void render_model(GLFWwindow *window, GLuint shader_program, GLuint vertex_buffer, GLuint color_buffer) {
    glUseProgram(shader_program);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspect_ratio = (float) width / (float) height;

    glm::mat4 projection = glm::perspective(glm::radians(45.f), aspect_ratio, 0.1f, 100.f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(4, 3, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 mvp = projection * view * model;

    GLuint matrix_id = glGetUniformLocation(shader_program, "MVP");
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void error_exit(string message) {
    std::cerr << message << "\n";
    std::exit(1);
    if (glfw_started) {
        glfwTerminate();
    }
}