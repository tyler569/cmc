#include <cstdint>
#include <cstdlib>

#include <OpenGL/OpenGL.h>
#include <fmt/core.h>

struct Chunk {
    constexpr static size_t COUNT = 16 * 16 * 384;

    int x;
    int z;
    uint16_t data[COUNT];

    static size_t id(int x, int y, int z) {
        if (x > 15 || z > 15 || y > 320 || x < 0 || z < 0 || y < -64) {
            fmt::print(stderr, "Invalid block id {} {} {}\n", x, y, z);
            std::exit(2);
        }

        return (y + 64) * 256 + z * 16 + x;
    }

    uint16_t& block(size_t id) { return data[id]; }
    uint16_t& block(int x, int y, int z) { return data[id(x, y, z)]; }

    Chunk(int x, int z) : x(x), z(z) {}

    static Chunk *generate(int x, int z) {
        auto c = new Chunk{x, z};

        for (int i = 0; i < 512; i++) {
            c->block(i) = 1;
        }

        return c;
    }

    std::vector<GLfloat> createMesh();
};