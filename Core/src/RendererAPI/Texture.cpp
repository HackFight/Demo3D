#include <RendererAPI/Texture.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <cstdint>
#include <memory>
#include <iostream>

namespace Core {

    Texture::Texture()
    {
        glGenTextures(1, &m_RendererID);
    }
    Texture::Texture(int width, int height, uint32_t param, uint32_t format, const void* data)
    {
        glGenTextures(1, &m_RendererID);
        SetData(width, height, param, format, data);
    }
    Texture::Texture(const char* filename, uint32_t param, uint32_t format)
    {
        glGenTextures(1, &m_RendererID);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
        if (data == nullptr)
        {
            std::cout << "Failed to load texture\n";
        }
        SetData(width, height, param, format, data);
	}
    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }
    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::SetData(int width, int height, uint32_t param, uint32_t format, const void* data)
    {
        glActiveTexture(GL_TEXTURE0);
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