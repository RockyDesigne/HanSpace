//
// Created by HORIA on 14.03.2024.
//

#ifndef HANSPACE_TEXTURES_H
#define HANSPACE_TEXTURES_H

#include "glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <algorithm>

enum TextureFormats {
    RGBA = GL_RGBA,
    RGB = GL_RGB,
    DEFAULT = GL_RGBA
};

struct TextureData {
    const char* filePath = nullptr;
    const unsigned char* data = nullptr;
    int dataLen = 0;
    GLuint id = 0;
    TextureFormats internalFormat = DEFAULT;
    TextureFormats format = DEFAULT;
};

namespace Textures {

    void createTexture(TextureData& texture) {
        int width, height, nrChannels;
        unsigned char *data;
        if (!texture.filePath) {
            data = stbi_load_from_memory(texture.data, texture.dataLen, &width, &height, &nrChannels, 0);
        } else if (texture.filePath) {
            stbi_load(texture.filePath, &width, &height, &nrChannels, 0);
        }

        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, texture.internalFormat, width, height, 0, texture.format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D); // this generates all the levels of the mipmap for the bound texture

        stbi_image_free(data);
    }

    void bindTexture(const TextureData& texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }

}

#endif //HANSPACE_TEXTURES_H
