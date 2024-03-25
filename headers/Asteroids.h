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
#include <algorithm>

namespace Asteroids {

    using COORDS = std::pair<float,float>;

    constexpr int asteroidWidthFromCenter = 50;

    std::uniform_int_distribution asteroidXdist {0,
                                                 Window::maxWidth - asteroidWidthFromCenter * 2};

    struct Asteroid {

        COORDS bottomLeft;
        COORDS bottomRight;
        COORDS topLeft;
        COORDS topRight;
        bool deleted = false;

        void updatePos(float xOffset, float yOffset) {
            bottomLeft = {
                    bottomLeft.first + xOffset,
                    bottomLeft.second + yOffset
            };

            bottomRight = {
                    bottomRight.first + xOffset,
                    bottomRight.second + yOffset
            };

            topLeft = {
                    topLeft.first + xOffset,
                    topLeft.second + yOffset
            };

            topRight = {
                    topRight.first + xOffset,
                    topRight.second + yOffset
            };
        }

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
    void makeRandAsteroids(int nr) {
        for (int i = 0; i < nr; ++i) {
            pushAsteroid((float) std::clamp(Random::getRandNum(asteroidXdist), 0, Window::width - asteroidWidthFromCenter * 2),
                         0.0f);
        }
    }

    void updateAsteroids() {
        for (int i = 0; i < asteroidsSize; ++i) {
            if (!asteroids[i].deleted)
                asteroids[i].updatePos(0.0f, -2.0f);
        }
    }

    void pushAsteroid(float xOffset, float yOffset) {
        if (asteroidsSize >= maxAsteroids) {
            for (auto& asteroid : asteroids)
                asteroid.deleted = true;
            asteroidsSize = 0;
        }
        asteroids[asteroidsSize].bottomLeft = {xOffset,(float)Window::height - asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].bottomRight = {xOffset + asteroidWidthFromCenter * 2, (float)Window::height - asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].topLeft = {xOffset, (float) Window::height + asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].topRight = {xOffset + asteroidWidthFromCenter * 2 , (float) Window::height + asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].deleted = false;
        ++asteroidsSize;
    }

}

#endif //HANSPACE_ASTEROIDS_H
