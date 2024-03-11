#include "glad.h"
#include <glfw3.h>
#include <iostream>
#include <cassert>

GLuint PROGRAM_ID;
GLint RESOLUTION_UNIFORM;

namespace HanShip {
    using COORDS = std::pair<float,float>;
    COORDS bottomLeft = {-25.f, -25.f};
    COORDS bottomRight = {25.f, -25.f};
    COORDS topLeft = {-25.f, 25.f};
    COORDS topRight = {25.f, 25.f};
}

namespace Window {
    GLFWwindow* winPtr;
    int width = 800;
    int height = 600;
    const char* title = "HanSpace";
    constexpr const int maxWidth = 1920;
    constexpr const int maxHeight = 1080;
}

namespace Shaders {
    const char* vertexSource = "#version 460 core\n"
                               "layout(location = 0) in vec2 position;\n"
                               "layout(location = 1) in vec3 color;\n"
                               "uniform vec2 resolution;\n"
                               "out vec4 fragColor;\n"
                               "vec2 screen_to_ndc(vec2 pos) {\n"
                               "return (pos / resolution) * 2.0f - 1.0f;\n"
                               "}\n"
                               "void main() {\n"
                               "gl_Position = vec4(screen_to_ndc(position), 0.0f, 1.0f);\n"
                               "fragColor = vec4(color, 1.0f);\n"
                               "}\n";
    const char* fragSource = "#version 460 core\n"
                             "in vec4 fragColor;\n"
                             "out vec4 outColor;\n"
                             "void main() {\n"
                             "\n"
                             "outColor = fragColor;\n"
                             "}\n";

    GLuint compile_shader(const char* shaderSource, GLenum shaderType)
    {
        GLuint shader;
        GLint success;
        char infoLog[512]={0};

        shader = glCreateShader(shaderType);

        glShaderSource(shader,1,&shaderSource,nullptr);

        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            switch (shaderType) {
                case GL_VERTEX_SHADER: {
                    fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
                    break;
                }
                case GL_FRAGMENT_SHADER: {
                    fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
                    break;
                }
                default: {
                    fprintf(stderr, "ERROR::SHADER::UNKNOWN::COMPILATION_FAILED: %s\n", infoLog);
                    break;
                }
            }
            exit(-1);
        }

        return shader;
    }

    bool link_program(GLuint vertexShader, GLuint fragmentShader)
    {
        GLint success;
        char infoLog[512] = {0};

        PROGRAM_ID = glCreateProgram();
        glAttachShader(PROGRAM_ID, vertexShader);
        glAttachShader(PROGRAM_ID, fragmentShader);

        glLinkProgram(PROGRAM_ID);

        glGetProgramiv(PROGRAM_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(PROGRAM_ID, 512, nullptr, infoLog);
            fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
            return false;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return true;
    }
}

namespace Buffers {
    enum Attributes {
        POSITION_ATTRIBUTES,
        COLOR_ATTRIBUTES,
    };

    GLuint vao = 0, vbo = 0;

    struct Vertex {
    [[maybe_unused]]    float x, y;
    [[maybe_unused]]    float r, g, b;
    };

    constexpr const int verticesCap = Window::maxWidth * Window::maxHeight;
    Vertex vertices[verticesCap];
    size_t verticesCount = 0;

    void init_buffers() {
        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //describing the data to opengl
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(POSITION_ATTRIBUTES);
        glVertexAttribPointer(POSITION_ATTRIBUTES,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              (2 + 3) * sizeof(float),
                              reinterpret_cast<GLvoid*>(0));

        //glVertexAttribDivisor(POSITION_ATTRIBUTES, 1);

        glEnableVertexAttribArray(COLOR_ATTRIBUTES);
        glVertexAttribPointer(COLOR_ATTRIBUTES,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              (2 + 3) * sizeof(float),
                              reinterpret_cast<GLvoid*>(sizeof(float)*2));

        //glVertexAttribDivisor(COLOR_ATTRIBUTES, 1);
    }

    void push_vert(float x, float y, float r, float g, float b) {
        assert(verticesCount < verticesCap && "Overflow in the VERTICES buffer!");
        vertices[verticesCount].x = x;
        vertices[verticesCount].y = y;
        vertices[verticesCount].r = r;
        vertices[verticesCount].g = g;
        vertices[verticesCount].b = b;
        ++verticesCount;
    }

    void clear_buff() {
        verticesCount = 0;
    }

    void sync_buffers() {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        static_cast<GLsizeiptr>(verticesCount * sizeof (vertices[0])),
                        vertices);

    }

}

void handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
    using namespace HanShip;
    (void) scancode;
    (void) mods;
    float impulse = 25.f;
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
                break;
            }
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

    //compile shaders
    GLuint vShaderId = Shaders::compile_shader(Shaders::vertexSource, GL_VERTEX_SHADER);
    GLuint fragShaderId = Shaders::compile_shader(Shaders::fragSource,GL_FRAGMENT_SHADER);
    Shaders::link_program(vShaderId, fragShaderId);
    glUseProgram(PROGRAM_ID);

    RESOLUTION_UNIFORM = glGetUniformLocation(PROGRAM_ID, "resolution");
    glUniform2f(RESOLUTION_UNIFORM, static_cast<GLfloat>(Window::width),static_cast<GLfloat>(Window::height));

    //prepare buffers
    Buffers::init_buffers();
    Buffers::sync_buffers();

    glClearColor(.0f, .0f,.0f,1.f);
    glViewport(width / 2 - Window::width  / 2,
               height / 2 - Window::height / 2, width,height);
    using namespace HanShip;
    while (!glfwWindowShouldClose(winPtr)) {

        Buffers::clear_buff();

        Buffers::push_vert(bottomLeft.first,bottomLeft.second, 1.0f, 0.5f,0.75f);
        Buffers::push_vert(bottomRight.first,bottomRight.second, 1.0f,0.5f,0.75f);
        Buffers::push_vert(topLeft.first,topLeft.second, 1.0f, 0.5f,0.75f);
        Buffers::push_vert(topRight.first,topRight.second,1.0f, 0.5f,0.75f);
        Buffers::sync_buffers();

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_STRIP,
                              0,
                              4);

        glfwSwapBuffers(winPtr);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}