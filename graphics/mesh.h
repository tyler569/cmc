#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <vector>

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

#endif