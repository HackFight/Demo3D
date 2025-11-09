#include <RendererAPI/Texture.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <cstdint>
#include <memory>
#include <iostream>

namespace Core {

    Texture::Texture()
		: textureType(GL_TEXTURE_2D), multisampled(false), samples(4)
    {
        glGenTextures(1, &m_RendererID);
    }
    Texture::Texture( uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType, const void* data, bool multisampled, int samples)
		: multisampled(multisampled), samples(samples)
    {
		multisampled ? textureType = GL_TEXTURE_2D_MULTISAMPLE : textureType = GL_TEXTURE_2D;
        glGenTextures(1, &m_RendererID);
        SetData(internalFormat, width, height, format, dataType, data, multisampled, samples);
    }
    Texture::Texture(const char* filename, uint32_t internalFormat, uint32_t format, bool flip)
		: textureType(GL_TEXTURE_2D), multisampled(false), samples(4)
    {
        glGenTextures(1, &m_RendererID);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(flip);
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
        if (data == nullptr)
        {
            std::cout << "Failed to load texture\n";
        }
        SetData(internalFormat, width, height, format, GL_UNSIGNED_BYTE, data);
	}
    Texture::Texture(std::vector<const char*> faces)
		: textureType(GL_TEXTURE_CUBE_MAP), multisampled(false), samples(4)
    {
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
        
        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            stbi_set_flip_vertically_on_load(false);
            unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << "\n";
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    Texture::~Texture()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture::Bind(int i) const
    {
		glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(textureType, m_RendererID);
    }
    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::SetData(uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType,  const void* data, bool multisampled, int samples)
    {
        glActiveTexture(GL_TEXTURE0);
        if (multisampled)
        {
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_TRUE);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, data);
        }
    }

    void Texture::GenerateMipmaps()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glGenerateMipmap(GL_TEXTURE_2D);
	}

    void Texture::SetParameters(uint32_t wrapping, uint32_t minFilter, uint32_t maxFilter)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
    }

    void Texture::SetBorderColor(float r, float g, float b, float a)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        float color[] = { r, g, b, a };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    }

    std::shared_ptr<Texture> Texture::Create()
    {
        return std::make_shared<Texture>();
    }
    std::shared_ptr<Texture> Texture::Create(uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType, const void* data, bool multisampled, int samples)
    {
        std::shared_ptr<Texture> temp = std::make_shared<Texture>(internalFormat, width, height, format, dataType, data, multisampled, samples);
		if (!multisampled)
            temp->SetParameters(GL_REPEAT, GL_LINEAR, GL_LINEAR);
		return temp;
    }
    std::shared_ptr<Texture> Texture::Create(const char* filename, uint32_t internalFormat, uint32_t format, bool flip)
    {
        std::shared_ptr<Texture> temp = std::make_shared<Texture>(filename, internalFormat, format, flip);
        temp->SetParameters(GL_REPEAT, GL_LINEAR, GL_LINEAR);
        return temp;
    }
    std::shared_ptr<Texture> Texture::CreateCubemap(std::vector<const char*> faces)
    {
        return std::make_shared<Texture>(faces);
    }
}