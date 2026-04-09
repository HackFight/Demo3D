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
#include <vector>

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

    size_t AssetsManager::quadVBO, AssetsManager::quadIndexBuffer, AssetsManager::quadVAO;
    size_t AssetsManager::layersVaos[];
    size_t AssetsManager::textureShader, AssetsManager::postProcessingShader, AssetsManager::intermediaryShader;

    void AssetsManager::Init() {
        //##### Shaders ######
        textureShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert",  RESOURCES_PATH "shaders/texture.frag");
        intermediaryShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/layer.vert",  RESOURCES_PATH "shaders/texture.frag");
        postProcessingShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/layer.vert",  RESOURCES_PATH "shaders/post.frag");

        //##### VAOs #####
        //Quad
        quadVBO = Core::VertexBufferManager::CreateVertexBuffer((float*)quadVertices.data(), quadVertices.size()*sizeof(Core::Vertex));
        quadIndexBuffer = Core::IndexBufferManager::CreateIndexBuffer(quadIndices, 6);
        quadVAO = Core::VertexArrayManager::CreateVertexArray();
        Core::VertexArrayManager::AddVertexBuffer(quadVAO, quadVBO);
        Core::VertexArrayManager::SetIndexBuffer(quadVAO, quadIndexBuffer);

        //Layers
        for(int i=0; i < PIXEL_SIZE*PIXEL_SIZE; i++) {
            layersVaos[i] = CreateLayerVao(i);
        }
    }

    size_t AssetsManager::GetVAO(Primitive primitive) {
        switch (primitive) {
            case QuadPrimitive:
                return quadVAO;
                break;
        }
        return -1;
    };
    size_t AssetsManager::CreateLayerVao(int index) {
        float sizeRatio = 1.0f/PIXEL_SIZE;
        float x = (int)(index/PIXEL_SIZE);
        float y = index%PIXEL_SIZE;
        glm::vec2 offset = {x, y};
        std::vector<Core::Vertex> vertices {
            {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, glm::vec2{0.0f,       0.0f} + offset,      {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ 1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, glm::vec2{sizeRatio,  0.0f} + offset,      {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{ 1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, glm::vec2{sizeRatio,  sizeRatio} + offset, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, glm::vec2{0.0f,       sizeRatio} + offset, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}
        };
        size_t vertexBuffer = Core::VertexBufferManager::CreateVertexBuffer((float*)vertices.data(), vertices.size()*sizeof(Core::Vertex));
        size_t res = Core::VertexArrayManager::CreateVertexArray();
        Core::VertexArrayManager::AddVertexBuffer(res, vertexBuffer);
        Core::VertexArrayManager::SetIndexBuffer(res, quadIndexBuffer);
        return res;
    }
    size_t AssetsManager::GetLayerVAO(int index) {
        if (index >= 0 && index < PIXEL_SIZE*PIXEL_SIZE) {
            return layersVaos[index];
        } else { return -1; }
    }

    size_t AssetsManager::GetShader(Shader shader) {
        switch (shader) {
            case TextureShader:
                return textureShader;
                break;
            case IntermediaryShader:
                return intermediaryShader;
            case PostProcessingShader:
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
        spriteInfo.vao = AssetsManager::GetVAO(AssetsManager::QuadPrimitive);
        spriteInfo.shader = AssetsManager::GetShader(AssetsManager::TextureShader);

        sprites.push_back(spriteInfo);
        return sprites.size() - 1;
    }
    size_t SpritesManager::CreateSprite(size_t texture, size_t vao, size_t shader, glm::vec2 position, float rotation, glm::vec2 scale)
    {
        size_t sprite = CreateSprite();
        sprites.at(sprite).vao = vao;
        sprites.at(sprite).shader = shader;
        sprites.at(sprite).texture = texture;
        sprites.at(sprite).position = position;
        sprites.at(sprite).rotation = rotation;
        sprites.at(sprite).scale = scale;
        return sprite;
    }
    size_t SpritesManager::CreateSprite(size_t texture, glm::vec2 position, float rotation, glm::vec2 scale)
    {
        return CreateSprite(texture,
            AssetsManager::GetVAO(AssetsManager::QuadPrimitive),
            AssetsManager::GetShader(AssetsManager::TextureShader),
            position,
            rotation,
            scale);
    }

    void SpritesManager::RenderSprite(Core::Camera &camera, size_t sprite) {
        SpriteInfo spriteInfo = sprites.at(sprite);
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
    void SpritesManager::RenderSprites(Core::Camera &camera) {
        for (size_t sprite = 0; sprite < sprites.size(); sprite++) {
            RenderSprite(camera, sprite);
        }
    }
    void SpritesManager::RenderSprites(Core::Camera &camera, std::vector<size_t> sprites) {
        for(size_t sprite : sprites) {
            RenderSprite(camera, sprite);
        }
    }
}