#pragma once

// std
#include <vector>

// Core
#include "RendererAPI/VertexArray.h"
#include "RendererAPI/Texture.h"
#include "RendererAPI/Shader.h"

namespace Core
{
    class Mesh
    {
    public:
        struct Texture
        {
            Core::Texture ptr;
            std::string type;
            std::string path;
        };

        // mesh data
        Core::VertexArray vertexArray;
        std::vector<Texture> textures;

        Mesh(std::vector<Core::Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
        void Draw(Core::Shader shader);
    };
}