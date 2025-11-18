#pragma once

//libs
#include <glad/glad.h>
#include <stdint.h>
#include <vector>

namespace Core
{
    struct FramebufferInfo
    {
        GLuint RendererID = 0;
        GLsizei Width = 0;
        GLsizei Height = 0;
    };

    class FramebufferManager
    {
    public:
        static uint32_t CreateFramebuffer();

        static void Bind(uint32_t framebuffer);
        static void Unbind();

        static void AttachTexture(uint32_t framebuffer, uint32_t texture);
        static void AttachRenderbuffer(uint32_t framebuffer, uint32_t renderbuffer);

        static void Blit(uint32_t framebuffer);
        static void Blit(uint32_t framebuffer, uint32_t destinationFramebuffer);

        static void ReleaseAll();

    private:
        static std::vector<FramebufferInfo> framebuffers;
    };

    struct RenderbufferInfo
    {
        GLuint RendererID = 0;
        GLint internalFormat = 0;
		GLsizei width = 0;
		GLsizei height = 0;
		bool multisampled = false;
		GLsizei samples = 0;
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

        static RenderbufferInfo GetRenderbufferInfo(uint32_t renderbuffer) { return renderbuffers[renderbuffer]; }

        static void ReleaseAll();

    private:
        static std::vector<RenderbufferInfo> renderbuffers;
    };
}