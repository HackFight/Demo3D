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
		Texture( uint32_t internalFormat, int width, int height, uint32_t param, uint32_t format, const void* data);
		Texture(const char* filename, uint32_t internalFormat, uint32_t param, uint32_t format, bool flip = false);
		Texture(std::vector<const char*> faces);
		~Texture();

		void Bind() const;
		static void Unbind();

		void SetData(uint32_t internalFormat, int width, int height, uint32_t param, uint32_t format, const void* data);
		uint32_t GetRendererID() {return  m_RendererID;}

		static std::shared_ptr<Texture> Create();
		static std::shared_ptr<Texture> Create( uint32_t internalFormat, int width, int height, uint32_t param, uint32_t format, const void* data);
		static std::shared_ptr<Texture> Create(const char* filename, uint32_t internalFormat, uint32_t param, uint32_t format);
		static std::shared_ptr<Texture> CreateCubeMap(std::vector<const char*> faces);

  private:
    uint32_t m_RendererID;
	};
}