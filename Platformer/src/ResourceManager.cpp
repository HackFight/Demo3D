#include "ResourceManager.h"

//Engine
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/FramebufferManager.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"

const std::vector<Core::Vertex> quadVertices {
    {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{ 1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{ 1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    {{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}
};
const uint32_t quadIndices[] {
    0, 1, 2,
    0, 2, 3
};

namespace Platformer {

    size_t ResourceManager::quadVAO;
    size_t ResourceManager::textureShader;

    void ResourceManager::Init() {
        //##### Shaders ######
        textureShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert",  RESOURCES_PATH "shaders/texture.frag");

        //##### VAOs #####
        //Quad
        unsigned int vertexBuffer = Core::VertexBufferManager::CreateVertexBuffer((float*)quadVertices.data(), quadVertices.size()*sizeof(Core::Vertex));
        unsigned int indexBuffer = Core::IndexBufferManager::CreateIndexBuffer(quadIndices, 6);
        quadVAO = Core::VertexArrayManager::CreateVertexArray();
        Core::VertexArrayManager::AddVertexBuffer(quadVAO, vertexBuffer);
        Core::VertexArrayManager::SetIndexBuffer(quadVAO, indexBuffer);
    }

    size_t ResourceManager::GetVAO(Primitive primitive) {
        switch (primitive) {
            case Quad:
                return quadVAO;
                break;
        }
        return -1;
    };

    size_t ResourceManager::GetShader(Shader shader) {
        switch (shader) {
            case Texture:
                return textureShader;
                break;
        }
        return -1;
    }

    size_t ResourceManager::CreatePlainRGBATexture(int width, int height, unsigned char fill) {
        unsigned char* data = new unsigned char[width * height * 4];
        for(int i = 0; i < width * height * 4; i++) data[i] = fill;

        unsigned int texture = Core::TextureManager::CreateTexture(GL_TEXTURE_2D, GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, false, 0);

        delete[] data;
        return texture;
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