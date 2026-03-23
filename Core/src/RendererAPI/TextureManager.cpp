#include "RendererAPI/TextureManager.h"

// libs
#include <stb_image/stb_image.h>

// std
#include <iostream>
#include <iomanip> // Add this include for std::setprecision

namespace Core
{
    std::vector<TextureInfo> TextureManager::textures;
    size_t TextureManager::CreateTexture()
    {
        TextureInfo texture;

        glGenTextures(1, &texture.rendererID);

        textures.push_back(texture);
        return textures.size() - 1;
    }
    size_t TextureManager::CreateTexture(GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum dataType, const void* data, bool multisampled, GLsizei samples)
    {
        size_t texture = CreateTexture();
        SetData(texture, target, internalFormat, width, height, format, dataType, data, multisampled, samples);
        SetParameters(texture, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        return texture;
    }
    size_t TextureManager::CreateTexture(const char* filename, bool flip)
    {
        size_t texture = CreateTexture();

        int width{}, height{}, nrChannels{};
        unsigned char* data = nullptr;

        stbi_set_flip_vertically_on_load(flip);
        data = stbi_load(filename, &width, &height, &nrChannels, 0);
        if (data == nullptr)
        {
            std::cout << "Failed to load texture\n";
        }

        switch (nrChannels) {
            case 4:
                textures[texture].internalFormat = GL_SRGB8_ALPHA8;
			    textures[texture].format = GL_RGBA;
                break;

            case 3:
                textures[texture].internalFormat = GL_SRGB8;
			    textures[texture].format = GL_RGB;
                break;

            case 2:
                textures[texture].internalFormat = GL_RG8;
			    textures[texture].format = GL_RG;
                break;

            case 1:
                textures[texture].internalFormat = GL_R8;
                textures[texture].format = GL_RED;
                break;
            
            default:
                std::cout << nrChannels << " is an unregistered amount of color channels! [" << filename << "]\n";
        }

        SetData(texture, GL_TEXTURE_2D, textures[texture].internalFormat, width, height, textures[texture].format, GL_UNSIGNED_BYTE, data, false, 0);
		SetParameters(texture, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        stbi_image_free(data);

        return texture;
    }
    size_t TextureManager::CreateCubemap(std::vector<const char *> faces)
    {
        size_t texture = CreateTexture();

        textures[texture].target = GL_TEXTURE_CUBE_MAP;
        glBindTexture(GL_TEXTURE_CUBE_MAP, textures[texture].rendererID);
        
        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            stbi_set_flip_vertically_on_load(false);
            unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
            if (data)
            {
                GLint internalFormat;
                GLenum format;
                if (nrChannels == 4)
                {
                    internalFormat = GL_RGBA8;
                    format = GL_RGBA;
                }
                else if (nrChannels == 3)
                {
                    internalFormat = GL_RGB8;
                    format = GL_RGB;
                }

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << "\n";
                stbi_image_free(data);
            }
        }

        if (nrChannels == 4)
        {
            textures[texture].internalFormat = GL_RGBA8;
            textures[texture].format = GL_RGBA;
        }
        else if (nrChannels == 3)
        {
            textures[texture].internalFormat = GL_RGB8;
            textures[texture].format = GL_RGB;
        }

        textures[texture].dataType = GL_UNSIGNED_BYTE;
        textures[texture].width = width;
        textures[texture].height = height;
        textures[texture].multisampled = false;
        textures[texture].samples = 0;

        TextureInfo info = textures[texture];

        glTexParameteri(info.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(info.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return texture;
    }

    void TextureManager::Bind(size_t texture, int i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(textures[texture].target, textures[texture].rendererID);
    }
    void TextureManager::Unbind(size_t texture, int i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(textures[texture].target, 0);
    }
    void TextureManager::Unbind(int i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void TextureManager::SetData(size_t texture, GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum dataType, const void *data, bool multisampled, GLsizei samples)
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

        glBindTexture(info.target, info.rendererID);
        
        if(info.multisampled)
            glTexImage2DMultisample(info.target, info.samples, info.internalFormat, info.width, info.height, GL_TRUE);
        else
            glTexImage2D(info.target, 0, info.internalFormat, info.width, info.height, 0, info.format, info.dataType, data);
    }

    void TextureManager::Resize(size_t texture, GLsizei width, GLsizei height)
    {
        TextureInfo info = textures[texture];
        SetData(texture, info.target, info.internalFormat, width, height, info.format, info.dataType, nullptr, info.multisampled, info.samples);
    }

    void TextureManager::GenerateMipmaps(size_t texture)
    {
        Bind(texture, 0);
        glGenerateMipmap(textures[texture].target);
    }

    void TextureManager::SetParameters(size_t texture, GLint wrapping, GLint minFilter, GLint maxFilter)
    {
        TextureInfo info = textures[texture];

        glBindTexture(info.target, info.rendererID);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(info.target, GL_TEXTURE_WRAP_T, wrapping);
        glTexParameteri(info.target, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(info.target, GL_TEXTURE_MAG_FILTER, maxFilter);
    }

    void TextureManager::SetBorderColor(size_t texture, float r, float g, float b, float a)
    {
        TextureInfo info = textures[texture];

        glBindTexture(info.target, info.rendererID);
        float color[] = { r, g, b, a };
		glTexParameterfv(info.target, GL_TEXTURE_BORDER_COLOR, color);
    }

    static size_t _EstimateBppFromInternalFormat(GLint internalFormat)
    {
        switch (internalFormat)
        {
        case GL_RGBA16F: return 8;    // 4 channels * 2 bytes
        case GL_RGBA8:   return 4;
        case GL_RGB8:    return 3;
        case GL_R8:      return 1;
        case GL_DEPTH_COMPONENT32F: return 4;
        case GL_DEPTH24_STENCIL8: return 4; // approx
        default: return 4; // conservative default
        }
    }

    size_t TextureManager::EstimateTotalMemoryBytes()
    {
        size_t total = 0;
        for (const TextureInfo& t : textures)
        {
            if (t.rendererID == 0 || t.width == 0 || t.height == 0) continue;
            size_t bpp = _EstimateBppFromInternalFormat(t.internalFormat);
            size_t pixels = (size_t)t.width * (size_t)t.height;
            size_t bytes = pixels * bpp;
            if (t.multisampled && t.samples > 0) bytes *= (size_t)t.samples;
            total += bytes;
        }
        return total;
    }

    void TextureManager::DebugPrintSummary()
    {
        size_t totalBytes = EstimateTotalMemoryBytes();
        size_t texCount = 0;
        for (const TextureInfo& t : textures) if (t.rendererID != 0) ++texCount;
        std::cout << "[TextureManager] Textures: " << texCount
            << ", Estimated GPU bytes: " << std::fixed << std::setprecision(2)
            << (totalBytes / 1024.0 / 1024.0) << " MB\n";
    }

    void TextureManager::ReleaseAll()
    {
        for (auto& info : textures)
        {
            if (info.rendererID != 0)
            {
                glDeleteTextures(1, &info.rendererID);
                info.rendererID = 0;
            }
        }
        textures.clear();
    }
}