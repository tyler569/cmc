#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fmt/core.h>

#include "camera_controller.h"
#include "cube_data.cpp"
#include "load_shaders.h"

using std::string;

void errorExit(string message);
bool glfwStarted = false;

void renderModel(GLFWwindow *, GLuint, GLuint, GLuint, glm::mat4&, glm::mat4&);

int main() {
    glewExperimental = true;

    if (!glfwInit()) {
        errorExit("Failed to initialize GLFW");
    }
    glfwStarted = true;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Demo", NULL, NULL);
    if (!window) {
        errorExit("Failed to create window");
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        errorExit("Failed to initialize GLEW");
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // {
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData), colorBufferData, GL_STATIC_DRAW);
    // }

    GLuint shaderProgram = loadShaders("../shader_vertex.glsl", "../shader_fragment.glsl");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glClearColor(0.1, 0.2, 0.3, 1.);

    // Give the mouse cursor time to get to the middle of the window?
    // I know, it doesn't make any sense that this should be needed, but alas.
    // I don't know if rendering two frames is needed because of clock time or
    // if it has something to do with event queue clearing or something, but
    // less drastic actions continue to result in the first camera update including
    // mouse offsets that result in the camera flying off into neverland, and this
    // seems to consistently prevent that from happening.
    for (int i = 0; i < 2; i++) {
        int xWindow, yWindow;
        glfwGetWindowSize(window, &xWindow, &yWindow);
        glfwSetCursorPos(window, xWindow / 2., yWindow / 2.);
        glfwPollEvents();
    }

    CameraController controller{window};

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        controller.update();
        auto vp = controller.vp();

        auto model_base = glm::mat4(1.f);

        for (int x = -10; x <= 10; x += 1) {
            for (int y = -10; y <= 10; y += 1) {
                auto model = glm::translate(model_base, glm::vec3(x, 0, y));
                renderModel(window, shaderProgram, vertexBuffer, colorBuffer, vp, model);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
}

void renderModel(GLFWwindow *window, GLuint shaderProgram, GLuint vertexBuffer, GLuint colorBuffer, glm::mat4& vp, glm::mat4& model) {
    glUseProgram(shaderProgram);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspect_ratio = (float) width / (float) height;

    glm::mat4 mvp = vp * model;

    GLuint matrix_id = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void errorExit(string message) {
    fmt::print("Error: {}\n", message);
    std::exit(1);
    if (glfwStarted) {
        glfwTerminate();
    }
}