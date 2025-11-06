#include "RendererAPI/Framebuffer.h"
#include "RendererAPI/Texture.h"

#include <glad/glad.h>
#include <memory>

namespace Core {
    Renderbuffer::Renderbuffer(AttachementType type, int width, int height)
        : type(type)
    {
        glGenRenderbuffers(1, &m_RendererID);
        SetData(width, height);
    }
    Renderbuffer::~Renderbuffer()
    {
        glDeleteRenderbuffers(1, &m_RendererID);
    }

    void Renderbuffer::Bind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
    }
    void Renderbuffer::Unbind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void Renderbuffer::SetData(int width, int height)
    {
        Bind();
        switch (type) {
        case Color:
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
            break;
        
        case Depth_Stencil:
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            break;
        }
    }

    std::shared_ptr<Renderbuffer> Renderbuffer::Create(AttachementType type, int width, int height)
    {
        return std::make_shared<Renderbuffer>(type, width, height);
    }



    Framebuffer::Framebuffer()
    {
        glGenFramebuffers(1, &m_RendererID);
    }
    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }
    void Framebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::AttachTexture(std::shared_ptr<Texture> texture)
    {
        Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetRendererID(), 0);
    }
    void Framebuffer::AttachTexture(AttachementType type, int width, int height)
    {
        Bind();
        std::shared_ptr<Texture> texture;
        switch (type)
        {
        case Color:
            texture = Texture::Create(width, height, GL_LINEAR, GL_RGB, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetRendererID(), 0);
            break;

        case Depth_Stencil:
            unsigned int textureD;
            glGenTextures(1, &textureD);
            glBindTexture(GL_TEXTURE_2D, textureD);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureD, 0);
            break;
        }
    }

    void Framebuffer::AttachRenderBuffer(std::shared_ptr<Renderbuffer> renderbuffer)
    {
        Bind();
        renderbuffer->Bind();
        switch (renderbuffer->GetType()) {
        case Color:
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer->GetRendererID());
            break;

        case Depth_Stencil:
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->GetRendererID());
            break;
        }  
    }

    std::shared_ptr<Framebuffer> Framebuffer::Create()
    {
        return std::make_shared<Framebuffer>();
    }
}