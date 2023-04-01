#include <fmt/core.h>

#include "dump.h"

void dump(std::vector<uint8_t> array) {
    for (size_t i = 0; i < array.size(); i++) {
        fmt::print("{:02x} ", array[i]);
        if (i % 16 == 15) {
            fmt::print("\n");
        }
    }
    fmt::print("\n");
}
