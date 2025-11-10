#pragma once

// Engine
#include "Core/Model.h"

class ModelGen {
public:
    static Core::Model GetQuad(std::vector<Core::Texture> textures = std::vector<Core::Texture>{ Core::Texture() });
    static Core::Model GetCube(std::vector<Core::Texture> textures = std::vector<Core::Texture>{ Core::Texture() });
    static Core::Model GetPlane(const int size, std::vector<Core::Texture> textures = std::vector<Core::Texture>{ Core::Texture() });

private:
    static std::vector<Core::Mesh::Texture> toMeshTextures(std::vector<Core::Texture> textures);
};