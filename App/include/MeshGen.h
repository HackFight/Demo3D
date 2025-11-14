#pragma once

//std
#include <stdint.h>

class MeshGen {
public:
    static uint32_t GetQuad();
    static uint32_t GetCube();
    static uint32_t GetReversedCube();
    static uint32_t GetPlane(const int size);
};