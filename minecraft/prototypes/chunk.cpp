#include <vector>

#include <OpenGL/OpenGL.h>

#include "chunk.h"

enum Faces {
    NORTH,
    WEST,
    SOUTH,
    EAST,
    UP,
    DOWN,
};

constexpr GLfloat faces[][18] = {
    { 0, 0, 0, 0, 0, 1, 1, 1, 0 },
};

std::vector<GLfloat> Chunk::createMesh() {
    auto v = std::vector<GLfloat>();

    for (int x = 0; x < 16; x++) {
        for (int y = -64; y < 320; y++) {
            for (int z = 0; z < 16; z++) {
                
            }
        }
    }

    return v;
}