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
}

#endif //HANSPACE_WINDOW_H
