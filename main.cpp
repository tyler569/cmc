#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fmt/core.h>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#include "camera_controller.h"
#include "cube_data.cpp"
#include "load_bmp.h"
#include "load_shaders.h"
#include "util.h"

bool glfwStarted = false;

void renderModel(GLFWwindow *, GLuint, GLuint, glm::mat4&, glm::mat4&, int);

std::vector<GLfloat> faceUv(int x, int y) {
    std::vector<GLfloat> faceUv = {};
    GLfloat xv = x / 16.;
    GLfloat yv = y / 16.;
    GLfloat dv = 1. / 16.;

    for (int i = 0; i < 12; i++) {
        if (i & 1) {
            faceUv.push_back(yv + dv * cubeUvs[i]);
        } else {
            faceUv.push_back(xv + dv * cubeUvs[i]);
        }
    }

    return faceUv;
}

struct vertex {
    float position[3];
    float uv[2];
};

std::vector<GLfloat> generateMesh() {
    constexpr GLfloat duv = 1. / 16.;

    auto emitFront = [](std::vector<GLfloat> &mesh, GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        mesh.insert(mesh.end(), {
            x, y, z, u, v,
            x, y+1, z, u, v+duv,
            x+1, y, z, u+duv, v,
            x, y+1, z, u, v+duv,
            x+1, y+1, z, u+duv, v+duv,
            x+1, y, z, u+duv, v
        });
    };

    auto emitRight = [](std::vector<GLfloat> &mesh, GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        mesh.insert(mesh.end(), {
            x+1, y, z, u, v,
            x+1, y+1, z, u, v+duv,
            x+1, y, z+1, u+duv, v,
            x+1, y+1, z, u, v+duv,
            x+1, y+1, z+1, u+duv, v+duv,
            x+1, y, z+1, u+duv, v
        });
    };

    auto emitBack = [](std::vector<GLfloat> &mesh, GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        mesh.insert(mesh.end(), {
            x+1, y, z+1, u, v,
            x+1, y+1, z+1, u, v+duv,
            x, y, z+1, u+duv, v,
            x+1, y+1, z+1, u, v+duv,
            x, y+1, z+1, u+duv, v+duv,
            x, y, z+1, u+duv, v
        });
    };

    std::vector<GLfloat> mesh = {};

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            emitFront(mesh, x, y, 0, x / 16., y / 16.);
        }
    }

    // emitFront(mesh, 0, 0, 0, 0, 0);
    // emitBack(mesh, 0, 0, 0, 0, 0);
    // emitRight(mesh, 0, 0, 0, 0, 0);

    // fmt::print("mesh: ");
    // for (auto v : mesh) {
    //     fmt::print("{} ", v);
    // }
    // fmt::print("\n");

    return mesh;
}

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

    GLFWwindow *window = glfwCreateWindow(1000, 600, "Demo", NULL, NULL);
    if (!window) {
        errorExit("Failed to create window");
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        errorExit("Failed to initialize GLEW");
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    /*
    std::vector<GLfloat> cube = {};
    std::vector<GLfloat> cubeColor = {};
    std::vector<GLfloat> cubeUv = {};

    auto blockUv = faceUv(9, 0);

    for (int i = 0; i < 6; i++) {
        cube.insert(cube.end(), std::begin(cubeSides[i]), std::end(cubeSides[i]));
        cubeColor.insert(cubeColor.end(), std::begin(cubeColors[i]), std::end(cubeColors[i]));
        cubeUv.insert(cubeUv.end(), std::begin(blockUv), std::end(blockUv));
    }

*/
    // {
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    /*

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof(GLfloat), &cube[0], GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeColor.size() * sizeof(GLfloat), &cubeColor[0], GL_STATIC_DRAW);

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, cubeUv.size() * sizeof(GLfloat), &cubeUv[0], GL_STATIC_DRAW);
    */
    // }

    std::vector<GLfloat> meshVector = generateMesh();
    int points = meshVector.size() / 5;

    GLuint mesh;
    glGenBuffers(1, &mesh);
    glBindBuffer(GL_ARRAY_BUFFER, mesh);
    glBufferData(GL_ARRAY_BUFFER, meshVector.size() * sizeof(GLfloat), &meshVector[0], GL_STATIC_DRAW);

    GLuint texture = loadBmp("../terrain.bmp");

    GLuint shaderProgram = loadShaders("../shader_vertex.glsl", "../shader_fragment.glsl");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glEnable(GL_CULL_FACE);

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

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        controller.update();
        auto vp = controller.vp();

        auto model_base = glm::mat4(1.f);

        // for (int x = -10; x <= 10; x += 1) {
        //     for (int y = -10; y <= 10; y += 1) {
        //         auto model = glm::translate(model_base, glm::vec3(x * 2, 0, y * 2));
        //         // model = glm::rotate(model, (float)glfwGetTime() / 10.f, glm::vec3(0, 1, 0));
        //         // model = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
        //         renderModel(window, shaderProgram, mesh, vp, model);
        //     }
        // }

        renderModel(window, shaderProgram, mesh, vp, model_base, points);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
}

void renderModel(GLFWwindow *window, GLuint shaderProgram, GLuint meshBuffer, glm::mat4& vp, glm::mat4& model, int points) {
    glUseProgram(shaderProgram);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspect_ratio = (float) width / (float) height;

    glm::mat4 mvp = vp * model;

    GLuint matrix_id = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, meshBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, points);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void errorExit(std::string message) {
    fmt::print("Error: {}\n", message);
    std::exit(1);
    if (glfwStarted) {
        glfwTerminate();
    }
}