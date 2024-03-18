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

    void resetCoords() {
        bottomLeft = {Window::width/2 - asteroidWidthFromCenter + offsetX,Window::height/2 - asteroidWidthFromCenter + offsetY};
        bottomRight = {Window::width/2 + asteroidWidthFromCenter + offsetX, Window::height/2 - asteroidWidthFromCenter + offsetY};
        topLeft = {Window::width/2 - asteroidWidthFromCenter + offsetX, Window::height/2 + asteroidWidthFromCenter + offsetY};
        topRight = {Window::width/2 + asteroidWidthFromCenter + offsetX, Window::height/2 + asteroidWidthFromCenter + offsetY};
    }

    void updateCalcCoords(float YOffSet) {
        if (bottomLeft.second - YOffSet <= 0.0f) {
            resetCoords();
        }
        topLeft = {topLeft.first, topLeft.second - YOffSet};
        topRight = {topRight.first, topRight.second - YOffSet};
        bottomLeft = {bottomLeft.first, bottomLeft.second - YOffSet};
        bottomRight = {bottomRight.first, bottomRight.second - YOffSet};
    }

    void drawAsteroid() {
        Buffers::push_vert(bottomLeft.first,bottomLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
        Buffers::push_vert(bottomRight.first,bottomRight.second, 1.0f,0.5f,0.75f, 1.0f, 0.0f);
        Buffers::push_vert(topLeft.first,topLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
        Buffers::push_vert(topRight.first,topRight.second,1.0f, 0.5f,0.75, 1.0f, 1.0f);
    }

}

#endif //HANSPACE_ASTEROIDS_H
