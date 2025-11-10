#pragma once

// std
#include <stdint.h>
#include <vector>

namespace Core {

	struct TextureInfo
	{
		uint32_t target;
		uint32_t internalFormat;
		int width;
		int height;
		uint32_t format;
		uint32_t dataType;
		bool multisampled;
		int samples;
	};

    class Texture
	{
	public:
		Texture();
		Texture(uint32_t target, uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType, const void* data, bool multisampled, int samples);
		Texture(const char* filename, bool flip = false);
		Texture(std::vector<const char*> faces);
		~Texture();

		void Bind(int i) const;
		static void Unbind(uint32_t target);

		void SetData(uint32_t target, uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType, const void* data, bool multisampled, int samples);
		void Resize(int width, int height);
		void GenerateMipmaps();

		void SetParameters(uint32_t wrapping, uint32_t minFilter, uint32_t maxFilter);
		void SetBorderColor(float r, float g, float b, float a);
		
		uint32_t GetRendererID() { return  m_RendererID; }
		TextureInfo GetTextureInfo() { return m_TextureInfo; }

	private:
		uint32_t m_RendererID;
		TextureInfo m_TextureInfo;
	};
}