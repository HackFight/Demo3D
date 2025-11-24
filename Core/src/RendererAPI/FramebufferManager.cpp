#include "RendererAPI/FramebufferManager.h"

//libs
#include <cstdint>
#include <glm/glm.hpp>

//std
#include <iostream>

//Core
#include "RendererAPI/TextureManager.h"
#include "Core/Application.h"

namespace Core
{
    std::vector<FramebufferInfo> FramebufferManager::framebuffers;
    uint32_t FramebufferManager::CreateFramebuffer()
    {
        FramebufferInfo framebuffer;
        glGenFramebuffers(1, &framebuffer.RendererID);

        framebuffers.push_back(framebuffer);
        return framebuffers.size() - 1;
    }

    void FramebufferManager::Bind(uint32_t framebuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[framebuffer].RendererID);
    }
    void FramebufferManager::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FramebufferManager::AttachTexture(uint32_t framebuffer, uint32_t texture)
    {
        FramebufferInfo framebufferInfo = framebuffers[framebuffer];
        TextureInfo textureInfo = TextureManager::GetTextureInfo(texture);

        if(framebufferInfo.Width == 0 && framebufferInfo.Height == 0)
		{
            framebuffers[framebuffer].Width = textureInfo.width;
            framebuffers[framebuffer].Height = textureInfo.height;
		}
        else
        {
            if (framebufferInfo.Width != textureInfo.width || framebufferInfo.Height != textureInfo.height)
            {
				std::cout << "Warning: Attaching texture of different size to framebuffer!\n";
            }
        }

		GLenum attachement;
        if(textureInfo.internalFormat == GL_RGB8 || textureInfo.internalFormat == GL_RGBA8 || textureInfo.internalFormat == GL_RGB16F || textureInfo.internalFormat == GL_RGBA16F)
            attachement = GL_COLOR_ATTACHMENT0;
        else if(textureInfo.internalFormat == GL_DEPTH_COMPONENT32F || textureInfo.internalFormat == GL_DEPTH_COMPONENT24)
            attachement = GL_DEPTH_ATTACHMENT;
        else if (textureInfo.internalFormat == GL_DEPTH24_STENCIL8)
            attachement = GL_DEPTH_STENCIL_ATTACHMENT;
        else
        {
            std::cout << "Error: Unsupported texture format for framebuffer attachment!\n";
            return;
		}

        Bind(framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, textureInfo.target, textureInfo.RendererID, 0);
        Unbind();
    }

    void FramebufferManager::AttachRenderbuffer(uint32_t framebuffer, uint32_t renderbuffer)
    {
        FramebufferInfo framebufferInfo = framebuffers[framebuffer];
        RenderbufferInfo renderbufferInfo = RenderbufferManager::GetRenderbufferInfo(renderbuffer);

        if(framebufferInfo.Width == 0 && framebufferInfo.Height == 0)
		{
            framebuffers[framebuffer].Width = renderbufferInfo.width;
            framebuffers[framebuffer].Height = renderbufferInfo.height;
		}
        else
        {
            if (framebufferInfo.Width != renderbufferInfo.width || framebufferInfo.Height != renderbufferInfo.height)
            {
				std::cout << "Warning: Attaching renderbuffer of different size to framebuffer!\n";
            }
        }

        GLenum attachement;
        if(renderbufferInfo.internalFormat == GL_RGB8 || renderbufferInfo.internalFormat == GL_RGBA8)
            attachement = GL_COLOR_ATTACHMENT0;
        else if(renderbufferInfo.internalFormat == GL_DEPTH_COMPONENT32F || renderbufferInfo.internalFormat == GL_DEPTH_COMPONENT24)
            attachement = GL_DEPTH_ATTACHMENT;
        else if (renderbufferInfo.internalFormat == GL_DEPTH24_STENCIL8)
            attachement = GL_DEPTH_STENCIL_ATTACHMENT;
        else
        {
            std::cout << "Error: Unsupported texture format for framebuffer attachment!\n";
            return;
		}

        Bind(framebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachement, GL_RENDERBUFFER, renderbufferInfo.RendererID);
        Unbind();
    }

    void FramebufferManager::Blit(uint32_t framebuffer)
    {
        FramebufferInfo info = framebuffers[framebuffer];

        glm::vec2 fbSize = Application::Get().GetFramebufferSize();
        if (info.Width != fbSize.x || info.Height != fbSize.y)
        {
            std::cout << "Warning: Blitting framebuffer to window of different size!\n";
		}

        glBindFramebuffer(GL_READ_FRAMEBUFFER, info.RendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, info.Width, info.Height, 0, 0, fbSize.x, fbSize.y, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }
    void FramebufferManager::Blit(uint32_t framebuffer, uint32_t destinationFramebuffer)
    {
        FramebufferInfo info = framebuffers[framebuffer];
        FramebufferInfo destInfo = framebuffers[destinationFramebuffer];

        if (info.Width != destInfo.Width || info.Height != destInfo.Height)
        {
            std::cout << "Warning: Blitting framebuffer to framebuffer of different size!\n";
		}

        glBindFramebuffer(GL_READ_FRAMEBUFFER, info.RendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destInfo.RendererID);
        glBlitFramebuffer(0, 0, info.Width, info.Height, 0, 0, destInfo.Width, destInfo.Height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }

    void FramebufferManager::ReleaseAll()
    {
        for(FramebufferInfo framebuffer : framebuffers)
        {
            if (framebuffer.RendererID != 0)
            {
                glDeleteFramebuffers(1, &framebuffer.RendererID);
                framebuffer.RendererID = 0;
            }
        }
    }



    std::vector<RenderbufferInfo> RenderbufferManager::renderbuffers;
    uint32_t RenderbufferManager::CreateRenderbuffer()
    {
        RenderbufferInfo renderbuffer;
        glGenRenderbuffers(1, &renderbuffer.RendererID);

        renderbuffers.push_back(renderbuffer);
        return renderbuffers.size() - 1;
    }
    uint32_t RenderbufferManager::CreateRenderbuffer(GLint internalFormat, GLsizei width, GLsizei height, bool multisampled, GLsizei samples)
    {
        uint32_t renderbuffer = CreateRenderbuffer();
        SetData(renderbuffer, internalFormat, width, height, multisampled, samples);
        return renderbuffer;
    }

    void RenderbufferManager::Bind(uint32_t renderbuffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[renderbuffer].RendererID);
    }
    void RenderbufferManager::Unbind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void RenderbufferManager::SetData(uint32_t renderbuffer, GLint internalFormat, GLsizei width, GLsizei height, bool multisampled, GLsizei samples)
    {
        renderbuffers[renderbuffer].internalFormat = internalFormat;
		renderbuffers[renderbuffer].width = width;
		renderbuffers[renderbuffer].height = height;
		renderbuffers[renderbuffer].multisampled = multisampled;
		renderbuffers[renderbuffer].samples = samples;

        RenderbufferInfo info = renderbuffers[renderbuffer];

        Bind(renderbuffer);
        if (info.multisampled)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, info.samples, info.internalFormat, info.width, info.height);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, info.internalFormat, info.width, info.height);
        }
    }

    void RenderbufferManager::Resize(uint32_t renderbuffer, GLsizei width, GLsizei height)
    {
        RenderbufferInfo info = renderbuffers[renderbuffer];
        SetData(renderbuffer, info.internalFormat, width, height, info.multisampled, info.samples);
    }

    void RenderbufferManager::ReleaseAll()
    {
        for (auto& renderbuffer : renderbuffers)
        {
            if (renderbuffer.RendererID != 0)
            {
                glDeleteRenderbuffers(1, &renderbuffer.RendererID);
                renderbuffer.RendererID = 0;
            }
        }
        renderbuffers.clear();
    }
}