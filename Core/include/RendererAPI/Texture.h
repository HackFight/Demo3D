#pragma once

// std
#include <memory>
#include <stdint.h>
#include <vector>

namespace Core {

    class Texture
	{
	public:
		Texture();
		Texture(uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType, const void* data, bool multisampled, int samples);
		Texture(const char* filename, uint32_t internalFormat, uint32_t format, bool flip);
		Texture(std::vector<const char*> faces);
		~Texture();

		void Bind(int i) const;
		static void Unbind();

		void SetData(uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType, const void* data, bool multisampled = false, int samples = 4);
		void GenerateMipmaps();
		void SetParameters(uint32_t wrapping, uint32_t minFilter, uint32_t maxFilter);
		void SetBorderColor(float r, float g, float b, float a);
		
		uint32_t GetRendererID() {return  m_RendererID;}

		static std::shared_ptr<Texture> Create();
		static std::shared_ptr<Texture> Create(uint32_t internalFormat, int width, int height, uint32_t format, uint32_t dataType, const void* data, bool multisampled = false, int samples = 4);
		static std::shared_ptr<Texture> Create(const char* filename, uint32_t internalFormat, uint32_t format, bool flip = false);
		static std::shared_ptr<Texture> CreateCubemap(std::vector<const char*> faces);
	private:
		uint32_t m_RendererID;
		uint32_t textureType;
		bool multisampled;
		int samples;
	};
}