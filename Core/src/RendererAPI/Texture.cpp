#include <RendererAPI/Texture.h>

// libs
#include <glad/glad.h>
#include <stb_image/stb_image.h>

// std
#include <iostream>

namespace Core {

    Texture::Texture()
    {
        glGenTextures(1, &m_RendererID);
    }
    Texture::Texture(uint32_t target, uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType, const void* data, bool multisampled, int samples)
    {
        glGenTextures(1, &m_RendererID);
        SetData(target, internalFormat, width, height, format, dataType, data, multisampled, samples);
    }
    Texture::Texture(const char* filename, bool flip)
    {
        glGenTextures(1, &m_RendererID);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(flip);
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
        if (data == nullptr)
        {
            std::cout << "Failed to load texture\n";
        }

        if (nrChannels == 4)
            m_TextureInfo.internalFormat = m_TextureInfo.format = GL_RGBA;
        else if (nrChannels == 3)
            m_TextureInfo.internalFormat = m_TextureInfo.format = GL_RGB;
        else if (nrChannels == 1)
            m_TextureInfo.internalFormat = m_TextureInfo.format = GL_RED;

		SetData(GL_TEXTURE_2D, m_TextureInfo.internalFormat, width, height, m_TextureInfo.format, GL_UNSIGNED_BYTE, data, false, 0);
        stbi_image_free(data);
    }
    Texture::Texture(std::vector<const char*> faces)
    {
        glGenTextures(1, &m_RendererID);
        
        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            stbi_set_flip_vertically_on_load(false);
            unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
            if (data)
            {
				SetData(GL_TEXTURE_CUBE_MAP, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data, false, 0);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << "\n";
                stbi_image_free(data);
            }
        }

        m_TextureInfo.target = GL_TEXTURE_CUBE_MAP;
        m_TextureInfo.internalFormat = GL_RGB;
        m_TextureInfo.width = width;
		m_TextureInfo.height = height;
        m_TextureInfo.format = GL_RGB;
        m_TextureInfo.dataType = GL_UNSIGNED_BYTE;
		m_TextureInfo.multisampled = false;
		m_TextureInfo.samples = 0;

        glTexParameteri(m_TextureInfo.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(m_TextureInfo.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(m_TextureInfo.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_TextureInfo.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_TextureInfo.target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture::Bind(int i) const
    {
		glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(m_TextureInfo.target, m_RendererID);
    }
    void Texture::Unbind(uint32_t target)
    {
        glBindTexture(target, 0);
    }

    void Texture::SetData(uint32_t target, uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType,  const void* data, bool multisampled, int samples)
    {
        m_TextureInfo.target = target;
        m_TextureInfo.internalFormat = internalFormat;
        m_TextureInfo.format = format;
        m_TextureInfo.dataType = dataType;
        m_TextureInfo.width = width;
        m_TextureInfo.height = height;
        m_TextureInfo.multisampled = multisampled;
        m_TextureInfo.samples = samples;

        glBindTexture(m_TextureInfo.target, m_RendererID);
        
        if(m_TextureInfo.multisampled)
            glTexImage2DMultisample(m_TextureInfo.target, m_TextureInfo.samples, m_TextureInfo.internalFormat, m_TextureInfo.width, m_TextureInfo.height, GL_TRUE);
        else
            glTexImage2D(m_TextureInfo.target, 0, m_TextureInfo.internalFormat, m_TextureInfo.width, m_TextureInfo.height, 0, m_TextureInfo.format, m_TextureInfo.dataType, data);
    }

    void Texture::Resize(int width, int height)
    {
        glBindTexture(m_TextureInfo.target, m_RendererID);
		SetData(m_TextureInfo.target, m_TextureInfo.internalFormat, width, height, m_TextureInfo.format, m_TextureInfo.dataType, nullptr, m_TextureInfo.multisampled, m_TextureInfo.samples);
	}

    void Texture::GenerateMipmaps()
    {
        glBindTexture(m_TextureInfo.target, m_RendererID);
        glGenerateMipmap(m_TextureInfo.target);
	}

    void Texture::SetParameters(uint32_t wrapping, uint32_t minFilter, uint32_t maxFilter)
    {
        glBindTexture(m_TextureInfo.target, m_RendererID);
        glTexParameteri(m_TextureInfo.target, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(m_TextureInfo.target, GL_TEXTURE_WRAP_T, wrapping);
        glTexParameteri(m_TextureInfo.target, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(m_TextureInfo.target, GL_TEXTURE_MAG_FILTER, maxFilter);
    }

    void Texture::SetBorderColor(float r, float g, float b, float a)
    {
        glBindTexture(m_TextureInfo.target, m_RendererID);
        float color[] = { r, g, b, a };
		glTexParameterfv(m_TextureInfo.target, GL_TEXTURE_BORDER_COLOR, color);
    }
}