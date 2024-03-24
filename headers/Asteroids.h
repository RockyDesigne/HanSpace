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
        bool deleted = false;

        void drawAsteroid() const {
            Buffers::push_vert(bottomLeft.first ,bottomLeft.second , 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
            Buffers::push_vert(bottomRight.first ,bottomRight.second , 1.0f,0.5f,0.75f, 1.0f, 0.0f);
            Buffers::push_vert(topLeft.first ,topLeft.second , 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
            Buffers::push_vert(topRight.first ,topRight.second ,1.0f, 0.5f,0.75, 1.0f, 1.0f);
        }

    };

    /*
    COORDS bottomLeft = {0.0f,Window::height - asteroidWidthFromCenter};
    COORDS bottomRight = {asteroidWidthFromCenter * 2 , Window::height - asteroidWidthFromCenter};
    COORDS topLeft = {0.0f - asteroidWidthFromCenter , Window::height + asteroidWidthFromCenter};
    COORDS topRight = {asteroidWidthFromCenter * 2 , Window::height + asteroidWidthFromCenter};
    */

    constexpr int maxAsteroids = 1000;
    int asteroidsSize = 0;
    Asteroid asteroids[maxAsteroids];
    void pushAsteroid(float xOffset, float yOffset);
    void initAsteroids(int nr) {
        for (int i = 0; i < nr; ++i) {
            pushAsteroid((float) Random::getRandNum(asteroidXdist), 0.0f);
        }
    }

    void pushAsteroid(float xOffset, float yOffset) {
        assert(asteroidsSize < maxAsteroids && "Too many asteroids!");
        asteroids[asteroidsSize].bottomLeft = {xOffset,(float)Window::height - asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].bottomRight = {xOffset + asteroidWidthFromCenter * 2, (float)Window::height - asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].topLeft = {xOffset, (float) Window::height + asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].topRight = {xOffset + asteroidWidthFromCenter * 2 , (float) Window::height + asteroidWidthFromCenter + yOffset};
        ++asteroidsSize;
    }

}

#endif //HANSPACE_ASTEROIDS_H
