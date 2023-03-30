#include <vector>

#include <GL/glew.h>

struct Vertex {
    GLfloat position[3];
    GLfloat uv[2];

    static void setupGL() {

    }
};

struct Mesh {
    std::vector<Vertex> vertices;

    std::vector<Vertex> *operator->() {
        return &vertices;
    }

    size_t count() const {
        return vertices.size();
    }

    size_t dataLen() const {
        return count() * sizeof(Vertex);
    }

    GLfloat *dataPtr() const {
        return (GLfloat *)&vertices[0];
    }
};
