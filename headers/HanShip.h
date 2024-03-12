//
// Created by HORIA on 12.03.2024.
//

#ifndef HANSPACE_HANSHIP_H
#define HANSPACE_HANSHIP_H

#include "Window.h"
#include "Buffers.h"
#include <utility>

namespace HanShip {
    using COORDS = std::pair<float,float>;
    int shipWidthFromCenter = 50;
    COORDS bottomLeft = {Window::width/2-shipWidthFromCenter, Window::height/2-shipWidthFromCenter};
    COORDS bottomRight = {Window::width/2+shipWidthFromCenter, Window::height/2-shipWidthFromCenter};
    COORDS topLeft = {Window::width/2-shipWidthFromCenter, Window::height/2+shipWidthFromCenter};
    COORDS topRight = {Window::width/2+shipWidthFromCenter, Window::height/2+shipWidthFromCenter};

    void drawShip() {

        Buffers::push_vert(bottomLeft.first,bottomLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
        Buffers::push_vert(bottomRight.first,bottomRight.second, 1.0f,0.5f,0.75f, 1.0f, 0.0f);
        Buffers::push_vert(topLeft.first,topLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
        Buffers::push_vert(topRight.first,topRight.second,1.0f, 0.5f,0.75, 1.0f, 1.0f);

    }

}

#endif //HANSPACE_HANSHIP_H
