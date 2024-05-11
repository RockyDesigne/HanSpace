//
// Created by HORIA on 14.03.2024.
//

#ifndef HANSPACE_BACKGROUND_H
#define HANSPACE_BACKGROUND_H

#include "Window.h"
#include "Buffers.h"
#include "Textures.h"
#include "HanBackgroundTexture.h"
#include <utility>

namespace BackGround {

    TextureData backgroundTexture {
            .textureData = HanBackground_png,
            .dataLen = (int)HanBackground_png_len,
            .textureId = 0,
            .internalFormat = RGB,
            .format = RGB
    };

    using COORDS = std::pair<float,float>;

    COORDS bottomLeft = {0.f,0.f};
    COORDS bottomRight = {Window::width, 0.f};
    COORDS topLeft = {0.f, Window::height};
    COORDS topRight = {Window::width, Window::height};

    void calcBackground() {
        bottomLeft = {0.f,0.f};
        bottomRight = {Window::width, 0.f};
        topLeft = {0.f, Window::height};
        topRight = {Window::width, Window::height};
    }

    void drawBackground() {
        Buffers::push_vert(bottomLeft.first,bottomLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
        Buffers::push_vert(bottomRight.first,bottomRight.second, 1.0f,0.5f,0.75f, 1.0f, 0.0f);
        Buffers::push_vert(topLeft.first,topLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
        Buffers::push_vert(topRight.first,topRight.second,1.0f, 0.5f,0.75, 1.0f, 1.0f);
    }

}


#endif //HANSPACE_BACKGROUND_H
