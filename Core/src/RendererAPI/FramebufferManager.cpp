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
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers.at(framebuffer).rendererID);
    }
    void FramebufferManager::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FramebufferManager::AttachTexture(size_t framebuffer, size_t texture)
    {
        FramebufferInfo const framebufferInfo = framebuffers.at(framebuffer);
        TextureInfo const textureInfo = TextureManager::GetTextureInfo(texture);

        if(framebufferInfo.renderBuffer == -1) {
            framebuffers.at(framebuffer).width = textureInfo.width;
            framebuffers.at(framebuffer).height = textureInfo.height;
        } else {
            RenderbufferInfo renderbufferInfo = RenderbufferManager::GetRenderbufferInfo(framebufferInfo.renderBuffer);
            if(renderbufferInfo.width == textureInfo.width && renderbufferInfo.height == textureInfo.height) {
                framebuffers.at(framebuffer).width = textureInfo.width;
                framebuffers.at(framebuffer).height = textureInfo.height;
            } else {
                std::cout << "[ERROR] : Texture size is different from already bound render buffer size!\n";
            }
        }
        framebuffers.at(framebuffer).texture = texture;

		GLenum attachement;
        if(textureInfo.internalFormat == GL_RGB8 || textureInfo.internalFormat == GL_RGBA8 || textureInfo.internalFormat == GL_RGB16F || textureInfo.internalFormat == GL_RGBA16F)
            attachement = GL_COLOR_ATTACHMENT0;
        else if(textureInfo.internalFormat == GL_DEPTH_COMPONENT32F || textureInfo.internalFormat == GL_DEPTH_COMPONENT24)
            attachement = GL_DEPTH_ATTACHMENT;
        else if (textureInfo.internalFormat == GL_DEPTH24_STENCIL8)
            attachement = GL_DEPTH_STENCIL_ATTACHMENT;
        else
        {
            std::cout << "[ERROR] : Unsupported texture format for framebuffer attachment!\n- Texture format : " << textureInfo.internalFormat << "\n";
            return;
		}

        Bind(framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, textureInfo.target, textureInfo.rendererID, 0);
        Unbind();
    }

    void FramebufferManager::AttachRenderbuffer(size_t framebuffer, size_t renderbuffer)
    {
        FramebufferInfo const framebufferInfo = framebuffers.at(framebuffer);
        RenderbufferInfo const renderbufferInfo = RenderbufferManager::GetRenderbufferInfo(renderbuffer);

        if(framebufferInfo.texture == -1) {
            framebuffers.at(framebuffer).width = renderbufferInfo.width;
            framebuffers.at(framebuffer).height = renderbufferInfo.height;
        } else {
            TextureInfo textureInfo = TextureManager::GetTextureInfo(framebufferInfo.texture);
            if(renderbufferInfo.width == textureInfo.width && renderbufferInfo.height == textureInfo.height) {
                framebuffers.at(framebuffer).width = renderbufferInfo.width;
                framebuffers.at(framebuffer).height = renderbufferInfo.height;
            } else {
                std::cout << "[ERROR] : Render buffer size is different from already bound texture size!\n";
            }
        }
        framebuffers.at(framebuffer).renderBuffer = renderbuffer;

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

    void FramebufferManager::Resize(size_t framebuffer, GLsizei width, GLsizei height) {
        FramebufferInfo const framebufferInfo = framebuffers.at(framebuffer);

        if(framebufferInfo.texture != -1) {
            TextureManager::Resize(framebufferInfo.texture, width, height);
        }
        if (framebufferInfo.renderBuffer != -1) {
            RenderbufferManager::Resize(framebufferInfo.renderBuffer, width, height);
        }

        framebuffers.at(framebuffer).width = width;
        framebuffers.at(framebuffer).height = height;
    }

    void FramebufferManager::Blit(size_t framebuffer)
    {
        FramebufferInfo const framebufferInfo = framebuffers.at(framebuffer);

        glm::vec2 fbSize = Application::Get().GetFramebufferSize();
        if (framebufferInfo.width != fbSize.x || framebufferInfo.height != fbSize.y)
        {
            std::cout << "Warning: Blitting framebuffer to window of different size!\nFramebuffer " << framebuffer << " : (" << framebufferInfo.width << ", " << framebufferInfo.height
            << ")\nViewport : (" << fbSize.x << ", " << fbSize.y << ")\n";
		}

        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferInfo.rendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, framebufferInfo.width, framebufferInfo.height, 0, 0, fbSize.x, fbSize.y, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }
    void FramebufferManager::Blit(size_t framebuffer, size_t destinationFramebuffer)
    {
        FramebufferInfo info = framebuffers.at(framebuffer);
        FramebufferInfo destInfo = framebuffers.at(destinationFramebuffer);

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
        RenderbufferInfo renderbufferInfo;
        glGenRenderbuffers(1, &renderbufferInfo.rendererID);

        renderbuffers.push_back(renderbufferInfo);
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
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers.at(renderbuffer).rendererID);
    }
    void RenderbufferManager::Unbind()
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void RenderbufferManager::SetData(size_t renderbuffer, GLint internalFormat, GLsizei width, GLsizei height, bool multisampled, GLsizei samples)
    {
        renderbuffers.at(renderbuffer).internalFormat = internalFormat;
		renderbuffers.at(renderbuffer).width = width;
		renderbuffers.at(renderbuffer).height = height;
		renderbuffers.at(renderbuffer).multisampled = multisampled;
		renderbuffers.at(renderbuffer).samples = samples;

        RenderbufferInfo const renderbufferInfo = renderbuffers.at(renderbuffer);

        Bind(renderbuffer);
        if (renderbufferInfo.multisampled)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, renderbufferInfo.samples, renderbufferInfo.internalFormat, renderbufferInfo.width, renderbufferInfo.height);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, renderbufferInfo.internalFormat, renderbufferInfo.width, renderbufferInfo.height);
        }
    }

    void RenderbufferManager::Resize(size_t renderbuffer, GLsizei width, GLsizei height)
    {
        RenderbufferInfo const renderbufferInfo = renderbuffers.at(renderbuffer);
        SetData(renderbuffer, renderbufferInfo.internalFormat, width, height, renderbufferInfo.multisampled, renderbufferInfo.samples);
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