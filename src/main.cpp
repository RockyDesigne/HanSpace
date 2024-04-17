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
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

constexpr uint64_t bitmapWidth = 512;
constexpr uint64_t bitmapHeight = 512;

constexpr uint64_t fileSize = 50'000;
unsigned char ttf_buffer[fileSize];
unsigned char pixels[512 * 512];
stbtt_bakedchar chardata[200];

GLuint fontTexture;

void my_stbtt_initfont()
{
    auto file = fopen(R"(G:\projects\repos\HanGames\HanSpace\font\LTSuperiorMono-Regular.otf)", "rb");
    fread(ttf_buffer, 1, fileSize,
          file);

    fclose(file);

    stbtt_fontinfo font;

    auto offset = stbtt_GetFontOffsetForIndex(ttf_buffer,0);

    stbtt_InitFont(&font, ttf_buffer, offset);

    stbtt_BakeFontBitmap(ttf_buffer, 0, 64, pixels, 512, 512, 0, font.numGlyphs, chardata);

    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

void bindFont() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
}

void drawGlyph(float x, float y, char c) {

    stbtt_aligned_quad q = {};

    stbtt_GetBakedQuad(reinterpret_cast<const stbtt_bakedchar*>(chardata),
                       512,
                       512,  // same data as above
                       c,             // character to display
                       &x,
                       &y,   // pointers to current position in screen pixel space
                       &q,      // output: quad to draw
                       1);

    float windowHeight = Window::height; // replace with actual function to get window height

    // invert y coordinates
    q.y0 = windowHeight - q.y0;
    q.y1 = windowHeight - q.y1;

    Buffers::push_vert(q.x0,q.y0, 1.0f, 0.5f,0.75f, q.s0,q.t0);
    Buffers::push_vert(q.x1,q.y0, 1.0f,0.5f,0.75f, q.s1,q.t0);
    Buffers::push_vert(q.x0,q.y1, 1.0f, 0.5f,0.75f, q.s0,q.t1);
    Buffers::push_vert(q.x1,q.y1,1.0f, 0.5f,0.75, q.s1,q.t1);
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

auto score = "Score: " + std::to_string(SCORE);
char* text = score.data();
int textLen = (int) strlen(text);

void updateText() {
    score = "Score: " + std::to_string(SCORE);
    text = score.data();
    textLen = (int) score.length();
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

    Textures::createShipTexture();
    Textures::createShipBoomTexture();

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
    Textures::createBoomTexture();

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

    my_stbtt_initfont();

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

    float letterSpace = 37.0f;
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(winPtr)) {

        double currentTime = glfwGetTime();
        double frameTime = currentTime - lastTime;
        lastTime = currentTime;

        accumulator += frameTime;

        while (accumulator >= timeStep) {
            // game state update

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
                }
            }

            for (int i = 0; i < HanShip::projectilesSize; ++i) {
                if (!HanShip::projectiles[i].deleted) {
                    for (int j = 0; j < Asteroids::asteroidsSize; ++j) {
                        if (!Asteroids::asteroids[j].deleted && checkCollisionWithProjectile(Asteroids::asteroids[j], HanShip::projectiles[i])) {
                            Asteroids::asteroids[j].deleted = true;
                            HanShip::projectiles[i].deleted = true;
                            ++SCORE;
                        }
                    }
                }
            }

            Asteroids::updateAsteroids();
            HanShip::updateProjectiles();
            HanShip::updateShip();
            updateText();

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

        if (!HanShip::deleted) {
            Textures::bindShipTexture();
            HanShip::drawShip();
        } else {
            Textures::bindShipBoomTexture();
            HanShip::drawShipBoom();
        }

        glDrawArrays(GL_TRIANGLE_STRIP, (GLint)Buffers::verticesCount - 4, 4);

        glUseProgram(ASTEROID_PROGRAM);
        Textures::bindAsteroidTexture();
        Textures::bindBoomTexture();

        for (const auto& asteroid : Asteroids::asteroids) {
            glUniform1i(ASTEROID_PROGRAM_DELETED_UNIFORM, asteroid.deleted);
            if (!asteroid.deleted) {
                asteroid.drawAsteroid();
                glDrawArrays(GL_TRIANGLE_STRIP, (GLint) Buffers::verticesCount - 4, 4);
            } else if (asteroid.deleted && asteroid.spriteCurrFrame < Asteroids::Asteroid::maxFrames) {
                asteroid.drawBoom();
                glDrawArrays(GL_TRIANGLE_STRIP, (GLint) Buffers::verticesCount - 4, 4);
            }
        }

        glUseProgram(PROJECTILE_PROGRAM);

        for (const auto& projectile : HanShip::projectiles) {
            if (!projectile.deleted) {
                projectile.drawProjectile();
                glDrawArrays(GL_TRIANGLE_STRIP, (GLint) Buffers::verticesCount - 4, 4);
            }
        }

        glUseProgram(TEXT_PROGRAM);
        bindFont();
        for (int i = 0; i < textLen; ++i) {
            drawGlyph(10 + (letterSpace*i), 60, text[i]);
            glDrawArrays(GL_TRIANGLE_STRIP, (GLint)Buffers::verticesCount - 4, 4);
        }

        //Asteroids::updateAsteroids();
        Buffers::sync_buffers();

        glfwSwapBuffers(winPtr);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}