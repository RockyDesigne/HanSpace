#include "glad.h"
#include <glfw3.h>
#include <iostream>
#include "Font.h"
#include "Window.h"
#include "HanShip.h"
#include "Shaders.h"
#include "Global_Variables.h"
#include "Buffers.h"
#include "Asteroids.h"
#include "Background.h"
#include "Textures.h"

auto score = "Score: " + std::to_string(SCORE);
char* text = score.data();
int textLen = (int) strlen(text);

void updateText() {
    score = "Score: " + std::to_string(SCORE);
    text = score.data();
    textLen = (int) score.length();
}

void restart() {
    Asteroids::clearAsteroids();

    HanShip::clearProjectiles();

    HanShip::resetShipPos();
    HanShip::deleted = false;
    HanShip::spriteCurrFrame = 0;
    HanShip::frameCounter = 0;

    SCORE = 0;
    updateText();

    GAME_OVER = false;
}

void handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
    using namespace HanShip;
    (void) scancode;
    (void) mods;
        switch (key) {
            case GLFW_KEY_ESCAPE: {
                if (action == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, true);
                break;
            }
            case GLFW_KEY_ENTER: {
                if (GAME_OVER && action == GLFW_PRESS) {
                    restart();
                }
                break;
            }
            default:
                break;
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

    glUseProgram(TEXT_PROGRAM);
    glUniform2f(TEXT_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

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

/*
int main() {
    fread(ttf_buffer, 1, fileSize,
          fopen(R"(G:\projects\repos\HanGames\HanSpace\font\LTSuperiorMono-Regular.otf)", "rb"));

    stbtt_fontinfo font;

    stbtt_InitFont(&font, ttf_buffer, 0);

    stbtt_BakeFontBitmap(ttf_buffer, 0, 64.0f, pixels, 512, 512, 32, 96, chardata);

    if (!stbi_write_png("font.png", 512, 512, 1, pixels, 512)) {
        throw std::runtime_error("Error writing file png!");
    }

}
*/

void drawBuffer() {
    for (int i = 0; i < (int) Buffers::verticesCount / 4; ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }
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
    GLuint textShaderId = Shaders::compile_shader(Shaders::textShader, GL_FRAGMENT_SHADER);

    //link ship
    Shaders::link_program(vShaderId, shipShaderId, SHIP_PROGRAM);
    glUseProgram(SHIP_PROGRAM);

    SHIP_PROGRAM_RESOLUTION_UNIFORM = glGetUniformLocation(SHIP_PROGRAM, "resolution");
    glUniform2f(SHIP_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    Textures::createTexture(HanShip::shipTexture);

    Textures::createTexture(HanShip::shipBoomTexture);

    //link bkg
    Shaders::link_program(vShaderId, backgroundShaderId, BACKGROUND_PROGRAM);
    glUseProgram(BACKGROUND_PROGRAM);

    //get resolution for bkg
    BACKGROUND_PROGRAM_RESOLUTION_UNIFORM = glGetUniformLocation(BACKGROUND_PROGRAM, "resolution");
    glUniform2f(BACKGROUND_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));
    BACKGROUND_PROGRAM_YOFFSET = glGetUniformLocation(BACKGROUND_PROGRAM, "yOffset");

    //glUniform1i(glGetUniformLocation(BACKGROUND_PROGRAM, "backgroundTexture"), 0);

    Textures::createTexture(BackGround::backgroundTexture);

    //link asteroid
    Shaders::link_program(vShaderId, asteroidShaderId, ASTEROID_PROGRAM);
    glUseProgram(ASTEROID_PROGRAM);

    //get resolution for asteroid
    ASTEROID_PROGRAM_RESOLUTION_UNIFORM = glGetUniformLocation(ASTEROID_PROGRAM, "resolution");
    glUniform2f(ASTEROID_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    Textures::createTexture(Asteroids::asteroidTexture);

    Textures::createTexture(Asteroids::asteroidBoomTexture);

    glUniform1i(glGetUniformLocation(ASTEROID_PROGRAM, "asteroidTexture"), 0);
    glUniform1i(glGetUniformLocation(ASTEROID_PROGRAM, "boomTexture"), 1);

    ASTEROID_PROGRAM_DELETED_UNIFORM = glGetUniformLocation(ASTEROID_PROGRAM, "deleted");
    glUniform1i(ASTEROID_PROGRAM_DELETED_UNIFORM, GL_FALSE);

    //link projectile
    Shaders::link_program(vShaderId, projectileShaderId, PROJECTILE_PROGRAM);
    glUseProgram(PROJECTILE_PROGRAM);
    PROJECTILE_PROGRAM_RESOLUTION_UNIFORM = glGetUniformLocation(PROJECTILE_PROGRAM, "resolution");
    glUniform2f(PROJECTILE_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    //link text
    Shaders::link_program(vShaderId, textShaderId, TEXT_PROGRAM);
    glUseProgram(TEXT_PROGRAM);
    glUniform2f(TEXT_PROGRAM_RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));
    TEXT_PROGRAM_RESOLUTION_UNIFORM = glGetUniformLocation(TEXT_PROGRAM, "resolution");
    glUniform1i(glGetUniformLocation(TEXT_PROGRAM, "glyphTexture"), 0);

    MyFont::my_stbtt_initfont();

    //prepare buffers
    Buffers::init_buffers();
    Buffers::sync_buffers();

    glClearColor(.0f, .0f,.0f,1.f);
    glViewport(width / 2 - Window::width  / 2,
               height / 2 - Window::height / 2, width,height);

    using namespace HanShip;

    float yOffset = 0.0f;
    float speed = 0.25f;

    double lastTime = glfwGetTime();
    constexpr double timeStep = 1.0 / 60.0; // 60 updates per second
    double accumulator = 0.0;
    constexpr double asteroidTime = 50.0;
    double timeToNextAsteroid = asteroidTime;

    const char* gameOverText = "Game Over!";
    const int gameOverTextLen = (int) strlen(gameOverText);
    const char* resetGameText = "Press Enter to restart";
    const int resetGameTextLen = (int) strlen(resetGameText);

    float letterSpace = 37.0f;
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(winPtr)) {

        double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;
        lastTime = currentTime;

        accumulator += frameTime;

        while (accumulator >= timeStep) {
            // game state update

            if (!GAME_OVER) {

                timeToNextAsteroid -= 1.0;
                if (timeToNextAsteroid <= 0.0) {
                    Asteroids::makeRandAsteroids(1);
                    timeToNextAsteroid = asteroidTime;
                }

                yOffset += (float) (speed * timeStep);
                if (yOffset > 1.0f) {
                    yOffset = 0.0f;
                }

                //collision checks
                for (int i = 0; i < Asteroids::asteroidsSize; ++i) {
                    if (!Asteroids::asteroids[i].deleted && HanShip::checkCollisionWithShip(Asteroids::asteroids[i])) {
                        Asteroids::asteroids[i].deleted = true;
                        HanShip::deleted = true;
                        GAME_OVER = true;
                    }
                }

                for (int i = 0; i < HanShip::projectilesSize; ++i) {
                    if (!HanShip::projectiles[i].deleted) {
                        for (int j = 0; j < Asteroids::asteroidsSize; ++j) {
                            if (!Asteroids::asteroids[j].deleted &&
                                checkCollisionWithProjectile(Asteroids::asteroids[j], HanShip::projectiles[i])) {
                                Asteroids::asteroids[j].deleted = true;
                                HanShip::projectiles[i].deleted = true;
                                ++SCORE;
                            }
                        }
                    }
                }
                updateText();
            }
            Asteroids::updateAsteroids();
            HanShip::updateProjectiles();
            HanShip::updateShip();
            accumulator -= timeStep;
        }

        //rendering
        glClear(GL_COLOR_BUFFER_BIT);

        //send background data
        glUseProgram(BACKGROUND_PROGRAM);
        glUniform1f(BACKGROUND_PROGRAM_YOFFSET, yOffset);

        Buffers::clear_buff();

        Textures::bindTexture(BackGround::backgroundTexture);
        BackGround::drawBackground();

        Buffers::sync_buffers();

        drawBuffer();

        //send ship data
        glUseProgram(SHIP_PROGRAM);

        Buffers::clear_buff();

        if (!HanShip::deleted) {
            Textures::bindTexture(HanShip::shipTexture);
            HanShip::drawShip();
        } else {
            Textures::bindTexture(HanShip::shipBoomTexture);
            HanShip::drawShipBoom();
        }

        Buffers::sync_buffers();

        drawBuffer();

        //send asteroid data
        glUseProgram(ASTEROID_PROGRAM);
        //Textures::bindBoomTexture();

        Buffers::clear_buff();

        Textures::bindTexture(Asteroids::asteroidTexture);

        for (int i = 0; i < Asteroids::asteroidsSize; ++i) {
            if (!Asteroids::asteroids[i].deleted)
                Asteroids::asteroids[i].drawAsteroid();
        }

        Buffers::sync_buffers();

        drawBuffer();

        Buffers::clear_buff();

        Textures::bindTexture(Asteroids::asteroidBoomTexture);

        for (int i = 0; i < Asteroids::asteroidsSize; ++i) {
            if (Asteroids::asteroids[i].deleted && Asteroids::asteroids[i].spriteCurrFrame < Asteroids::Asteroid::maxFrames) {
                Asteroids::asteroids[i].drawBoom();
            }
        }

        Buffers::sync_buffers();

        drawBuffer();

        //send projectile data
        glUseProgram(PROJECTILE_PROGRAM);
        Buffers::clear_buff();

        for (int i = 0; i < HanShip::projectilesSize; ++i) {
            if (!HanShip::projectiles[i].deleted) {
                HanShip::projectiles[i].drawProjectile();
            }
        }

        Buffers::sync_buffers();

        drawBuffer();

        //send text data
        glUseProgram(TEXT_PROGRAM);

        Buffers::clear_buff();

        MyFont::bindFont();

        for (int i = 0; i < textLen; ++i) {
            MyFont::drawGlyph(10 + (letterSpace*(float)i), 60, text[i]);
        }

        if (GAME_OVER) {
            for (int i = 0; i < gameOverTextLen; ++i) {
                MyFont::drawGlyph((float) Window::width / 2 - ((float)gameOverTextLen / 3 * 60) + (letterSpace * (float)i), (float)Window::height / 2 + 60,
                          gameOverText[i]);
            }

            for (int i = 0; i < resetGameTextLen; ++i) {
                MyFont::drawGlyph((float) Window::width / 2 - ((float)resetGameTextLen / 3 * 60) + (letterSpace * (float)i), (float) Window::height / 2 + 180,
                          resetGameText[i]);
            }
        }

        Buffers::sync_buffers();

        drawBuffer();

        glfwSwapBuffers(winPtr);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}