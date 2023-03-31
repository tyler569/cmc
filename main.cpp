#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fmt/core.h>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#include "graphics/camera_controller.h"
#include "graphics/load_bmp.h"
#include "graphics/load_shaders.h"
#include "graphics/mesh.h"
#include "minecraft/connection.h"

// one of each block / fake chunk
#define TEXTURE_DEMO 0
#define ASIO_TEST 0

bool glfwStarted = false;

void errorExit(std::string message);

uint8_t chunk[16][16][16];

void generateMesh(Mesh &m) {
    double time = glfwGetTime();
    m.vertices.clear();

#if TEXTURE_DEMO
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            m.emitFace(Mesh::FRONT, x * 2, y * 2, 0, y * 16 + x);
            m.emitFace(Mesh::BACK, x * 2, y * 2, 0, y * 16 + x);
            m.emitFace(Mesh::TOP, x * 2, y * 2, 0, y * 16 + x);
            m.emitFace(Mesh::BOTTOM, x * 2, y * 2, 0, y * 16 + x);
            m.emitFace(Mesh::LEFT, x * 2, y * 2, 0, y * 16 + x);
            m.emitFace(Mesh::RIGHT, x * 2, y * 2, 0, y * 16 + x);
        }
    }
#else
    for (int x = 0; x < 16; x++)
    for (int y = 0; y < 16; y++)
    for (int z = 0; z < 16; z++) {
        if (chunk[x][y][z] != 0 && (x - 1 < 0 || chunk[x-1][y][z] == 0)) {
            m.emitFace(Mesh::LEFT, x, y, z, chunk[x][y][z]);
        }
        if (chunk[x][y][z] != 0 && (x + 1 > 15 || chunk[x+1][y][z] == 0)) {
            m.emitFace(Mesh::RIGHT, x, y, z, chunk[x][y][z]);
        }
        if (chunk[x][y][z] != 0 && (y - 1 < 0 || chunk[x][y-1][z] == 0)) {
            m.emitFace(Mesh::BOTTOM, x, y, z, chunk[x][y][z]);
        }
        if (chunk[x][y][z] != 0 && (y + 1 > 15 || chunk[x][y+1][z] == 0)) {
            m.emitFace(Mesh::TOP, x, y, z, chunk[x][y][z]);
        }
        if (chunk[x][y][z] != 0 && (z - 1 < 0 || chunk[x][y][z-1] == 0)) {
            m.emitFace(Mesh::FRONT, x, y, z, chunk[x][y][z]);
        }
        if (chunk[x][y][z] != 0 && (z + 1 > 15 || chunk[x][y][z+1] == 0)) {
            m.emitFace(Mesh::BACK, x, y, z, chunk[x][y][z]);
        }
    }
#endif

    m.build();

    fmt::print("mesh generation took {} ms\n", (glfwGetTime() - time) * 1000);
}

int main() {
#if ASIO_TEST
    asyncmain();
    return 0;
#endif

    std::srand(1);

    for (int x = 0; x < 16; x++)
    for (int y = 0; y < 8; y++)
    for (int z = 0; z < 16; z++) {
        chunk[x][y][z] = std::rand() % 2 ? 1 : 16;
    }

    chunk[8][3][8] = 0;
    chunk[8][8][8] = 23;
    chunk[8][9][8] = 23;

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

    GLFWwindow *window = glfwCreateWindow(1000, 600, "Demo", nullptr, nullptr);
    if (!window) {
        errorExit("Failed to create window");
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        errorExit("Failed to initialize GLEW");
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    Mesh mesh;

    auto updateMesh = [&mesh]() {
        generateMesh(mesh);
    };

    updateMesh();

    GLuint texture = loadTextureArray("../terrain.png", 16, 16, 16, 16);

    GLuint shaderProgram = loadShaders("../shader_vertex.glsl", "../shader_fragment.glsl");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

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

        mesh.render(shaderProgram, texture, vp, model_base);

        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            chunk[0][0][0] = 0;
            updateMesh();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
}

void errorExit(std::string message) {
    fmt::print("Error: {}\n", message);
    std::exit(1);
    if (glfwStarted) {
        glfwTerminate();
    }
}
