cmake_minimum_required(VERSION 3.25)
set(CMAKE_VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

project(HanSpace)

include_directories(headers)

set(HEADERS headers/glad.h headers/khrplatform.h)

set(SOURCES src/glad.c src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME}
        PRIVATE -Wall -Wextra -pedantic -ggdb -std=c++20
        )

target_include_directories(${PROJECT_NAME} PRIVATE
        "<GLFW HEADERS>")

link_directories("<GLFW BINS>")

# Set flags for static linking
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

target_link_libraries(${PROJECT_NAME} PRIVATE
        "<GLFW LIB PATH STATIC>"
        opengl32 
        kernel32 #these are windows specific
        user32
        gdi32
        shell32)
