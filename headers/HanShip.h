//
// Created by HORIA on 12.03.2024.
//

#ifndef HANSPACE_HANSHIP_H
#define HANSPACE_HANSHIP_H

#include "Window.h"
#include "Buffers.h"
#include "Asteroids.h"
#include "Global_Variables.h"
#include "ShipBoomTexture.h"
#include "HanShipTexture.h"
#include "Textures.h"
#include <utility>

namespace HanShip {
    TextureData shipTexture {
        .data = HanShipTexture,
        .dataLen = HanShipTextureLen,
        .id = 0
    };

    TextureData shipBoomTexture {
            .data = ShipBoomPng,
            .dataLen = (int)ShipBoomPngLen,
            .id = 0
    };

    using COORDS = std::pair<float,float>;
    int shipWidthFromCenter = 50;
    COORDS bottomLeft = {Window::width/2-shipWidthFromCenter, Window::height/2-shipWidthFromCenter};
    COORDS bottomRight = {Window::width/2+shipWidthFromCenter, Window::height/2-shipWidthFromCenter};
    COORDS topLeft = {Window::width/2-shipWidthFromCenter, Window::height/2+shipWidthFromCenter};
    COORDS topRight = {Window::width/2+shipWidthFromCenter, Window::height/2+shipWidthFromCenter};

    int spriteFrameDuration = 2;
    int fireCoolDown = 10;
    int timeSinceLastShotFired = 0;
    int spriteCurrFrame = 0;
    int frameCounter = 0;
    static int maxFrames = 64;
    //bool animationActive = false;
    bool deleted = false;

    void resetShipPos() {
        bottomLeft = {Window::width/2-shipWidthFromCenter, Window::height/2-shipWidthFromCenter};
        bottomRight = {Window::width/2+shipWidthFromCenter, Window::height/2-shipWidthFromCenter};
        topLeft = {Window::width/2-shipWidthFromCenter, Window::height/2+shipWidthFromCenter};
        topRight = {Window::width/2+shipWidthFromCenter, Window::height/2+shipWidthFromCenter};
    }

    void moveShip(float xOffset, float yOffset) {
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

    bool checkCollisionWithShip(const Asteroids::Asteroid& asteroid) {
        // Define the ship's bounding box
        float shipLeft = HanShip::topLeft.first;
        float shipRight = HanShip::topRight.first;
        float shipTop = HanShip::topLeft.second;
        float shipBottom = HanShip::bottomLeft.second;

        // Define the asteroid's bounding box
        float asteroidLeft = asteroid.bottomLeft.first;
        float asteroidRight = asteroid.bottomRight.first;
        float asteroidTop = asteroid.topLeft.second;
        float asteroidBottom = asteroid.bottomLeft.second;

        // Check for collision using AABB method
        bool collisionX = shipLeft < asteroidRight && shipRight > asteroidLeft;
        bool collisionY = shipTop > asteroidBottom && shipBottom < asteroidTop;

        return collisionX && collisionY;
    }

    int projectilesSize = 0;

    struct Projectile {
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

        void drawProjectile() const {
            Buffers::push_vert(bottomLeft.first,bottomLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
            Buffers::push_vert(bottomRight.first,bottomRight.second, 1.0f,0.5f,0.75f, 1.0f, 0.0f);
            Buffers::push_vert(topLeft.first,topLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
            Buffers::push_vert(topRight.first,topRight.second,1.0f, 0.5f,0.75, 1.0f, 1.0f);
        }

    };

    bool checkCollisionWithProjectile(const Asteroids::Asteroid& asteroid, const Projectile& projectile) {
        // Define the ship's bounding box
        float projectileLeft = projectile.topLeft.first;
        float projectileRight = projectile.topRight.first;
        float projectileTop = projectile.topLeft.second;
        float projectileBottom = projectile.bottomLeft.second;

        // Define the asteroid's bounding box
        float asteroidLeft = asteroid.bottomLeft.first;
        float asteroidRight = asteroid.bottomRight.first;
        float asteroidTop = asteroid.topLeft.second;
        float asteroidBottom = asteroid.bottomLeft.second;

        // Check for collision using AABB method
        bool collisionX = projectileLeft < asteroidRight && projectileRight > asteroidLeft;
        bool collisionY = projectileTop > asteroidBottom && projectileBottom < asteroidTop;

        return collisionX && collisionY;
    }

    void drawShipBoom() {
        float width = 1.0f / 8.0f;

        auto tex_x = (float) (HanShip::spriteCurrFrame % 8);
        auto tex_y = (float) (HanShip::spriteCurrFrame / 8);

        Buffers::push_vert(bottomLeft.first, bottomLeft.second, 1.0f, 0.5f, 0.75f, width * tex_x, width * tex_y);
        Buffers::push_vert(bottomRight.first, bottomRight.second, 1.0f, 0.5f, 0.75f, width * (tex_x + 1), width * tex_y);
        Buffers::push_vert(topLeft.first, topLeft.second, 1.0f, 0.5f, 0.75f, width * tex_x, width * (tex_y + 1));
        Buffers::push_vert(topRight.first, topRight.second, 1.0f, 0.5f, 0.75f, width * (tex_x + 1), width * (tex_y + 1));
    }

    void drawShip() {
        Buffers::push_vert(bottomLeft.first,bottomLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
        Buffers::push_vert(bottomRight.first,bottomRight.second, 1.0f,0.5f,0.75f, 1.0f, 0.0f);
        Buffers::push_vert(topLeft.first,topLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
        Buffers::push_vert(topRight.first,topRight.second,1.0f, 0.5f,0.75, 1.0f, 1.0f);

    }

    constexpr int maxProjectiles = 1000;
    Projectile projectiles[maxProjectiles];

    void updateProjectiles() {
        for (int i = 0; i < projectilesSize; ++i) {
            if (!projectiles[i].deleted)
                projectiles[i].updatePos(0.0f, 10.0f);
        }
    }

    void clearProjectiles() {
        for (auto& projectile : projectiles) {
            projectile.deleted = true;
        }
        projectilesSize = 0;
    }

    void pewPew() {
        if (projectilesSize >= maxProjectiles) {
            clearProjectiles();
        }
        projectiles[projectilesSize].bottomLeft = {HanShip::topLeft.first + 15, HanShip::topLeft.second};
        projectiles[projectilesSize].bottomRight = {HanShip::topLeft.first + 20, HanShip::topLeft.second};
        projectiles[projectilesSize].topLeft = {HanShip::topLeft.first + 15, HanShip::topLeft.second + 25};
        projectiles[projectilesSize].topRight = {HanShip::topLeft.first + 20, HanShip::topLeft.second + 25};
        projectiles[projectilesSize].deleted = false;
        ++projectilesSize;
        projectiles[projectilesSize].bottomLeft = {HanShip::topRight.first - 15, HanShip::topRight.second};
        projectiles[projectilesSize].bottomRight = {HanShip::topRight.first - 20, HanShip::topRight.second};
        projectiles[projectilesSize].topLeft = {HanShip::topRight.first - 15, HanShip::topRight.second + 25};
        projectiles[projectilesSize].topRight = {HanShip::topRight.first - 20, HanShip::topRight.second + 25};
        projectiles[projectilesSize].deleted = false;
        ++projectilesSize;
    }

    void updateShip() {

        if (!GAME_OVER) {

            if (timeSinceLastShotFired > 0) {
                --timeSinceLastShotFired;
            }

            auto actionUp = glfwGetKey(Window::winPtr, GLFW_KEY_UP);
            auto actionDown = glfwGetKey(Window::winPtr, GLFW_KEY_DOWN);
            auto actionLeft = glfwGetKey(Window::winPtr, GLFW_KEY_LEFT);
            auto actionRight = glfwGetKey(Window::winPtr, GLFW_KEY_RIGHT);
            auto actionSpace = glfwGetKey(Window::winPtr, GLFW_KEY_SPACE);

            float impulse = 5.0f;

            if (actionUp == GLFW_PRESS) {
                if (topRight.second + impulse < (float) Window::height || topLeft.second + impulse < Window::height)
                    moveShip(0.f, impulse);
            }

            if (actionDown == GLFW_PRESS) {
                if (bottomRight.second - impulse > 0 || bottomLeft.second - impulse > 0)
                    moveShip(0.f, -impulse);
            }

            if (actionLeft == GLFW_PRESS) {
                if (bottomLeft.first - impulse > 0 || topLeft.first - impulse > 0)
                    moveShip(-impulse, 0.f);
            }

            if (actionRight == GLFW_PRESS) {
                if (topRight.first + impulse < (float) Window::width || bottomRight.first + impulse < Window::width)
                    moveShip(impulse, 0.f);
            }

            if (actionSpace == GLFW_PRESS) {
                if (timeSinceLastShotFired <= 0) {
                    pewPew();
                    timeSinceLastShotFired = fireCoolDown;
                }
            }

        }

        if (HanShip::deleted && spriteCurrFrame < HanShip::maxFrames) {
            ++HanShip::frameCounter;
            if (HanShip::frameCounter > HanShip::spriteFrameDuration) {
                HanShip::frameCounter = 0;
                ++HanShip::spriteCurrFrame;
            }
        }
    }

}

#endif //HANSPACE_HANSHIP_H
