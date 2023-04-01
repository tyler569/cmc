//
// Created by Tyler Philbrick on 3/31/23.
//
#include <vector>
#include "minecraft/varint.h"

void writeVarint(int value, std::vector<uint8_t> &v) {
    auto uvalue = static_cast<uint32_t>(value);
    while (true) {
        if ((uvalue & ~SEGMENT_BITS) == 0) {
            v.push_back(uvalue);
            return;
        }
        v.push_back((uvalue & SEGMENT_BITS) | CONTINUE_BIT);
        uvalue >>= 7;
    }
}

int readVarint(std::vector<uint8_t>::const_iterator &it) {
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

void writeVarlong(long value, std::vector<uint8_t> &v) {
    auto uvalue = static_cast<uint64_t>(value);
    while (true) {
        if ((uvalue & ~(long)SEGMENT_BITS) == 0) {
            v.push_back(uvalue);
            return;
        }
        v.push_back((uvalue & SEGMENT_BITS) | CONTINUE_BIT);
        uvalue >>= 7;
    }
}

long readVarlong(std::vector<uint8_t>::const_iterator &it) {
    long value = 0;
    int position = 0;
    uint8_t currentByte;

    while (true) {
        currentByte = *it++;
        value |= (currentByte & SEGMENT_BITS) << position;

        if ((currentByte & CONTINUE_BIT) == 0) break;

        position += 7;

        if (position >= 64) throw InvalidVarint{};
    }

    return value;
}

void testVarint() {
    const auto testValue = [](int value) {
        std::vector<uint8_t> buffer;
        writeVarint(value, buffer);

        printf("% -12i: ", value);
        for (const auto &byte: buffer) {
            printf("%i ", byte);
        }
        printf("   ");

        auto it = buffer.cbegin();
        printf("%i %i\n", readVarint(it), it == buffer.end());
    };

    testValue(0);
    testValue(1);
    testValue(2);
    testValue(127);
    testValue(128);
    testValue(255);
    testValue(25565);
    testValue(2097151);
    testValue(2147483647);
    testValue(-1);
    testValue(-2147483648);
}
