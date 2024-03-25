//
// Created by HORIA on 14.03.2024.
//

#ifndef HANSPACE_TEXTURES_H
#define HANSPACE_TEXTURES_H

#include "glad.h"
#include "HanShipTexture.h"
#include "HanBackgroundTexture.h"
#include "HanAsteroidTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Textures {
    int width, height, nrChannels;
    GLuint shipTexId, backgroundTexId, asteroidTexId;

    void createShipTexture() {
        unsigned char *data = stbi_load_from_memory(HanShipTexture, (int)HanShipTextureLen, &width, &height, &nrChannels, 0);
        glGenTextures(1, &shipTexId);
        glBindTexture(GL_TEXTURE_2D, shipTexId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        /*
         * A mipmap is a sequence of textures, each a power of two smaller than the previous
         * the more you zoom out, and the picture gets smaller, the lower level mipmaps are used
         */
        glGenerateMipmap(GL_TEXTURE_2D); // this generates all the levels of the mipmap for the bound texture

        stbi_image_free(data);
    }

    void createBackgroundTexture() {
        unsigned char *data = stbi_load_from_memory(HanBackground_png, (int) HanBackground_png_len,&width, &height, &nrChannels, 0);

        glGenTextures(1, &backgroundTexId);
        glBindTexture(GL_TEXTURE_2D, backgroundTexId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D); // this generates all the levels of the mipmap for the bound texture

        stbi_image_free(data);
    }

    void createAsteroidTexture() {
        unsigned char *data = stbi_load_from_memory(HanAsteroid_png, (int) HanBackground_png_len, &width, &height, &nrChannels, 0);

        glGenTextures(1, &asteroidTexId);
        glBindTexture(GL_TEXTURE_2D, asteroidTexId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D); // this generates all the levels of the mipmap for the bound texture

        stbi_image_free(data);
    }

    void bindShipTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shipTexId);
    }

    void bindBackgroundTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTexId);
    }

    void bindAsteroidTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, asteroidTexId);
    }

}

#endif //HANSPACE_TEXTURES_H
