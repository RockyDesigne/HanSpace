//
// Created by HORIA on 14.03.2024.
//

#ifndef HANSPACE_ASTEROIDS_H
#define HANSPACE_ASTEROIDS_H

#include "Window.h"
#include "Buffers.h"
#include <utility>

namespace Asteroids {
    using COORDS = std::pair<float,float>;

    constexpr int asteroidWidthFromCenter = 50;
    constexpr int offsetY = 500;

    constexpr int offsetX = 500;

    COORDS bottomLeft = {Window::width/2 - asteroidWidthFromCenter + offsetX,Window::height/2 - asteroidWidthFromCenter + offsetY};
    COORDS bottomRight = {Window::width/2 + asteroidWidthFromCenter + offsetX, Window::height/2 - asteroidWidthFromCenter + offsetY};
    COORDS topLeft = {Window::width/2 - asteroidWidthFromCenter + offsetX, Window::height/2 + asteroidWidthFromCenter + offsetY};
    COORDS topRight = {Window::width/2 + asteroidWidthFromCenter + offsetX, Window::height/2 + asteroidWidthFromCenter + offsetY};

    void drawAsteroid(float XOffSet, float YOffSet) {
        Buffers::push_vert(bottomLeft.first + XOffSet,bottomLeft.second - YOffSet, 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
        Buffers::push_vert(bottomRight.first + XOffSet,bottomRight.second - YOffSet, 1.0f,0.5f,0.75f, 1.0f, 0.0f);
        Buffers::push_vert(topLeft.first + XOffSet,topLeft.second - YOffSet, 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
        Buffers::push_vert(topRight.first + XOffSet,topRight.second - YOffSet,1.0f, 0.5f,0.75, 1.0f, 1.0f);
    }

}

#endif //HANSPACE_ASTEROIDS_H
