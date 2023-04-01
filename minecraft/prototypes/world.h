#include <cstdint>
#include <tuple>
#include <optional>
#include <unordered_map>

#include "minecraft/prototypes/chunk.h"

struct World {
    std::unordered_map<std::tuple<int, int>, Chunk *> chunks;

    Chunk *chunkAt(int x, int z) {
        auto tuple = std::tuple(x, z);

        if (chunks.contains(tuple)) {
           return chunks[tuple];
        } else {
            return nullptr;
        }
    }

    uint16_t blockAt(int x, int y, int z) {
        Chunk *chunk = chunkAt(x / 16, z / 16);
        if (!chunk) return 0;

        return chunk->block(x % 16, y, z % 16);
    }

    void generateChunk(int x, int z) {
        Chunk *c = Chunk::generate(x, z);
        chunks[std::tuple(x, z)] = c;
    }
};
