#pragma once

// libs
#include <glad/glad.h>

//std
#include <vector>
#include <cstdint>

namespace Core
{
    struct TextureInfo
	{
        GLuint RendererID = 0;
		GLenum target = 0;
		GLint internalFormat = 0;
		GLsizei width = 0;
		GLsizei height = 0;
		GLenum format = 0;
        GLenum dataType = 0;
		bool multisampled = false;
		GLsizei samples = 0;
	};

    class TextureManager
    {
    public:
        static uint32_t CreateTexture();
        static uint32_t CreateTexture(GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum dataType, const void* data, bool multisampled, GLsizei samples);
        static uint32_t CreateTexture(const char* filename, bool flip = false);
        static uint32_t CreateCubemap(std::vector<const char*> faces);

        static void Bind(uint32_t texture, int i);
        static void Unbind(uint32_t texture);

        static void SetData(uint32_t texture, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum dataType, const void* data, bool multisampled, GLsizei samples);
        static void Resize(uint32_t texture, GLsizei width, GLsizei height);
        static void GenerateMipmaps(uint32_t texture);

        static void SetParameters(uint32_t texture, GLint wrapping, GLint minFilter, GLint maxFilter);
        static void SetBorderColor(uint32_t texture, float r, float g, float b, float a);

        static TextureInfo GetTextureInfo(uint32_t texture) { return textures[texture]; }

        static void ReleaseAll();

    private:
        static std::vector<TextureInfo> textures;
    };
}