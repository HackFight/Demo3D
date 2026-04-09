#pragma once

//libs
#include "Renderer/Camera.h"
#include <cstddef>
#include <vector>

namespace Platformer {

    const int PIXEL_SIZE = 4;

    class AssetsManager {
    public:
        enum Primitive {
            QuadPrimitive
        };
        enum Shader {
            TextureShader,
            IntermediaryShader,
            PostProcessingShader
        };

        static void Init();
        static size_t GetVAO(Primitive primitive);
        static size_t GetLayerVAO(int index);
        static size_t GetShader(Shader shader);
        static size_t CreatePlainRGBATexture(int width, int height, unsigned char fill = 255);
        static void ReleaseAll();
        
        private:
        static size_t CreateLayerVao(int index);
        static size_t layersVaos[PIXEL_SIZE*PIXEL_SIZE];
        static size_t quadVBO, quadIndexBuffer, quadVAO;
        static size_t textureShader, intermediaryShader, postProcessingShader;
    };

    struct SpriteInfo {
        size_t vao = -1;
        size_t shader = -1;
        size_t texture = -1;
        glm::vec2 position = {0.0f, 0.0f};
        glm::vec2 scale = {0.5f, 0.5f};
        float rotation = 0.0f;
    };

    class SpritesManager {
    public:
        static size_t CreateSprite();
        static size_t CreateSprite(size_t texture, size_t vao, size_t shader, glm::vec2 position = {0.0f, 0.0f}, float rotation = 0.0f, glm::vec2 scale = {0.5f, 0.5f});
        static size_t CreateSprite(size_t texture, glm::vec2 position = {0.0f, 0.0f}, float rotation = 0.0f, glm::vec2 scale = {0.5f, 0.5f});

        static void RenderSprite(Core::Camera& camera, size_t sprite);
        static void RenderSprites(Core::Camera& camera);
        static void RenderSprites(Core::Camera& camera, std::vector<size_t> sprites);

        static size_t GetSpritesCount() { return sprites.size(); }
        
        static std::vector<SpriteInfo> sprites;
    };
}