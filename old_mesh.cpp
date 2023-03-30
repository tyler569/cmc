/*
std::vector<GLfloat> generateMesh() {
    constexpr GLfloat duv = 1. / 16.;

    auto emitFront = [](std::vector<GLfloat> &Mesh, GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        Mesh.insert(Mesh.end(), {
            x, y, z, u+duv, v+duv,
            x, y+1, z, u+duv, v,
            x+1, y, z, u, v+duv,
            x, y+1, z, u+duv, v,
            x+1, y+1, z, u, v,
            x+1, y, z, u, v+duv
        });
    };

    auto emitBack = [](std::vector<GLfloat> &Mesh, GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        Mesh.insert(Mesh.end(), {
            x+1, y, z+1, u+duv, v+duv,
            x+1, y+1, z+1, u+duv, v,
            x, y, z+1, u, v+duv,
            x+1, y+1, z+1, u+duv, v,
            x, y+1, z+1, u, v,
            x, y, z+1, u, v+duv
        });
    };

    auto emitRight = [](std::vector<GLfloat> &Mesh, GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        Mesh.insert(Mesh.end(), {
            x+1, y, z, u+duv, v+duv,
            x+1, y+1, z, u+duv, v,
            x+1, y, z+1, u, v+duv,
            x+1, y+1, z, u+duv, v,
            x+1, y+1, z+1, u, v,
            x+1, y, z+1, u, v+duv
        });
    };

    auto emitLeft = [](std::vector<GLfloat> &Mesh, GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        Mesh.insert(Mesh.end(), {
            x, y, z+1, u+duv, v+duv,
            x, y+1, z+1, u+duv, v,
            x, y, z, u, v+duv,
            x, y+1, z+1, u+duv, v,
            x, y+1, z, u, v,
            x, y, z, u, v+duv
        });
    };

    auto emitTop = [](std::vector<GLfloat> &Mesh, GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        Mesh.insert(Mesh.end(), {
            x, y+1, z, u+duv, v+duv,
            x, y+1, z+1, u+duv, v,
            x+1, y+1, z, u, v+duv,
            x, y+1, z+1, u+duv, v,
            x+1, y+1, z+1, u, v,
            x+1, y+1, z, u, v+duv
        });
    };

    auto emitBottom = [](std::vector<GLfloat> &Mesh, GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v) {
        Mesh.insert(Mesh.end(), {
            x, y, z+1, u+duv, v+duv,
            x, y, z, u+duv, v,
            x+1, y, z+1, u, v+duv,
            x+1, y, z+1, u, v+duv,
            x, y, z, u+duv, v,
            x+1, y, z, u, v
        });
    };

    std::vector<GLfloat> Mesh = {};

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            emitFront(Mesh, x * 2, y * 2, 0, x / 16., y / 16.);
            emitBack(Mesh, x * 2, y * 2, 0, x / 16., y / 16.);
            emitTop(Mesh, x * 2, y * 2, 0, x / 16., y / 16.);
            emitBottom(Mesh, x * 2, y * 2, 0, x / 16., y / 16.);
            emitLeft(Mesh, x * 2, y * 2, 0, x / 16., y / 16.);
            emitRight(Mesh, x * 2, y * 2, 0, x / 16., y / 16.);
        }
    }

    // fmt::print("Mesh: ");
    // for (auto v : Mesh) {
    //     fmt::print("{} ", v);
    // }
    // fmt::print("\n");

    return Mesh;
}
*/