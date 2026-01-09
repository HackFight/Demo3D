#pragma once

namespace Platformer {

    class ResourceManager {
    public:
        enum Primitive {
            Quad,
        };
        enum Shader {
            Texture,
        };

        static void Init();
        static unsigned int GetVAO(Primitive primitive);
        static unsigned int GetShader(Shader shader);
        static unsigned int CreatePlainRGBATexture(int width, int height, unsigned char fill = 255);
        static void ReleaseAll();

    private:
        static unsigned int quadVAO;
        static unsigned int textureShader;
    };
}