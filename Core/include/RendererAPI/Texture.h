#pragma once

// std
#include <memory>
#include <stdint.h>

namespace Core {

    class Texture
	{
	public:
    	Texture();
		Texture(int width, int height, uint32_t param, uint32_t format, const void* data);
		Texture(const char* filename, uint32_t param, uint32_t format);
		~Texture();

		void Bind() const;
		void Unbind() const;

		void SetData(int width, int height, uint32_t param, uint32_t format, const void* data);

		static std::shared_ptr<Texture> Create();
		static std::shared_ptr<Texture> Create(int width, int height, uint32_t param, uint32_t format, const void* data);

  private:
    uint32_t m_RendererID;
	};
}