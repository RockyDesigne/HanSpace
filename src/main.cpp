#include "glad.h"
#include <glfw3.h>
#include <iostream>
#include "Window.h"
#include "HanShip.h"
#include "Shaders.h"
#include "Global_Variables.h"
#include "Buffers.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Asteroids {
    using COORDS = std::pair<float,float>;

    constexpr int asteroidWidthFromCenter = 50;

    COORDS bottomLeft = {Window::width/2 - asteroidWidthFromCenter,Window::height/2-asteroidWidthFromCenter};
    COORDS bottomRight = {Window::width/2 + asteroidWidthFromCenter, Window::height/2-asteroidWidthFromCenter};
    COORDS topLeft = {Window::width/2 - asteroidWidthFromCenter, Window::height/2+asteroidWidthFromCenter};
    COORDS topRight = {Window::width/2 + asteroidWidthFromCenter, Window::height/2+asteroidWidthFromCenter};

    void drawAsteroid() {
        Buffers::push_vert(bottomLeft.first,bottomLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
        Buffers::push_vert(bottomRight.first,bottomRight.second, 1.0f,0.5f,0.75f, 1.0f, 0.0f);
        Buffers::push_vert(topLeft.first,topLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
        Buffers::push_vert(topRight.first,topRight.second,1.0f, 0.5f,0.75, 1.0f, 1.0f);
    }

}

namespace BackGround {
    using COORDS = std::pair<float,float>;

    COORDS bottomLeft = {0.f,0.f};
    COORDS bottomRight = {Window::width, 0.f};
    COORDS topLeft = {0.f, Window::height};
    COORDS topRight = {Window::width, Window::height};

    void calcBackground() {
        bottomLeft = {0.f,0.f};
        bottomRight = {Window::width, 0.f};
        topLeft = {0.f, Window::height};
        topRight = {Window::width, Window::height};
    }

    void drawBackground() {
        Buffers::push_vert(bottomLeft.first,bottomLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 0.0f);
        Buffers::push_vert(bottomRight.first,bottomRight.second, 1.0f,0.5f,0.75f, 1.0f, 0.0f);
        Buffers::push_vert(topLeft.first,topLeft.second, 1.0f, 0.5f,0.75f, 0.0f, 1.0f);
        Buffers::push_vert(topRight.first,topRight.second,1.0f, 0.5f,0.75, 1.0f, 1.0f);
    }

}

namespace Textures {
    int width, height, nrChannels;
    GLuint shipTexId, backgroundTexId, asteroidTexId;

    void createShipTexture() {
        unsigned char *data = stbi_load(R"(G:\projects\repos\HanGames\HanSpace\textures\SpaceShipSmall.png)", &width, &height, &nrChannels, 0);

        glGenTextures(1, &shipTexId);
        glBindTexture(GL_TEXTURE_2D, shipTexId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        /*
         * A mipmap is a sequence of textures, each a power of two smaller than the previous
         * the more you zoom out, and the picture gets smaller, the lower level mipmaps are used
         */
        glGenerateMipmap(GL_TEXTURE_2D); // this generates all the levels of the mipmap for the bound texture

        stbi_image_free(data);
    }

    void createBackgroundTexture() {
        unsigned char *data = stbi_load(R"(G:\projects\repos\HanGames\HanSpace\textures\seamless space.PNG)", &width, &height, &nrChannels, 0);

        glGenTextures(1, &backgroundTexId);
        glBindTexture(GL_TEXTURE_2D, backgroundTexId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D); // this generates all the levels of the mipmap for the bound texture

        stbi_image_free(data);
    }

    void createAsteroidTexture() {
        unsigned char *data = stbi_load(R"(G:\projects\repos\HanGames\HanSpace\textures\asteroid-big-0000.png)", &width, &height, &nrChannels, 0);

        glGenTextures(1, &asteroidTexId);
        glBindTexture(GL_TEXTURE_2D, asteroidTexId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D); // this generates all the levels of the mipmap for the bound texture

        stbi_image_free(data);
    }

    void bindShipTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shipTexId);
    }

    void bindBackgroundTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTexId);
    }

    void bindAsteroidTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, asteroidTexId);
    }

}

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

    //prepare buffers
    Buffers::init_buffers();
    Buffers::sync_buffers();

    glClearColor(.0f, .0f,.0f,1.f);
    glViewport(width / 2 - Window::width  / 2,
               height / 2 - Window::height / 2, width,height);

    using namespace HanShip;

    float yOffset = 0.0f;
    float speed = 0.0025f;

    while (!glfwWindowShouldClose(winPtr)) {
        glClear(GL_COLOR_BUFFER_BIT);
        Buffers::clear_buff();

        yOffset += speed;
        if (yOffset > 1.0f) {
            yOffset = 0.0f;
        }

        glUseProgram(BACKGROUND_PROGRAM);
        glUniform1f(BACKGROUND_PROGRAM_YOFFSET, yOffset);

        Textures::bindBackgroundTexture();
        BackGround::drawBackground();

        glDrawArrays(GL_TRIANGLE_STRIP,
                     0,
                     4);

        glUseProgram(SHIP_PROGRAM);
        Textures::bindShipTexture();
        HanShip::drawShip();

        glDrawArrays(GL_TRIANGLE_STRIP,
                     4,
                     4);


        glUseProgram(ASTEROID_PROGRAM);
        Textures::bindAsteroidTexture();
        Asteroids::drawAsteroid();
        glDrawArrays(GL_TRIANGLE_STRIP,
                     8,
                     4);

        Buffers::sync_buffers();

        glfwSwapBuffers(winPtr);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}