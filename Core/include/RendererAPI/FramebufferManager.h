#pragma once

//libs
#include <glad/glad.h>
#include <stdint.h>
#include <vector>

namespace Core
{
    struct FramebufferInfo
    {
        GLuint RendererID;
        GLsizei Width;
        GLsizei Height;
    };

    class FramebufferManager
    {
    public:
        static uint32_t CreateFramebuffer();

        static void Bind(uint32_t framebuffer);
        static void Unbind();

        static void AttachTexture(uint32_t framebuffer, uint32_t texture);
        static void AttachRenderbuffer(uint32_t framebuffer, uint32_t renderbuffer);

        static void Blit(uint32_t framebuffer, uint32_t destinationFramebuffer);

        static void ReleaseAll();

    private:
        static std::vector<FramebufferInfo> framebuffers;
    };

    struct RenderbufferInfo
    {
        GLuint RendererID;
        GLint internalFormat;
		GLsizei width;
		GLsizei height;
		bool multisampled;
		GLsizei samples;
    };

    class RenderbufferManager
    {
    public:
        static uint32_t CreateRenderbuffer();
        static uint32_t CreateRenderbuffer(GLint internalFormat, GLsizei width, GLsizei height, bool multisampled, GLsizei samples);

        static void Bind(uint32_t renderbuffer);
        static void Unbind();

        static void SetData(uint32_t renderbuffer, GLint internalFormat, GLsizei width, GLsizei height, bool multisampled, GLsizei samples);
        static void Resize(uint32_t renderbuffer, GLsizei width, GLsizei height);

    private:
        static std::vector<RenderbufferInfo> renderbuffers;
    };
}