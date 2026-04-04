#include "AssetsManager.h"

//Engine
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/FramebufferManager.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"

// libs
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

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

    size_t AssetsManager::quadVAO;
    size_t AssetsManager::textureShader, AssetsManager::postProcessingShader, AssetsManager::intermediaryShader;

    void AssetsManager::Init() {
        //##### Shaders ######
        textureShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert",  RESOURCES_PATH "shaders/texture.frag");
        intermediaryShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/layer.vert",  RESOURCES_PATH "shaders/texture.frag");
        postProcessingShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/layer.vert",  RESOURCES_PATH "shaders/post.frag");

        //##### VAOs #####
        //Quad
        size_t vertexBuffer = Core::VertexBufferManager::CreateVertexBuffer((float*)quadVertices.data(), quadVertices.size()*sizeof(Core::Vertex));
        size_t indexBuffer = Core::IndexBufferManager::CreateIndexBuffer(quadIndices, 6);
        quadVAO = Core::VertexArrayManager::CreateVertexArray();
        Core::VertexArrayManager::AddVertexBuffer(quadVAO, vertexBuffer);
        Core::VertexArrayManager::SetIndexBuffer(quadVAO, indexBuffer);
    }

    size_t AssetsManager::GetVAO(Primitive primitive) {
        switch (primitive) {
            case Quad:
                return quadVAO;
                break;
        }
        return -1;
    };

    size_t AssetsManager::GetShader(Shader shader) {
        switch (shader) {
            case Texture:
                return textureShader;
                break;
            case Intermediary:
                return intermediaryShader;
            case PostProcessing:
                return postProcessingShader;
                break;
        }
        std::cout << "[ERROR]: Unknown shader!\n";
        return -1;
    }

    size_t AssetsManager::CreatePlainRGBATexture(int width, int height, unsigned char fill) {
        unsigned char* data = new unsigned char[width * height * 4];
        for(int i = 0; i < width * height * 4; i++) data[i] = fill;

        size_t texture = Core::TextureManager::CreateTexture(GL_TEXTURE_2D, GL_RGBA8, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data, false, 0);

        delete[] data;
        return texture;
    }

    void AssetsManager::ReleaseAll() {
        Core::VertexBufferManager::ReleaseAll();
        Core::IndexBufferManager::ReleaseAll();
        Core::VertexBufferManager::ReleaseAll();
        Core::ShaderManager::ReleaseAll();
        Core::TextureManager::ReleaseAll();
        Core::RenderbufferManager::ReleaseAll();
        Core::FramebufferManager::ReleaseAll();
    }

    std::vector<SpriteInfo> SpritesManager::sprites;
    size_t SpritesManager::CreateSprite() {

        SpriteInfo spriteInfo;
        spriteInfo.vao = AssetsManager::GetVAO(AssetsManager::Quad);
        spriteInfo.shader = AssetsManager::GetShader(AssetsManager::Texture);

        sprites.push_back(spriteInfo);
        return sprites.size() - 1;
    }
    size_t SpritesManager::CreateSprite(size_t texture, glm::vec2 position, float rotation, glm::vec2 scale)
    {
        size_t sprite = CreateSprite();
        sprites.at(sprite).texture = texture;
        sprites.at(sprite).position = position;
        sprites.at(sprite).rotation = rotation;
        sprites.at(sprite).scale = scale;
        return sprite;
    }

    void SpritesManager::RenderSprites(Core::Camera &camera) {
        for (SpriteInfo spriteInfo : sprites) {
            size_t shader = spriteInfo.shader;
            Core::ShaderManager::setmat4(shader, "viewMat", camera.getViewMatrix());
            Core::ShaderManager::setmat4(shader, "projMat", camera.getProjectionMatrix());
            Core::ShaderManager::setmat4(
                shader,
                "modelMat",
                glm::rotate(
                    glm::scale(
                        glm::translate(
                            glm::mat4(1.0f),
                            glm::vec3(spriteInfo.position, 0.0f)
                        ),
                        glm::vec3(spriteInfo.scale,1.0f)
                    ),
                    glm::radians(spriteInfo.rotation),
                    {0.0f, 0.0f, 1.0f}));
            
            Core::TextureManager::Bind(spriteInfo.texture, 0);
            Core::ShaderManager::setInt(shader, "texUnit", 0);
            Core::ShaderManager::set3f(shader, "viewPos", camera.position);

            Core::RendererAPI::DrawIndexed(spriteInfo.vao);
        }
    }
}