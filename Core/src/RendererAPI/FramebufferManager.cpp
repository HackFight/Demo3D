#include "RendererAPI/FramebufferManager.h"

//libs
#include <glm/glm.hpp>

//std
#include <iostream>

//Core
#include "RendererAPI/TextureManager.h"
#include "Core/Application.h"

namespace Core
{
    std::vector<FramebufferInfo> FramebufferManager::framebuffers;
    size_t FramebufferManager::CreateFramebuffer()
    {
        FramebufferInfo framebuffer;
        glGenFramebuffers(1, &framebuffer.rendererID);

        framebuffers.push_back(framebuffer);
        return framebuffers.size() - 1;
    }

    void FramebufferManager::Bind(size_t framebuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[framebuffer].rendererID);
    }
    void FramebufferManager::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FramebufferManager::AttachTexture(size_t framebuffer, size_t texture)
    {
        FramebufferInfo framebufferInfo = framebuffers[framebuffer];
        TextureInfo textureInfo = TextureManager::GetTextureInfo(texture);

        if(framebufferInfo.width == 0 && framebufferInfo.height == 0)
		{
            framebuffers[framebuffer].width = textureInfo.width;
            framebuffers[framebuffer].height = textureInfo.height;
		}
        else
        {
            if (framebufferInfo.width != textureInfo.width || framebufferInfo.height != textureInfo.height)
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
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, textureInfo.target, textureInfo.rendererID, 0);
        Unbind();
    }

    void FramebufferManager::AttachRenderbuffer(size_t framebuffer, size_t renderbuffer)
    {
        FramebufferInfo framebufferInfo = framebuffers[framebuffer];
        RenderbufferInfo renderbufferInfo = RenderbufferManager::GetRenderbufferInfo(renderbuffer);

        if(framebufferInfo.width == 0 && framebufferInfo.height == 0)
		{
            framebuffers[framebuffer].width = renderbufferInfo.width;
            framebuffers[framebuffer].height = renderbufferInfo.height;
		}
        else
        {
            if (framebufferInfo.width != renderbufferInfo.width || framebufferInfo.height != renderbufferInfo.height)
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
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachement, GL_RENDERBUFFER, renderbufferInfo.rendererID);
        Unbind();
    }

    void FramebufferManager::Blit(size_t framebuffer)
    {
        FramebufferInfo info = framebuffers[framebuffer];

        glm::vec2 fbSize = Application::Get().GetFramebufferSize();
        if (info.width != fbSize.x || info.height != fbSize.y)
        {
            std::cout << "Warning: Blitting framebuffer to window of different size!\n";
		}

        glBindFramebuffer(GL_READ_FRAMEBUFFER, info.rendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, info.width, info.height, 0, 0, fbSize.x, fbSize.y, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }
    void FramebufferManager::Blit(size_t framebuffer, size_t destinationFramebuffer)
    {
        FramebufferInfo info = framebuffers[framebuffer];
        FramebufferInfo destInfo = framebuffers[destinationFramebuffer];

        if (info.width != destInfo.width || info.height != destInfo.height)
        {
            std::cout << "Warning: Blitting framebuffer to framebuffer of different size!\n";
		}

        glBindFramebuffer(GL_READ_FRAMEBUFFER, info.rendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destInfo.rendererID);
        glBlitFramebuffer(0, 0, info.width, info.height, 0, 0, destInfo.width, destInfo.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }

    void FramebufferManager::ReleaseAll()
    {
        for(FramebufferInfo framebuffer : framebuffers)
        {
            if (framebuffer.rendererID != 0)
            {
                glDeleteFramebuffers(1, &framebuffer.rendererID);
                framebuffer.rendererID = 0;
            }
        }
    }



    std::vector<RenderbufferInfo> RenderbufferManager::renderbuffers;
    size_t RenderbufferManager::CreateRenderbuffer()
    {
        RenderbufferInfo renderbuffer;
        glGenRenderbuffers(1, &renderbuffer.rendererID);

        renderbuffers.push_back(renderbuffer);
        return renderbuffers.size() - 1;
    }
    size_t RenderbufferManager::CreateRenderbuffer(GLint internalFormat, GLsizei width, GLsizei height, bool multisampled, GLsizei samples)
    {
        size_t renderbuffer = CreateRenderbuffer();
        SetData(renderbuffer, internalFormat, width, height, multisampled, samples);
        return renderbuffer;
    }

    void RenderbufferManager::Bind(size_t renderbuffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[renderbuffer].rendererID);
    }
    void RenderbufferManager::Unbind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void RenderbufferManager::SetData(size_t renderbuffer, GLint internalFormat, GLsizei width, GLsizei height, bool multisampled, GLsizei samples)
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

    void RenderbufferManager::Resize(size_t renderbuffer, GLsizei width, GLsizei height)
    {
        RenderbufferInfo info = renderbuffers[renderbuffer];
        SetData(renderbuffer, info.internalFormat, width, height, info.multisampled, info.samples);
    }

    void RenderbufferManager::ReleaseAll()
    {
        for (auto& renderbuffer : renderbuffers)
        {
            if (renderbuffer.rendererID != 0)
            {
                glDeleteRenderbuffers(1, &renderbuffer.rendererID);
                renderbuffer.rendererID = 0;
            }
        }
        renderbuffers.clear();
    }
}