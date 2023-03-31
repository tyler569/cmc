#ifndef GL_MINECRAFT_VARINT_H
#define GL_MINECRAFT_VARINT_H

#include <cstdlib>
#include <iterator>

#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80

void writeVarint(int value, std::vector<uint8_t> &v);

class InvalidVarint : std::exception {};

template <std::input_iterator Iterator>
int readVarint(Iterator &it) {
    int value = 0;
    int position = 0;
    uint8_t currentByte;

    while (true) {
        currentByte = *it++;
        value |= (currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BIT) == 0) break;

        position += 7;

        if (position >= 32) throw InvalidVarint{};
    }

    return value;
}

void testVarint();

#endif