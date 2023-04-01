#ifndef GL_PACKET_H
#define GL_PACKET_H

#include <concepts>
#include <bit>
#include <string>
#include <vector>

#include <fmt/core.h>

#include "minecraft/varint.h"
#include "util/byteswap.h"

struct packetSerializer {
    std::vector<uint8_t> &data;

    void writeVarint(int i) {
        ::writeVarint(i, data);
    }

    void writeVarlong(long i) {
        ::writeVarlong(i, data);
    }

    template <typename T>
    void write(T value) {
        if constexpr (sizeof(T) == 1) {
            data.push_back(std::bit_cast<uint8_t>(value));
        } else if constexpr (sizeof(T) == 2) {
            auto bs = byteswap(std::bit_cast<uint16_t>(value));
            write(reinterpret_cast<uint8_t *>(&bs), 2);
        } else if constexpr (sizeof(T) == 4) {
            auto bs = byteswap(std::bit_cast<uint32_t>(value));
            write(reinterpret_cast<uint8_t *>(&bs), 4);
        } else if constexpr (sizeof(T) == 8) {
            auto bs = byteswap(std::bit_cast<uint64_t>(value));
            write(reinterpret_cast<uint8_t *>(&bs), 8);
        }
    }

    void write(const uint8_t *ptr, size_t len) {
        data.insert(data.end(), ptr, ptr + len);
    }

    void write(const uint8_t *begin, const uint8_t *end) {
        data.insert(data.end(), begin, end);
    }

    void write(std::string::const_iterator begin, std::string::const_iterator end) {
        data.insert(data.end(), begin, end);
    }

    void write(std::string_view::const_iterator begin, std::string_view::const_iterator end) {
        data.insert(data.end(), begin, end);
    }

    void write(std::vector<uint8_t> bytes) {
        data.insert(data.end(), bytes.begin(), bytes.end());
    }

    void write(const std::string &s) {
        writeVarint(s.length());
        write(s.begin(), s.end());
    }

    void write(std::string_view s) {
        writeVarint(s.length());
        write(s.begin(), s.end());
    }
};

struct packetDeserializer {
    std::vector<uint8_t> &packetData;
    std::vector<uint8_t>::const_iterator it = packetData.begin();
    std::vector<uint8_t>::const_iterator packetEnd = packetData.end();

    int readVarint() {
        return ::readVarint(it);
    }

    long readVarlong() {
        return ::readVarlong(it);
    }

    std::vector<uint8_t>::const_iterator readEnd(size_t desiredSize) {
        size_t available = packetEnd - it;
        size_t canRead = std::min(desiredSize, available);
        if (canRead != desiredSize) {
            fmt::print(stderr, "WARN: tried to read {} from packet containing only {}", desiredSize, available);
        }
        return it + canRead;
    }

    void readBytes(size_t count, uint8_t *destination) {
        auto end = readEnd(count);
        std::copy(it, end, destination);
        it = end;
    }

    template <typename T>
    T read() {
        T value;
        readBytes(sizeof(T), reinterpret_cast<uint8_t *>(&value));
        if constexpr (sizeof(value) == 1) {
            return value;
        } else if constexpr (sizeof(value) == 2) {
            return std::bit_cast<T>(byteswap(std::bit_cast<uint16_t>(value)));
        } else if constexpr (sizeof(value) == 4) {
            return std::bit_cast<T>(byteswap(std::bit_cast<uint32_t>(value)));
        } else if constexpr (sizeof(value) == 8) {
            return std::bit_cast<T>(byteswap(std::bit_cast<uint64_t>(value)));
        }
    }

    template <>
    std::string read() {
        auto size = readVarint();
        auto end = readEnd(size);
        std::string str(it, end);
        it = end;
        return str;
    }

    std::vector<uint8_t> readBytes(size_t size) {
        std::vector<uint8_t> data(size);
        auto end = readEnd(size);
        std::copy(it, end, data.begin());
        it = end;
        return data;
    }
};

void testPacket();

#endif