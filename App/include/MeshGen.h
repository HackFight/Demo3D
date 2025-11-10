#pragma once

// Engine
#include "RendererAPI/VertexArray.h"

class MeshGen {
public:
    static Core::VertexArray GetQuad();
    static Core::VertexArray GetCube();
    static Core::VertexArray GetReversedCube();
    static Core::VertexArray GetPlane(const int size);
};