#include "RendererAPI/TextureManager.h"

// libs
#include <cstdint>
#include <stb_image/stb_image.h>

// std
#include <iostream>

namespace Core
{
    std::vector<TextureInfo> TextureManager::textures;
    uint32_t TextureManager::CreateTexture()
    {
        TextureInfo texture;

        glGenTextures(1, &texture.RendererID);

        textures.push_back(texture);
        return textures.size() - 1;
    }
    uint32_t TextureManager::CreateTexture(GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum dataType, const void* data, bool multisampled, GLsizei samples)
    {
        uint32_t texture = CreateTexture();
        SetData(texture, target, internalFormat, width, height, format, dataType, data, multisampled, samples);
        return texture;
    }
    uint32_t TextureManager::CreateTexture(const char* filename, bool flip)
    {
        uint32_t texture = CreateTexture();

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(flip);
        unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
        if (data == nullptr)
        {
            std::cout << "Failed to load texture\n";
        }

        if (nrChannels == 4)
            textures[texture].internalFormat = textures[texture].format = GL_RGBA8;
        else if (nrChannels == 3)
            textures[texture].internalFormat = textures[texture].format = GL_RGB8;

        SetData(texture, GL_TEXTURE_2D, textures[texture].internalFormat, width, height, textures[texture].format, GL_UNSIGNED_BYTE, data, false, 0);
        stbi_image_free(data);

        return texture;
    }
    uint32_t TextureManager::CreateCubemap(std::vector<const char *> faces)
    {
        uint32_t texture = CreateTexture();
        
        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            stbi_set_flip_vertically_on_load(false);
            unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
            if (data)
            {
                SetData(texture, GL_TEXTURE_CUBE_MAP, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data, false, 0);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << "\n";
                stbi_image_free(data);
            }
        }

        TextureInfo info = textures[texture];

        glTexParameteri(info.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(info.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return texture;
    }

    void TextureManager::Bind(uint32_t texture, int i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(textures[texture].target, textures[texture].RendererID);
    }
    void TextureManager::Unbind(uint32_t texture)
    {
        glBindTexture(textures[texture].target, 0);
    }

    void TextureManager::SetData(uint32_t texture, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum dataType, const void *data, bool multisampled, GLsizei samples)
    {
        textures[texture].target = target;
        textures[texture].internalFormat = internalFormat;
        textures[texture].format = format;
        textures[texture].dataType = dataType;
        textures[texture].width = width;
        textures[texture].height = height;
        textures[texture].multisampled = multisampled;
        textures[texture].samples = samples;

        TextureInfo info = textures[texture];

        glBindTexture(info.target, info.RendererID);
        
        if(info.multisampled)
            glTexImage2DMultisample(info.target, info.samples, info.internalFormat, info.width, info.height, GL_TRUE);
        else
            glTexImage2D(info.target, 0, info.internalFormat, info.width, info.height, 0, info.format, info.dataType, data);
    }

    void TextureManager::Resize(uint32_t texture, GLsizei width, GLsizei height)
    {
        TextureInfo info = textures[texture];
        SetData(texture, info.target, info.internalFormat, width, height, info.format, info.dataType, nullptr, info.multisampled, info.samples);
    }

    void TextureManager::GenerateMipmaps(uint32_t texture)
    {
        Bind(texture, 0);
        glGenerateMipmap(textures[texture].target);
    }

    void TextureManager::SetParameters(uint32_t texture, GLint wrapping, GLint minFilter, GLint maxFilter)
    {
        TextureInfo info = textures[texture];

        glBindTexture(info.target, info.RendererID);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_T, wrapping);
        glTexParameteri(info.target, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(info.target, GL_TEXTURE_MAG_FILTER, maxFilter);
    }

    void TextureManager::SetBorderColor(uint32_t texture, float r, float g, float b, float a)
    {
        TextureInfo info = textures[texture];

        glBindTexture(info.target, info.RendererID);
        float color[] = { r, g, b, a };
		glTexParameterfv(info.target, GL_TEXTURE_BORDER_COLOR, color);
    }

    void TextureManager::ReleaseAll()
    {
        for(TextureInfo info : textures)
        {
            glDeleteTextures(1, &info.RendererID);
            info.RendererID = 0;
        }
    }
}