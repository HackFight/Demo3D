#pragma once

//std
#include <cstddef>

class MeshGen {
public:
    static size_t getQuad();
    static size_t getCube();
    static size_t getReversedCube();
    static size_t getPlane(const int size);
};