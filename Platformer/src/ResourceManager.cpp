#include "ResourceManager.h"

//Engine
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/FramebufferManager.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"
#include <cstdint>

const std::vector<Core::Vertex> quadVertices {
    {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{ 1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{ 1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
    {{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
};
const uint32_t quadIndices[] {
    0, 1, 2,
    0, 2, 3
};

namespace Platformer {

    unsigned int ResourceManager::quadVAO;
    unsigned int ResourceManager::textureShader;

    void ResourceManager::Init() {
        //##### Shaders ######
        textureShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", "shaders/texture.frag");

        //##### VAOs #####
        //Quad
        unsigned int vertexBuffer = Core::VertexBufferManager::CreateVertexBuffer((float*)quadVertices.data(), quadVertices.size()*sizeof(Core::Vertex));
        unsigned int indexBuffer = Core::IndexBufferManager::CreateIndexBuffer(quadIndices, 6);
        quadVAO = Core::VertexArrayManager::CreateVertexArray();
        Core::VertexArrayManager::AddVertexBuffer(quadVAO, vertexBuffer);
        Core::VertexArrayManager::SetIndexBuffer(quadVAO, indexBuffer);
    }

    unsigned int ResourceManager::GetVAO(Primitive primitive) {
        switch (primitive) {
            case Quad:
                return quadVAO;
                break;
        }
        return -1;
    };

    unsigned int ResourceManager::GetShader(Shader shader) {
        switch (shader) {
            case Texture:
                return textureShader;
                break;
        }
        return -1;
    }

    void ResourceManager::ReleaseAll() {
        Core::VertexBufferManager::ReleaseAll();
        Core::IndexBufferManager::ReleaseAll();
        Core::VertexBufferManager::ReleaseAll();
        Core::ShaderManager::ReleaseAll();
        Core::TextureManager::ReleaseAll();
        Core::RenderbufferManager::ReleaseAll();
        Core::FramebufferManager::ReleaseAll();
    }
}