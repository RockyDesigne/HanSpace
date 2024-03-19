//
// Created by HORIA on 14.03.2024.
//

#ifndef HANSPACE_ASTEROIDS_H
#define HANSPACE_ASTEROIDS_H

#include "Window.h"
#include "Buffers.h"
#include "Randomization.h"
#include <utility>
#include <random>

namespace Asteroids {

    using COORDS = std::pair<float,float>;

    constexpr int asteroidWidthFromCenter = 50;

    std::uniform_int_distribution asteroidXdist {0,
                                                 Window::width - asteroidWidthFromCenter * 2};

    struct Asteroid {

        COORDS bottomLeft;
        COORDS bottomRight;
        COORDS topLeft;
        COORDS topRight;

        void drawAsteroid(float XOffSet, float YOffSet) const {
            Buffers::push_vert(bottomLeft.first + XOffSet,bottomLeft.second - YOffSet, 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
            Buffers::push_vert(bottomRight.first + XOffSet,bottomRight.second - YOffSet, 1.0f,0.5f,0.75f, 1.0f, 0.0f);
            Buffers::push_vert(topLeft.first + XOffSet,topLeft.second - YOffSet, 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
            Buffers::push_vert(topRight.first + XOffSet,topRight.second - YOffSet,1.0f, 0.5f,0.75, 1.0f, 1.0f);
        }

    };

    /*
    COORDS bottomLeft = {0.0f,Window::height - asteroidWidthFromCenter};
    COORDS bottomRight = {asteroidWidthFromCenter * 2 , Window::height - asteroidWidthFromCenter};
    COORDS topLeft = {0.0f - asteroidWidthFromCenter , Window::height + asteroidWidthFromCenter};
    COORDS topRight = {asteroidWidthFromCenter * 2 , Window::height + asteroidWidthFromCenter};
    */

    constexpr int maxAsteroids = 5;

    Asteroids::Asteroid asteroids[maxAsteroids];

    void initAsteroids() {
        for (auto & asteroid : asteroids) {
            int randXOffset = Random::getRandNum(asteroidXdist);
            asteroid = {
                    .bottomLeft = {randXOffset,Window::height - asteroidWidthFromCenter},
                    .bottomRight = {randXOffset + asteroidWidthFromCenter * 2, Window::height - asteroidWidthFromCenter},
                    .topLeft = {randXOffset, Window::height + asteroidWidthFromCenter},
                    .topRight = {randXOffset + asteroidWidthFromCenter * 2 , Window::height + asteroidWidthFromCenter}
            };
        }
    }

}

#endif //HANSPACE_ASTEROIDS_H
