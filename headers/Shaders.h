//
// Created by HORIA on 12.03.2024.
//

#ifndef HANSPACE_SHADERS_H
#define HANSPACE_SHADERS_H

#include "glad.h"
#include "Global_Variables.h"
#include <iostream>

namespace Shaders {
    const char* vertexSource = "#version 460 core\n"
                               "layout(location = 0) in vec2 position;\n"
                               "layout(location = 1) in vec3 color;\n"
                               "layout (location = 2) in vec2 texCoords;\n"
                               "out vec2 fragTexCoords;\n"
                               "uniform vec2 resolution;\n"
                               "out vec4 fragColor;\n"
                               "vec2 screen_to_ndc(vec2 pos) {\n"
                               "return (pos / resolution) * 2.0f - 1.0f;\n"
                               "}\n"
                               "void main() {\n"
                               "gl_Position = vec4(screen_to_ndc(position), 0.0f, 1.0f);\n"
                               "fragColor = vec4(color, 1.0f);\n"
                               "fragTexCoords = texCoords;\n"
                               "}\n";
    const char* shipShader = "#version 460 core\n"
                             "in vec4 fragColor;\n"
                             "in vec2 fragTexCoords;\n"
                             "out vec4 outColor;\n"
                             "uniform sampler2D shipTexture;\n"
                             "void main() {\n"
                             "\n"
                             "  outColor = texture(shipTexture, fragTexCoords);\n"
                             "}\n";

    const char* backgroundShader = "#version 460 core\n"
                                   "in vec2 fragTexCoords;\n"
                                   "out vec4 outColor;\n"
                                   "uniform sampler2D backgroundTexture;\n"
                                   "uniform float yOffset;\n"
                                   "void main() {\n"
                                   "    outColor = texture(backgroundTexture, vec2(fragTexCoords.x,fragTexCoords.y + yOffset));\n"
                                   "}\n";

    const char* asteroidShader = "#version 460 core\n"
                                 "in vec2 fragTexCoords;\n"
                                 "out vec4 outColor;\n"
                                 "uniform sampler2D asteroidTexture;\n"
                                 "uniform sampler2D boomTexture;\n"
                                 "uniform bool deleted;\n"
                                 "void main() {\n"
                                 "    if (!deleted)\n"
                                 "      outColor = texture(asteroidTexture, vec2(fragTexCoords.x,fragTexCoords.y));\n"
                                 "    else\n"
                                 "      outColor = texture(boomTexture, vec2(fragTexCoords.x,fragTexCoords.y));\n"
                                 "}\n";

    const char* projectileShader = "#version 460 core\n"
                                   "in vec2 fragTexCoords;\n"
                                   "in vec4 fragColor;\n"
                                   "out vec4 outColor;\n"
                                   "void main() {\n"
                                   "    outColor = fragColor;\n"
                                   "}\n";

    const char* textShader = "#version 460 core\n"
                             "in vec4 fragColor;\n"
                             "in vec2 fragTexCoords;\n"
                             "out vec4 outColor;\n"
                             "uniform sampler2D shipTexture;\n"
                             "void main() {\n"
                             "\n"
                             "  float d = texture(shipTexture, fragTexCoords).r;\n"
                             "  float aaf = fwidth(d);\n"
                             "  float alpha = smoothstep(0.5-aaf, 0.5 + aaf, d);\n"
                             "  outColor = vec4(fragColor.rgb, alpha);\n"
                             "  //vec4 textureColor = texture(shipTexture, fragTexCoords);\n"
                             "  //outColor = vec4(textureColor.r,textureColor.gb, textureColor.a);\n"
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

    bool link_program(GLuint vertexShader, GLuint fragmentShader, GLuint& program)
    {
        GLint success;
        char infoLog[512] = {0};

        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
            return false;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return true;
    }
}

#endif //HANSPACE_SHADERS_H
