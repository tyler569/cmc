#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <fmt/core.h>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#include "camera_controller.h"
#include "cube_data.cpp"
#include "load_bmp.h"
#include "load_shaders.h"
#include "util.h"

bool glfwStarted = false;

void renderModel(GLuint, GLuint, glm::mat4&, glm::mat4&, int);
void openGlErrorCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam
);

struct Vertex {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat uv[2];
    GLfloat textureIndex;
};

std::ostream& operator <<(std::ostream& os, const Vertex& v) {
    os << "{"
        << v.position[0] << ", "
        << v.position[1] << ", "
        << v.position[2] << ", "
        << v.uv[0] << ", "
        << v.uv[1] << ", "
        << v.textureIndex << "}";
    return os;
}

struct Mesh {
    static constexpr GLfloat duv = 1.;
    static constexpr GLfloat faceOffsets[6][18] = {
        {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0}, // Front
        {1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1}, // Back
        {1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1}, // Right
        {0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0}, // Left
        {0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0}, // Top
        {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1}, // Bottom
    };

    static constexpr GLfloat uvOffsets[6][12] = {
        {1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1}, // Front
        {1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1}, // Back
        {1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1}, // Right
        {1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1}, // Left
        {1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1}, // Top
        {1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0}, // Bottom
    };

    static constexpr GLfloat normals[6][3] = {
        {-1, 0, 0}, // Front
        {1, 0, 0},  // Back
        {0, 0, -1}, // Right
        {0, 0, 1},  // Left
        {0, 1, 0},  // Top
        {0, -1, 0}, // Bottom
    };

    enum Face {
        FRONT,
        BACK,
        RIGHT,
        LEFT,
        TOP,
        BOTTOM,
    };

    std::vector<Vertex> vertices;

    void emitFace(Face face, GLfloat x, GLfloat y, GLfloat z, GLfloat index) {
        const auto& offsets = faceOffsets[face];
        const auto& uvs = uvOffsets[face];
        const auto& normal = normals[face];
        for (size_t i = 0, j = 0; i < std::size(offsets); i += 3, j += 2) {
            vertices.push_back(Vertex{
                {x + offsets[i], y + offsets[i + 1], z + offsets[i + 2]},
                {normal[0], normal[1], normal[2]},
                {uvs[j], uvs[j + 1]},
                index,
            });
        }
    }

    [[nodiscard]] size_t dataLen() const {
        return vertexCount() * sizeof(Vertex);
    }

    [[nodiscard]] GLfloat *dataPtr() const {
        return (GLfloat *)&vertices[0];
    }

    [[nodiscard]] size_t vertexCount() const {
        return vertices.size();
    }
};

uint8_t chunk[16][16][16];

Mesh generateMesh() {

    double time = glfwGetTime();

    Mesh m = {};

#if 0
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
    for (int y = 0; y < 8; y++)
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

    fmt::print("mesh generation took {} ms\n", (glfwGetTime() - time) * 1000);

    return m;
}

int main() {
    std::srand(1);

    for (int x = 0; x < 16; x++)
    for (int y = 0; y < 8; y++)
    for (int z = 0; z < 16; z++) {
        chunk[x][y][z] = std::rand() % 2 ? 1 : 16;
    }

    chunk[8][3][8] = 0;

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

    Mesh meshObject;
    size_t points;
    GLuint mesh;
    glGenBuffers(1, &mesh);

    auto updateMesh = [&meshObject, &points, mesh]() {
        meshObject = generateMesh();
        points = meshObject.vertexCount();

        glBindBuffer(GL_ARRAY_BUFFER, mesh);
        glBufferData(GL_ARRAY_BUFFER, meshObject.dataLen(), meshObject.dataPtr(), GL_STATIC_DRAW);
    };

    updateMesh();

    loadBmp("../terrain.png");

    GLuint shaderProgram = loadShaders("../shader_vertex.glsl", "../shader_fragment.glsl");

    glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(openGlErrorCallback, 0);
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

        renderModel(shaderProgram, mesh, vp, model_base, points);

        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            chunk[0][0][0] = 0;
            updateMesh();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));
}

void renderModel(GLuint shaderProgram, GLuint meshBuffer, glm::mat4& vp, glm::mat4& model, int points) {
    glUseProgram(shaderProgram);

    glm::mat4 mvp = vp * model;

    GLuint matrixId = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

    GLuint timeId = glGetUniformLocation(shaderProgram, "time");
    glUniform1f(timeId, (float)glfwGetTime());

    glBindBuffer(GL_ARRAY_BUFFER, meshBuffer);

    constexpr size_t stride = sizeof(Vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, uv));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, textureIndex));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glDrawArrays(GL_TRIANGLES, 0, points);

    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
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

void openGlErrorCallback(
    GLenum _source,
    GLenum type,
    GLuint _id,
    GLenum severity,
    GLsizei _length,
    const GLchar *message,
    const void *_userParam
) {
    fmt::print(stderr, "GL callback {} type {} severity {} message {}\n",
        type == GL_DEBUG_TYPE_ERROR ? "ERROR" : "info",
        type, severity, message);
}
