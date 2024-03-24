#include "glad.h"
#include <glfw3.h>
#include <iostream>
#include "Window.h"
#include "HanShip.h"
#include "Shaders.h"
#include "Global_Variables.h"
#include "Buffers.h"
#include "Asteroids.h"
#include "Background.h"
#include "Textures.h"

void handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
    using namespace HanShip;
    (void) scancode;
    (void) mods;
    float impulse = 100.f;
    switch (key) {
        case GLFW_KEY_ESCAPE: {
            if (action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
            break;
        }
        case GLFW_KEY_UP: {
            if (action == GLFW_PRESS) {
                bottomLeft = topLeft;
                bottomRight = topRight;
                topLeft = {topLeft.first, topLeft.second + impulse};
                topRight = {topRight.first, topRight.second + impulse};
            }
            break;
        }
        case GLFW_KEY_DOWN: {
            if (action == GLFW_PRESS) {
                topLeft = bottomLeft;
                topRight = bottomRight;
                bottomLeft = {bottomLeft.first, bottomLeft.second - impulse};
                bottomRight = {bottomRight.first, bottomRight.second - impulse};
            }
            break;
        }
        case GLFW_KEY_LEFT: {
            if (action == GLFW_PRESS) {
                bottomRight = bottomLeft;
                topRight = topLeft;
                bottomLeft = {bottomLeft.first - impulse, bottomLeft.second};
                topLeft = {topLeft.first - impulse, topLeft.second};
            }
            break;
        }
        case GLFW_KEY_RIGHT: {
            if (action == GLFW_PRESS) {
                bottomLeft = bottomRight;
                topLeft = topRight;
                bottomRight = {bottomRight.first + impulse, bottomRight.second};
                topRight = {topRight.first + impulse, topRight.second};
            }
            break;
        }

        case GLFW_KEY_SPACE: {
            if (action == GLFW_PRESS) {
                HanShip::pewPew();
                //printf("pewPew() called!\n");
            }
            break;
        }

        default: {
            break;
        }
    }
}

void handleWindowResize(GLFWwindow* window, int width, int height) {
    //make x and y center
    (void) window;
    //int x = (width-Window::width) / 2;
    //int y = (height-Window::height) / 2;

    Window::width = width;
    Window::height = height;

    glViewport(width / 2 - Window::width  / 2,
               height / 2 - Window::height / 2, width,height);
    /*
     * When you try and set a uniform and it cant find it results in GL_INVALID_OPERATION error generated. Invalid component count error
     */
    glUseProgram(SHIP_PROGRAM);
    glUniform2f(SHIP_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    glUseProgram(BACKGROUND_PROGRAM);
    glUniform2f(BACKGROUND_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    glUseProgram(ASTEROID_PROGRAM);
    glUniform2f(ASTEROID_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    glUseProgram(PROJECTILE_PROGRAM);
    glUniform2f(PROJECTILE_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    BackGround::calcBackground();
}

void handleMessageCallback(GLenum source,
                           GLenum type,
                           GLuint id,
                           GLenum severity,
                           GLsizei length,
                           const GLchar* msg,
                           const void* userParam) {
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, msg = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, msg);
}

void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    using namespace Window;
    winPtr = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(winPtr);

    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

int main() {

    initWindow();

    //set window callbacks
    using namespace Window;
    glfwSetKeyCallback(winPtr, handleKeyPress);
    glfwSetFramebufferSizeCallback(winPtr, handleWindowResize);

    //enable gl debug
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(handleMessageCallback, nullptr);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //compile shaders
    GLuint vShaderId = Shaders::compile_shader(Shaders::vertexSource, GL_VERTEX_SHADER);
    GLuint shipShaderId = Shaders::compile_shader(Shaders::shipShader,GL_FRAGMENT_SHADER);
    GLuint asteroidShaderId = Shaders::compile_shader(Shaders::asteroidShader,GL_FRAGMENT_SHADER);
    GLuint backgroundShaderId = Shaders::compile_shader(Shaders::backgroundShader,GL_FRAGMENT_SHADER);
    GLuint projectileShaderId = Shaders::compile_shader(Shaders::projectileShader,GL_FRAGMENT_SHADER);

    //link ship
    Shaders::link_program(vShaderId, shipShaderId, SHIP_PROGRAM);
    glUseProgram(SHIP_PROGRAM);

    SHIP_PROGRAM_RESOLUTION_UNIFORM = glGetUniformLocation(SHIP_PROGRAM, "resolution");
    glUniform2f(SHIP_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    Textures::createShipTexture();

    //link bkg
    Shaders::link_program(vShaderId, backgroundShaderId, BACKGROUND_PROGRAM);
    glUseProgram(BACKGROUND_PROGRAM);

    //get resolution for bkg
    BACKGROUND_PROGRAM_RESOLUTION_UNIFORM = glGetUniformLocation(BACKGROUND_PROGRAM, "resolution");
    glUniform2f(BACKGROUND_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));
    BACKGROUND_PROGRAM_YOFFSET = glGetUniformLocation(BACKGROUND_PROGRAM, "yOffset");

    //glUniform1i(glGetUniformLocation(BACKGROUND_PROGRAM, "backgroundTexture"), 0);

    Textures::createBackgroundTexture();

    //link asteroid
    Shaders::link_program(vShaderId, asteroidShaderId, ASTEROID_PROGRAM);
    glUseProgram(ASTEROID_PROGRAM);

    //get resolution for asteroid
    ASTEROID_PROGRAM_RESOLUTION_UNIFORM = glGetUniformLocation(ASTEROID_PROGRAM, "resolution");
    glUniform2f(ASTEROID_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    Textures::createAsteroidTexture();

    //link projectile
    Shaders::link_program(vShaderId, projectileShaderId, PROJECTILE_PROGRAM);
    glUseProgram(PROJECTILE_PROGRAM);
    PROJECTILE_PROGRAM_RESOLUTION_UNIFORM = glGetUniformLocation(PROJECTILE_PROGRAM, "resolution");
    glUniform2f(PROJECTILE_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    //prepare buffers
    Buffers::init_buffers();
    Buffers::sync_buffers();

    glClearColor(.0f, .0f,.0f,1.f);
    glViewport(width / 2 - Window::width  / 2,
               height / 2 - Window::height / 2, width,height);

    using namespace HanShip;

    float yOffset = 0.0f;
    float speed = 0.25f;

    float asteroidSpeed = 10.f;
    float asteroidYOffset = 0.0f;

    Asteroids::initAsteroids();

    //constexpr int asteroidDist = 100;

    double lastTime = glfwGetTime();
    double timeStep = 1.0 / 60.0; // 60 updates per second
    double accumulator = 0.0;

    while (!glfwWindowShouldClose(winPtr)) {
        double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;
        lastTime = currentTime;

        accumulator += frameTime;

        while (accumulator >= timeStep) {
            // game state update
            yOffset += (float) (speed * timeStep);
            if (yOffset > 1.0f) {
                yOffset = 0.0f;
            }

            asteroidYOffset += asteroidSpeed * (float) timeStep;
            if (asteroidYOffset > (float) Window::height - Asteroids::asteroidWidthFromCenter) {
                asteroidYOffset = 0.0f;
            }

            HanShip::updateProjectiles();

            accumulator -= timeStep;
        }

        // Rendering
        glClear(GL_COLOR_BUFFER_BIT);
        Buffers::clear_buff();

        glUseProgram(BACKGROUND_PROGRAM);
        glUniform1f(BACKGROUND_PROGRAM_YOFFSET, yOffset);

        Textures::bindBackgroundTexture();
        BackGround::drawBackground();

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glUseProgram(SHIP_PROGRAM);
        Textures::bindShipTexture();
        HanShip::drawShip();

        glDrawArrays(GL_TRIANGLE_STRIP, (GLint)Buffers::verticesCount - 4, 4);

        glUseProgram(ASTEROID_PROGRAM);
        Textures::bindAsteroidTexture();

        for (auto & asteroid : Asteroids::asteroids) {
            asteroid.drawAsteroid(0.0f, 0.0f);
            if (!asteroid.deleted)
                glDrawArrays(GL_TRIANGLE_STRIP, (GLint)Buffers::verticesCount - 4, 4);
        }

        glUseProgram(PROJECTILE_PROGRAM);

        for (const auto& projectile : HanShip::projectiles) {
            if (!projectile.deleted) {
                projectile.drawProjectile();
                glDrawArrays(GL_TRIANGLE_STRIP, (GLint) Buffers::verticesCount - 4, 4);
            }
        }

        //collision checks

        for (auto & asteroid : Asteroids::asteroids) {
            if (HanShip::checkCollisionWithShip(asteroid)) {
                asteroid.deleted = true;
            }
        }

        for (auto& projectile : HanShip::projectiles) {
            for (auto & asteroid : Asteroids::asteroids) {
                if (checkCollisionWithProjectile(asteroid, projectile)) {
                    asteroid.deleted = true;
                    projectile.deleted = true;
                }
            }
        }

        Buffers::sync_buffers();

        glfwSwapBuffers(winPtr);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}