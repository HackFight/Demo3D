#include <Renderer/Texture.h>

#include <cstdint>
#include <glad/glad.h>
#include <memory>

namespace Core {

    Texture::Texture()
    {
        glGenTextures(1, &m_RendererID);
    }
    Texture::Texture(int width, int height, uint32_t param, uint32_t format, const void* data)
    {
        SetData(width, height, param, format, data);
    }
    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE0, m_RendererID);
    }
    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE0, 0);
    }

    void Texture::SetData(int width, int height, uint32_t param, uint32_t format, const void* data)
    {
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
    }

    std::shared_ptr<Texture> Texture::Create()
    {
        return std::make_shared<Texture>();
    }
    std::shared_ptr<Texture> Texture::Create(int width, int height, uint32_t param, uint32_t format, const void* data)
    {
        return std::make_shared<Texture>(width, height, param, format, data);
    }
}