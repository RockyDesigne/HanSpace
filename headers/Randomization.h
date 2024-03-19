//
// Created by HORIA on 19.03.2024.
//

#ifndef HANSPACE_RANDOMIZATION_H
#define HANSPACE_RANDOMIZATION_H

#include <random>

namespace Random {
    std::mt19937 mt {std::random_device{}()};
    int getRandNum(std::uniform_int_distribution<int>& dist) {
        return dist(mt);
    }
}

#endif //HANSPACE_RANDOMIZATION_H
