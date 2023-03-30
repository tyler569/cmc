#include <string>

struct BlockInfo {
    int id;
    std::string_view name;
    bool transparent;
    int uv[2] = {0, 0};
    int topUV[2] = {0, 0};
    int sideUV[2] = {0, 0};
    int bottomUV[2] = {0, 0};
};

constexpr BlockInfo blocks[] = {
    {0, "Air", true},
    {1, "Stone", false, {1, 1}}
};