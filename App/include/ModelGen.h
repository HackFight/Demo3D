#pragma once

// libs
#include "Core/Model.h"

// std
#include <memory>

class ModelGen {
public:
    static std::shared_ptr<Core::Model> GetQuad(std::vector<std::shared_ptr<Core::Texture>> textures = std::vector<std::shared_ptr<Core::Texture>>());
    static std::shared_ptr<Core::Model> GetCube(std::vector<std::shared_ptr<Core::Texture>> textures = std::vector<std::shared_ptr<Core::Texture>>());
    static std::shared_ptr<Core::Model> GetPlane(const int size, std::vector<std::shared_ptr<Core::Texture>> textures = std::vector<std::shared_ptr<Core::Texture>>());

private:
    static std::vector<Mesh::Texture> toMeshTextures(std::vector<std::shared_ptr<Core::Texture>> textures);
};