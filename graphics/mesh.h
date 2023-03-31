#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

struct Vertex {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat uv[2];
    GLfloat textureIndex;
};

std::ostream& operator <<(std::ostream& os, const Vertex& v);

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

    std::vector<Vertex> vertices{};
    GLuint buffer{};

    Mesh() {
        glGenBuffers(1, &buffer);
    }

    Mesh(Mesh &&other) noexcept {
        vertices = std::move(other.vertices);
        buffer = other.buffer;
        other.buffer = 0;
    }

    ~Mesh() {
        glDeleteBuffers(1, &buffer);
    }

    Mesh &operator=(Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    Mesh &operator=(Mesh &&other) noexcept {
        if (this != &other) {
            buffer = other.buffer;
            other.buffer = 0;
            vertices = std::move(other.vertices);
        }
        return *this;
    }

    void emitFace(Face face, GLfloat x, GLfloat y, GLfloat z, GLfloat index);

    [[nodiscard]] size_t dataLen() const {
        return vertexCount() * sizeof(Vertex);
    }

    [[nodiscard]] GLfloat *dataPtr() const {
        return (GLfloat *)&vertices[0];
    }

    [[nodiscard]] size_t vertexCount() const {
        return vertices.size();
    }

    void build() const {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, dataLen(), dataPtr(), GL_STATIC_DRAW);
    }

    void render(GLuint shaderProgram, GLuint texture, glm::mat4 &mv, glm::mat4 &p) const;
};

#endif