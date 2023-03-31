#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/mesh.h"

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

void Mesh::emitFace(Face face, GLfloat x, GLfloat y, GLfloat z, GLfloat index) {
    const auto &offsets = faceOffsets[face];
    const auto &uvs = uvOffsets[face];
    const auto &normal = normals[face];
    for (size_t i = 0, j = 0; i < std::size(offsets); i += 3, j += 2) {
        vertices.push_back(Vertex{
            {x + offsets[i], y + offsets[i + 1], z + offsets[i + 2]},
            {normal[0], normal[1], normal[2]},
            {uvs[j], uvs[j + 1]},
            index,
        });
    }
}

void Mesh::render(GLuint shaderProgram, GLuint texture, glm::mat4 &vp, glm::mat4 &model) const {
    glUseProgram(shaderProgram);

    glm::mat4 mvp = vp * model;

    GLuint matrixId = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

    GLuint timeId = glGetUniformLocation(shaderProgram, "time");
    glUniform1f(timeId, (float)glfwGetTime());

    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    constexpr size_t stride = sizeof(Vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, uv));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(Vertex, textureIndex));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount());

    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}