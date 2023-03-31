#include <iostream>

#include "graphics/mesh.h"

std::ostream& operator <<(std::ostream& os, const Vertex& v) {
    os << "{"
       << v.position[0] << ", "
       << v.position[1] << ", "
       << v.position[2] << ", "
       << v.uv[0] << ", "
       << v.uv[1] << ", "
       << v.textureIndex << "}";
    return os;
}
