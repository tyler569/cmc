#include <cstdint>
#include <utility>
#include <vector>

#include <GL/glew.h>

struct Chunk {
    uint16_t data[16 * 16 * 16];

    bool isBlock(int x, int y, int z) {
        return x >= 0 && x < 16 && y >= 0 && y < 16 && z >= 0 && z < 16;
    }

    uint16_t& block(int x, int y, int z) {
        return data[x + z * 16 + y * 64];
    }

    Chunk() {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 16; x++) {
                for (int z = 0; z < 16; z++) {
                    block(x, y, z) = 1;
                }
            }
        }
    }

    std::pair<std::vector<GLfloat>, std::vector<GLfloat>> mesh() const {
        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                for (int z = 0; z < 16; z++) {

                }
            }
        }
        return {};
    }
};