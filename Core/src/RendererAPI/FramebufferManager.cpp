#include "RendererAPI/FramebufferManager.h"

namespace Core
{
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
}