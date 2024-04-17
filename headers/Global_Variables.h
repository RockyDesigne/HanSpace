//
// Created by HORIA on 12.03.2024.
//

#ifndef HANSPACE_GLOBAL_VARIABLES_H
#define HANSPACE_GLOBAL_VARIABLES_H

#include "glad.h"

GLuint SHIP_PROGRAM, BACKGROUND_PROGRAM, ASTEROID_PROGRAM, PROJECTILE_PROGRAM, TEXT_PROGRAM;
GLint SHIP_PROGRAM_RESOLUTION_UNIFORM, BACKGROUND_PROGRAM_RESOLUTION_UNIFORM, BACKGROUND_PROGRAM_YOFFSET,
        ASTEROID_PROGRAM_RESOLUTION_UNIFORM, PROJECTILE_PROGRAM_RESOLUTION_UNIFORM, TEXT_PROGRAM_RESOLUTION_UNIFORM,
        ASTEROID_PROGRAM_DELETED_UNIFORM,
        SHIP_PROGRAM_DESTROYED_UNIFORM;
unsigned int SCORE = 0;

#endif //HANSPACE_GLOBAL_VARIABLES_H
