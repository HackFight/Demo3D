#pragma once

//libs
#include <glad/glad.h>
#include <vector>

namespace Core
{
    struct FramebufferInfo
    {
        GLuint rendererID = 0;
        GLsizei width = 0;
        GLsizei height = 0;
        size_t texture = -1;
        size_t renderBuffer = -1;
    };

    class FramebufferManager
    {
    public:
        static size_t CreateFramebuffer();

        static void Bind(size_t framebuffer);
        static void Unbind();

        static void AttachTexture(size_t framebuffer, size_t texture);
        static void AttachRenderbuffer(size_t framebuffer, size_t renderbuffer);
        static void Resize(size_t framebuffer, GLsizei width, GLsizei height);

        static void Blit(size_t framebuffer);
        static void Blit(size_t framebuffer, size_t destinationFramebuffer);

        static FramebufferInfo GetFramebufferInfo(size_t framebuffer) { return framebuffers.at(framebuffer); }

        static void ReleaseAll();

    private:
        static std::vector<FramebufferInfo> framebuffers;
    };

    struct RenderbufferInfo
    {
        GLuint rendererID = 0;
        GLint internalFormat = 0;
		GLsizei width = 0;
		GLsizei height = 0;
		bool multisampled = false;
		GLsizei samples = 0;
    };

    class RenderbufferManager
    {
    public:
        static size_t CreateRenderbuffer();
        static size_t CreateRenderbuffer(GLint internalFormat, GLsizei width, GLsizei height, bool multisampled, GLsizei samples);

        static void Bind(size_t renderbuffer);
        static void Unbind();

        static void SetData(size_t renderbuffer, GLint internalFormat, GLsizei width, GLsizei height, bool multisampled, GLsizei samples);
        static void Resize(size_t renderbuffer, GLsizei width, GLsizei height);

        static RenderbufferInfo GetRenderbufferInfo(size_t renderbuffer) { return renderbuffers.at(renderbuffer); }

        static void ReleaseAll();

    private:
        static std::vector<RenderbufferInfo> renderbuffers;
    };
}