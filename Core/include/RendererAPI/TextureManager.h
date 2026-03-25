#pragma once

// libs
#include <glad/glad.h>

//std
#include <vector>

namespace Core
{
    struct TextureInfo
	{
        GLuint rendererID = 0;
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
        static size_t CreateTexture();
        static size_t CreateTexture(GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum dataType, const void* data, bool multisampled, GLsizei samples);
        static size_t CreateTexture(const char* filename, bool flip = false);
        static size_t CreateCubemap(std::vector<const char*> faces);

        static void Bind(size_t texture, int i);
        static void Unbind(size_t texture, int i);
        static void Unbind(int i);

        static void SetData(size_t texture, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum dataType, const void* data, bool multisampled, GLsizei samples);
        static void Resize(size_t texture, GLsizei width, GLsizei height);
        static void GenerateMipmaps(size_t texture);

        static void SetParameters(size_t texture, GLint wrapping, GLint minFilter, GLint maxFilter);
        static void SetBorderColor(size_t texture, float r, float g, float b, float a);

        static TextureInfo GetTextureInfo(size_t texture) { return textures[texture]; }
        static int GetTextureCount() { return textures.size(); }

        static void ReleaseAll();

        // Diagnostics
        static void DebugPrintSummary();                  // Print texture count and estimated bytes
        static size_t EstimateTotalMemoryBytes();         // Estimate memory used by tracked textures (approx.)
    private:
        static std::vector<TextureInfo> textures;
    };
}