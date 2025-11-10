#include "RendererAPI/Framebuffer.h"
#include "Core/Application.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

namespace Core
{
    Renderbuffer::Renderbuffer()
    {
        glGenRenderbuffers(1, &m_RendererID);
	}
    Renderbuffer::Renderbuffer(uint32_t internalFormat, int width, int height, bool multisampled, int samples)
    {
        glGenRenderbuffers(1, &m_RendererID);
        SetData(internalFormat, width, height, multisampled, samples);
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

    void Renderbuffer::SetData(uint32_t internalFormat, int width, int height, bool multisampled, int samples)
    {
		m_RenderbufferInfo.internalFormat = internalFormat;
		m_RenderbufferInfo.width = width;
		m_RenderbufferInfo.height = height;
		m_RenderbufferInfo.multisampled = multisampled;
		m_RenderbufferInfo.samples = samples;

        Bind();
        if (m_RenderbufferInfo.multisampled)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_RenderbufferInfo.samples, m_RenderbufferInfo.internalFormat, m_RenderbufferInfo.width, m_RenderbufferInfo.height);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, m_RenderbufferInfo.internalFormat, m_RenderbufferInfo.width, m_RenderbufferInfo.height);
        }
    }

    void Renderbuffer::Resize(int width, int height)
    {
        SetData(m_RenderbufferInfo.internalFormat, width, height, m_RenderbufferInfo.multisampled, m_RenderbufferInfo.samples);
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

    void Framebuffer::AttachTexture(Texture texture)
    {
        if(m_Width == 0 && m_Height == 0)
		{
            m_Width = texture.GetTextureInfo().width;
            m_Height = texture.GetTextureInfo().height;
		}
        else
        {
            if (m_Width != texture.GetTextureInfo().width || m_Height != texture.GetTextureInfo().height)
            {
				std::cout << "Warning: Attaching texture of different size to framebuffer!\n";
            }
        }

		uint32_t textureInternalFormat = texture.GetTextureInfo().internalFormat;
		uint32_t attachement;

        if(textureInternalFormat == GL_RGB8 || textureInternalFormat == GL_RGBA8)
            attachement = GL_COLOR_ATTACHMENT0;
        else if(textureInternalFormat == GL_DEPTH_COMPONENT32F || textureInternalFormat == GL_DEPTH_COMPONENT24)
            attachement = GL_DEPTH_ATTACHMENT;
        else if (textureInternalFormat == GL_DEPTH24_STENCIL8)
            attachement = GL_DEPTH_STENCIL_ATTACHMENT;
        else
        {
            std::cout << "Error: Unsupported texture format for framebuffer attachment!\n";
            return;
		}

        Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, texture.GetTextureInfo().target, texture.GetRendererID(), 0);
    }

    void Framebuffer::AttachRenderBuffer(Renderbuffer renderbuffer)
    {
        if (m_Width == 0 && m_Height == 0)
        {
            m_Width = renderbuffer.GetRenderbufferInfo().width;
            m_Height = renderbuffer.GetRenderbufferInfo().height;
        }
        else
        {
            if (m_Width != renderbuffer.GetRenderbufferInfo().width || m_Height != renderbuffer.GetRenderbufferInfo().height)
			{
                std::cout << "Warning: Attaching renderbuffer of different size to framebuffer!\n";
            }
		}

		uint32_t renderBufferInternalFormat = renderbuffer.GetRenderbufferInfo().internalFormat;
        uint32_t attachement;

        if (renderBufferInternalFormat == GL_RGB8 || renderBufferInternalFormat == GL_RGBA8)
            attachement = GL_COLOR_ATTACHMENT0;
        else if (renderBufferInternalFormat == GL_DEPTH_COMPONENT32F || renderBufferInternalFormat == GL_DEPTH_COMPONENT24)
            attachement = GL_DEPTH_ATTACHMENT;
        else if (renderBufferInternalFormat == GL_DEPTH24_STENCIL8)
            attachement = GL_DEPTH_STENCIL_ATTACHMENT;
        else
        {
            std::cout << "Error: Unsupported renderbuffer format for framebuffer attachment!\n";
			return;
        }

		Bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachement, GL_RENDERBUFFER, renderbuffer.GetRendererID());
    }

    void Framebuffer::Blit()
    {
		glm::vec2 fbSize = Application::Get().GetFramebufferSize();
        if (m_Width != fbSize.x || m_Height != fbSize.y)
        {
            std::cout << "Warning: Blitting framebuffer to window of different size!\n";
		}

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	}

    void Framebuffer::Blit(Framebuffer destination)
    {
        if (m_Width != destination.m_Width || m_Height != destination.m_Height)
        {
            std::cout << "Warning: Blitting between framebuffers of different sizes!\n";
        }

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination.m_RendererID);

        glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }
}