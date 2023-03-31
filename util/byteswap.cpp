#include <cstdint>

#include "byteswap.h"

uint8_t byteswap(uint8_t v) {
    return v;
}

uint16_t byteswap(uint16_t v) {
    return v >> 8 | ((v & 0xff) << 8);
}

uint32_t byteswap(uint32_t v) {
    return v >> 24 |
        ((v & 0x00ff0000) >> 8) |
        ((v & 0x0000ff00) << 8) |
        ((v & 0x000000ff) <<24);
}

uint64_t byteswap(uint64_t v) {
    return static_cast<uint64_t>(byteswap(static_cast<uint32_t>(v))) << 32 |
        byteswap(static_cast<uint32_t>(v >> 32));
}
