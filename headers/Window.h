//
// Created by HORIA on 12.03.2024.
//

#ifndef HANSPACE_WINDOW_H
#define HANSPACE_WINDOW_H

#include <glfw3.h>

namespace Window {
    GLFWwindow* winPtr;
    int width = 800;
    int height = 600;
    const char* title = "HanSpace";
    constexpr const int maxWidth = 1920;
    constexpr const int maxHeight = 1080;

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

}

#endif //HANSPACE_WINDOW_H
