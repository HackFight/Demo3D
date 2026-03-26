#pragma once

//libs
#include <cstddef>

namespace Platformer {

    class ResourceManager {
    public:
        enum Primitive {
            Quad
        };
        enum Shader {
            Texture,
            PostProcessing
        };

        static void Init();
        static size_t GetVAO(Primitive primitive);
        static size_t GetShader(Shader shader);
        static size_t CreatePlainRGBATexture(int width, int height, unsigned char fill = 255);
        static void ReleaseAll();

    private:
        static size_t quadVAO;
        static size_t textureShader, postProcessingShader;
    };
}