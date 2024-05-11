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
    const unsigned char* textureData = nullptr;
    int dataLen = 0;
    GLuint textureId = 0;
    TextureFormats internalFormat = DEFAULT;
    TextureFormats format = DEFAULT;
};

namespace Textures {

    void createTexture(const unsigned char* textureData, int textureDataLen, GLuint &textureId, TextureFormats internalFormat = DEFAULT, TextureFormats format = DEFAULT) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load_from_memory(textureData, textureDataLen, &width, &height, &nrChannels, 0);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D); // this generates all the levels of the mipmap for the bound texture

        stbi_image_free(data);
    }

    void bindTexture(GLuint textureId) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

}

#endif //HANSPACE_TEXTURES_H
