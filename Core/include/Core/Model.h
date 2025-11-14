#pragma once

// std
#include <vector>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Core
#include "Core/Mesh.h"

namespace Core
{
class Model
{
public:
    Model();
    Model(const char* filename);
    Model(std::vector<Mesh> meshes);
    Model(Mesh mesh);

    void Draw(uint32_t shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Mesh::Texture> textures_loaded;

    void loadModel(std::string filename);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
}