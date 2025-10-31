#pragma once

// libs
#include "RendererAPI/VertexArray.h"

// std
#include <memory>

class MeshGen {
public:
    static std::shared_ptr<Core::VertexArray> GetQuad();
    static std::shared_ptr<Core::VertexArray> GetCube();
    static std::shared_ptr<Core::VertexArray> GetPlane(int size);
};