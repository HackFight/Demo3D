#pragma once

// Engine
#include "Core/Model.h"

class ModelGen {
public:
    static Core::Model getQuad(std::vector<size_t> textures = std::vector<size_t>{});
    static Core::Model getCube(std::vector<size_t> textures = std::vector<size_t>{});
	static Core::Model getReversedCube(std::vector<size_t> textures = std::vector<size_t>{});
    static Core::Model getPlane(int size, std::vector<size_t> textures = std::vector<size_t>{});

private:
    static std::vector<Core::Mesh::Texture> toMeshTextures(std::vector<size_t> textures);
};