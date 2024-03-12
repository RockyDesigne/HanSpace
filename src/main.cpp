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
    GLuint shipTexId, backgroundTexId;

    void createShipTexture() {
        unsigned char *data = stbi_load(R"(G:\projects\repos\HanGames\HanSpace\textures\SpaceShipSmall.png)", &width, &height, &nrChannels, 0);

        glGenTextures(1, &shipTexId);
        glBindTexture(GL_TEXTURE_2D, shipTexId);

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

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D); // this generates all the levels of the mipmap for the bound texture

        stbi_image_free(data);
    }

    void bindShipTexture() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shipTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void bindBackgroundTexture() {
        /*
         * TO DO
         * no need to give the parameters like this every time, only once is enough
         */
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, backgroundTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
    glUniform2f(RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

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
    GLuint fragShaderId = Shaders::compile_shader(Shaders::fragSource,GL_FRAGMENT_SHADER);
    Shaders::link_program(vShaderId, fragShaderId);
    glUseProgram(PROGRAM_ID);

    RESOLUTION_UNIFORM = glGetUniformLocation(PROGRAM_ID, "resolution");
    glUniform2f(RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    //tell each sampler the unit it belongs to
    glUniform1i(glGetUniformLocation(PROGRAM_ID, "shipTexture"), 0);
    glUniform1i(glGetUniformLocation(PROGRAM_ID, "backgroundTexture"), 1);

    //prepare texture
    Textures::createBackgroundTexture();
    Textures::createShipTexture();

    //prepare buffers
    Buffers::init_buffers();
    Buffers::sync_buffers();

    glClearColor(.0f, .0f,.0f,1.f);
    glViewport(width / 2 - Window::width  / 2,
               height / 2 - Window::height / 2, width,height);

    using namespace HanShip;

    /*
     * TO DO:
     * make a separate shader for drawing the ship and for the background, this is currently not at all optimized
     */
    while (!glfwWindowShouldClose(winPtr)) {
        glClear(GL_COLOR_BUFFER_BIT);
        Buffers::clear_buff();

        glUniform1i(glGetUniformLocation(PROGRAM_ID, "useShipTex"), false);
        Textures::bindBackgroundTexture();
        BackGround::drawBackground();

        glDrawArrays(GL_TRIANGLE_STRIP,
                     0,
                     4);

        glUniform1i(glGetUniformLocation(PROGRAM_ID, "useShipTex"), true);
        Textures::bindShipTexture();
        HanShip::drawShip();

        glDrawArrays(GL_TRIANGLE_STRIP,
                     4,
                     4);

        Buffers::sync_buffers();

        glfwSwapBuffers(winPtr);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}