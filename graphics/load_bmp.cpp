#include <cstdint>
#include <cstdio>
#include <vector>

#include <GL/glew.h>

#include "external/stb_image.h"

#include "util.h"

GLuint loadTextureArray(const char *filename, int tilesX, int tilesY, int tileW, int tileH) {
    int width, height, nChannels;

    uint8_t *data = stbi_load(filename, &width, &height, &nChannels, 4);

    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, 16, 16, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    int channels = nChannels;

    std::vector<uint8_t> tile(tileW * tileH * channels);
    int tileSizeX = tileW * channels;
    int rowLen = tilesX * tileSizeX;

    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {
            uint8_t *ptr = data + y * rowLen * tileH + x * tileSizeX;
            for (int row = 0; row < tileH; row++) {
                std::copy(ptr + row * rowLen, ptr + row * rowLen + tileSizeX, tile.begin() + row * tileSizeX);
            }
            int i = y * tilesY + x;

            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, tileW, tileH, 1, GL_RGBA, GL_UNSIGNED_BYTE, tile.data());
        }
    }

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);

    return texture;
}

GLuint loadTexture(const char *filename) {
    int width, height, nChannels;

    uint8_t *data = stbi_load(filename, &width, &height, &nChannels, 4);

    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture;
}