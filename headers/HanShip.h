//
// Created by HORIA on 12.03.2024.
//

#ifndef HANSPACE_HANSHIP_H
#define HANSPACE_HANSHIP_H

#include <utility>

namespace HanShip {
    using COORDS = std::pair<float,float>;
    COORDS bottomLeft = {-25.f, -25.f};
    COORDS bottomRight = {25.f, -25.f};
    COORDS topLeft = {-25.f, 25.f};
    COORDS topRight = {25.f, 25.f};
}

#endif //HANSPACE_HANSHIP_H
