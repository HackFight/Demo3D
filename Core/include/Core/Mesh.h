#pragma once

#include "RendererAPI/Buffer.h"
#include "RendererAPI/Shader.h"
#include "RendererAPI/Texture.h"
#include "RendererAPI/VertexArray.h"
#include <cstdint>
#include <memory>
#include <vector>

class Mesh
{
public:
    struct Texture
    {
        std::shared_ptr<Core::Texture> ptr;
        std::string type;
        std::string path;
    };

    // mesh data
    std::shared_ptr<Core::VertexArray> vertexArray;
    std::vector<Texture> textures;
    
    Mesh(std::vector<Core::Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
    void Draw(std::shared_ptr<Core::Shader> shader);
};