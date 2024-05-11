//
// Created by HORIA on 14.03.2024.
//

#ifndef HANSPACE_ASTEROIDS_H
#define HANSPACE_ASTEROIDS_H

#include "Textures.h"
#include "HanAsteroidTexture.h"
#include "BoomTexture.h"
#include "Window.h"
#include "Buffers.h"
#include "Randomization.h"
#include <utility>
#include <random>
#include <algorithm>

namespace Asteroids {

    TextureData asteroidTexture {
        .data = HanAsteroid_png,
        .dataLen = (int)HanAsteroid_png_len,
        .id = 0
    };

    TextureData asteroidBoomTexture {
            .data = boomPng,
            .dataLen = (int)boomPngLen,
            .id = 0
    };

    using COORDS = std::pair<float,float>;

    constexpr int asteroidWidthFromCenter = 50;

    struct Asteroid {

        COORDS bottomLeft;
        COORDS bottomRight;
        COORDS topLeft;
        COORDS topRight;
        static int spriteFrameDuration;
        int spriteCurrFrame = 0;
        int frameCounter = 0;
        static int maxFrames;
        //bool animationActive = false;
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

        void drawBoom() const {

            float width = 1.0f / 5.0f; // width of one frame
            Buffers::push_vert(bottomLeft.first, bottomLeft.second, 1.0f, 0.5f, 0.75f, width * (float) spriteCurrFrame, 0.0f);
            Buffers::push_vert(bottomRight.first, bottomRight.second, 1.0f, 0.5f, 0.75f, width * (float) spriteCurrFrame + width, 0.0f);
            Buffers::push_vert(topLeft.first, topLeft.second, 1.0f, 0.5f, 0.75f, width * (float) spriteCurrFrame, 1.0f);
            Buffers::push_vert(topRight.first, topRight.second, 1.0f, 0.5f, 0.75, width * (float) spriteCurrFrame + width, 1.0f);

        }

    };

    int Asteroid::maxFrames = 5;
    int Asteroid::spriteFrameDuration = 5;

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
            pushAsteroid((float) Random::getRandNum(std::uniform_int_distribution  {0,Window::width - asteroidWidthFromCenter * 2}),
                         0.0f);
        }
    }

    void updateAsteroids() {
        for (int i = 0; i < asteroidsSize; ++i) {
            if (!asteroids[i].deleted)
                asteroids[i].updatePos(0.0f, -2.0f);
            else if (asteroids[i].deleted && asteroids[i].spriteCurrFrame < Asteroid::maxFrames) {
                ++asteroids[i].frameCounter;
                if (asteroids[i].frameCounter > Asteroids::Asteroid::spriteFrameDuration) {
                    asteroids[i].frameCounter = 0;
                    ++asteroids[i].spriteCurrFrame;
                }
            }
        }
    }

    void clearAsteroids() {
        for (auto& asteroid : asteroids) {
            asteroid.deleted = true;
            asteroid.spriteCurrFrame = 5;
        }
        asteroidsSize = 0;
    }

    void pushAsteroid(float xOffset, float yOffset) {
        if (asteroidsSize >= maxAsteroids) {
            clearAsteroids();
        }
        asteroids[asteroidsSize].bottomLeft = {xOffset,(float)Window::height - asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].bottomRight = {xOffset + asteroidWidthFromCenter * 2, (float)Window::height - asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].topLeft = {xOffset, (float) Window::height + asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].topRight = {xOffset + asteroidWidthFromCenter * 2 , (float) Window::height + asteroidWidthFromCenter + yOffset};
        asteroids[asteroidsSize].deleted = false;
        asteroids[asteroidsSize].spriteCurrFrame = 0;
        asteroids[asteroidsSize].frameCounter = 0;
        ++asteroidsSize;
    }

}

#endif //HANSPACE_ASTEROIDS_H
