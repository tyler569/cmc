#include <bit>
#include <cstdint>
#include <string>
#include <vector>
#include "minecraft/varint.h"
#include "util/byteswap.h"

struct packet {
    int id;
};

struct packetSerializer {
    std::vector<uint8_t> data;

    void writeVarint(int i) {
        ::writeVarint(i, data);
    }

    void writeShort(short i) {
        auto value = byteswap(static_cast<uint32_t>(i));
    }

    void writeString(const std::string &str) {
        writeVarint(str.size());
        data.insert(data.end(), str.begin(), str.end());
    }

    void writeFloat(float f) {
        auto v = reinterpret_cast<uint32_t *>(&f);
        auto value = byteswap(*v);
    }
};