//
// Created by HORIA on 12.03.2024.
//

#ifndef HANSPACE_BUFFERS_H
#define HANSPACE_BUFFERS_H

#include "Window.h"
#include "glad.h"
#include <cassert>

namespace Buffers {
    enum Attributes {
        POSITION_ATTRIBUTES,
        COLOR_ATTRIBUTES,
        TEXTURE_ATTRIBUTES,
    };

    GLuint vao = 0, vbo = 0;

    struct Vertex {
        [[maybe_unused]]    float x, y;
        [[maybe_unused]]    float r, g, b;
        [[maybe_unused]]    float tex_x, tex_y;
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
                              (2 + 5) * sizeof(float),
                              reinterpret_cast<GLvoid*>(0));

        //glVertexAttribDivisor(POSITION_ATTRIBUTES, 1);

        glEnableVertexAttribArray(COLOR_ATTRIBUTES);
        glVertexAttribPointer(COLOR_ATTRIBUTES,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              (2 + 5) * sizeof(float),
                              reinterpret_cast<GLvoid*>(sizeof(float)*2));

        glEnableVertexAttribArray(TEXTURE_ATTRIBUTES);
        glVertexAttribPointer(TEXTURE_ATTRIBUTES,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              (2 + 5) * sizeof(float),
                              reinterpret_cast<GLvoid*>(sizeof(float)*5));

        //glVertexAttribDivisor(COLOR_ATTRIBUTES, 1);
    }

    void push_vert(float x, float y, float r, float g, float b, float tex_x, float tex_y) {
        assert(verticesCount < verticesCap && "Overflow in the VERTICES buffer!");
        vertices[verticesCount].x = x;
        vertices[verticesCount].y = y;
        vertices[verticesCount].r = r;
        vertices[verticesCount].g = g;
        vertices[verticesCount].b = b;
        vertices[verticesCount].tex_x = tex_x;
        vertices[verticesCount].tex_y = tex_y;
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

#endif //HANSPACE_BUFFERS_H
