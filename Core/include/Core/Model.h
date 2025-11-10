#pragma once

#include "Core/Mesh.h"
#include "RendererAPI/Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

namespace Core
{
class Model
{
public:
    Model(const char* filename);
    Model(std::vector<Mesh> meshes);

    void Draw(Core::Shader shader);

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