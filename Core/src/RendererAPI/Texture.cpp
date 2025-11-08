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
    Texture::Texture( uint32_t internalFormat, int width, int height, uint32_t param, uint32_t format, const void* data)
    {
        glGenTextures(1, &m_RendererID);
        SetData(internalFormat, width, height, param, format, data);
    }
    Texture::Texture(const char* filename, uint32_t internalFormat, uint32_t param, uint32_t format, bool flip)
    {
        glGenTextures(1, &m_RendererID);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(flip);
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
        if (data == nullptr)
        {
            std::cout << "Failed to load texture\n";
        }
        SetData(internalFormat, width, height, param, format, data);
	}
    Texture::Texture(std::vector<const char*> faces)
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

    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }
    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::SetData(uint32_t internalFormat, int width, int height, uint32_t param, uint32_t format, const void* data)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    std::shared_ptr<Texture> Texture::Create()
    {
        return std::make_shared<Texture>();
    }
    std::shared_ptr<Texture> Texture::Create( uint32_t internalFormat, int width, int height, uint32_t param, uint32_t format, const void* data)
    {
        return std::make_shared<Texture>(internalFormat, width, height, param, format, data);
    }
    std::shared_ptr<Texture> Texture::Create(const char* filename, uint32_t internalFormat, uint32_t param, uint32_t format)
    {
        return std::make_shared<Texture>(filename, internalFormat, param, format);
    }
    std::shared_ptr<Texture> Texture::CreateCubeMap(std::vector<const char*> faces)
    {
        return std::make_shared<Texture>(faces);
    }
}