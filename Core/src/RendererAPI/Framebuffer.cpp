#include "RendererAPI/Framebuffer.h"
#include "RendererAPI/Texture.h"

#include <glad/glad.h>
#include <memory>

namespace Core
{
    Renderbuffer::Renderbuffer(AttachementType type, int width, int height, bool multisampled, int samples)
        : type(type)
    {
        glGenRenderbuffers(1, &m_RendererID);
        SetData(width, height, multisampled, samples);
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

    void Renderbuffer::SetData(int width, int height, bool multisampled, int samples)
    {
        Bind();
        if (multisampled)
        {
            switch (type)
            {
            case Color:
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA, width, height);
                break;

            case Depth_Stencil:
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
                break;

            case Depth:
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
                break;
            }
        }
        else
        {
            switch (type)
            {
            case Color:
                glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
                break;

            case Depth_Stencil:
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
                break;

			case Depth:
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
				break;
            }
        }
    }

    std::shared_ptr<Renderbuffer> Renderbuffer::Create(AttachementType type, int width, int height, bool multisampled, int samples)
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

    void Framebuffer::AttachTexture(AttachementType type, std::shared_ptr<Texture> texture, bool multisampled)
    {
        Bind();
		uint32_t textureType = multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        switch (type)
        {
        case Color:
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType, texture->GetRendererID(), 0);
            break;

        case Depth_Stencil:
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, textureType, texture->GetRendererID(), 0);
            break;

        case Depth:
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureType, texture->GetRendererID(), 0);
            break;
        }
    }
    void Framebuffer::AttachTexture(AttachementType type, int width, int height, bool multisampled, int samples)
    {
        Bind();
        std::shared_ptr<Texture> texture;
        switch (type)
        {
        case Color:
            texture = Texture::Create(GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			texture->SetParameters(GL_REPEAT, GL_LINEAR, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetRendererID(), 0);
            break;

        case Depth_Stencil:
            unsigned int textureD;
            glGenTextures(1, &textureD);
            glBindTexture(GL_TEXTURE_2D, textureD);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureD, 0);
            break;

        case Depth:
            texture = Texture::Create(GL_DEPTH_COMPONENT, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			texture->SetParameters(GL_CLAMP_TO_BORDER, GL_NEAREST, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->GetRendererID(), 0);
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

        case Depth:
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->GetRendererID());
			break;
        }  
    }

    void Framebuffer::Blit(std::shared_ptr<Framebuffer> destination, int width, int height)
    {
        uint32_t drawID = destination ? destination->GetRendererID() : 0;
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawID);

        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	}

    std::shared_ptr<Framebuffer> Framebuffer::Create()
    {
        return std::make_shared<Framebuffer>();
    }
}