#pragma once

// std
#include <stdint.h>
#include <string>
#include <vector>

// Core
#include "RendererAPI/BufferManager.h"

namespace Core
{
    class Mesh
    {
    public:
        struct Texture
        {
            uint32_t ptr;
            std::string type;
            std::string path;
        };

        // mesh data
        uint32_t vertexArray;
        std::vector<Texture> textures;

        Mesh(std::vector<Core::Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
        Mesh(uint32_t vertexBuffer, uint32_t indexBuffer, std::vector<Texture> textures);
        void Draw(uint32_t shader);
    };
}