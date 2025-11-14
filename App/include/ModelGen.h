#pragma once

// Engine
#include "Core/Model.h"
#include <cstdint>

class ModelGen {
public:
    static Core::Model GetQuad(std::vector<uint32_t> textures = std::vector<uint32_t>{});
    static Core::Model GetCube(std::vector<uint32_t> textures = std::vector<uint32_t>{});
    static Core::Model GetPlane(const int size, std::vector<uint32_t> textures = std::vector<uint32_t>{});

private:
    static std::vector<Core::Mesh::Texture> toMeshTextures(std::vector<uint32_t> textures);
};