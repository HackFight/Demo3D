#pragma once

//libs
#include "Renderer/Camera.h"
#include <cstddef>
#include <vector>

namespace Platformer {

    class AssetsManager {
    public:
        enum Primitive {
            Quad
        };
        enum Shader {
            Texture,
            Intermediary,
            PostProcessing
        };

        static void Init();
        static size_t GetVAO(Primitive primitive);
        static size_t GetShader(Shader shader);
        static size_t CreatePlainRGBATexture(int width, int height, unsigned char fill = 255);
        static void ReleaseAll();

    private:
        static size_t quadVAO;
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
        static size_t CreateSprite(size_t texture, glm::vec2 position = {0.0f, 0.0f}, float rotation = 0.0f, glm::vec2 scale = {0.5f, 0.5f});

        static void RenderSprites(Core::Camera& camera);

        static size_t GetSpritesCount() { return sprites.size(); }
        
        static std::vector<SpriteInfo> sprites;
    };
}