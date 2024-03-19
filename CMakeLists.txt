cmake_minimum_required(VERSION 3.25)
set(CMAKE_VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

project(HanSpace)

include_directories(headers)

set(HEADERS headers/glad.h headers/khrplatform.h headers/stb_image.h headers/HanShip.h headers/Window.h headers/Shaders.h headers/Global_Variables.h headers/Buffers.h headers/Background.h headers/Textures.h)

set(SOURCES src/glad.c src/main.cpp headers/Asteroids.h headers/Randomization.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME}
        PRIVATE -Wall -Wextra -pedantic -ggdb -std=c++20
        )

target_include_directories(${PROJECT_NAME} PRIVATE
        "G:/projects/repos/libraries/glfw-3.3.9.bin.WIN64/include/GLFW")

link_directories("G:/projects/repos/libraries/glfw-3.3.9.bin.WIN64/lib-mingw-w64")

# Set flags for static linking
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

target_link_libraries(${PROJECT_NAME} PRIVATE
        "G:/projects/repos/libraries/glfw-3.3.9.bin.WIN64/lib-mingw-w64/libglfw3.a"
        opengl32
        kernel32
        user32
        gdi32
        shell32)
