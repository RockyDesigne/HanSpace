//
// Created by HORIA on 12.03.2024.
//

#ifndef HANSPACE_HANSHIP_H
#define HANSPACE_HANSHIP_H

#include <utility>

namespace HanShip {
    using COORDS = std::pair<float,float>;
    COORDS bottomLeft = {-100.f, -100.f};
    COORDS bottomRight = {100.f, -100.f};
    COORDS topLeft = {-100.f, 100.f};
    COORDS topRight = {100.f, 100.f};
}

#endif //HANSPACE_HANSHIP_H
