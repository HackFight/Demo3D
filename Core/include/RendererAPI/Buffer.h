#pragma once

// libs
#include <glm/glm.hpp>

// std
#include <memory>
#include <stdint.h>

namespace Core {

	struct Vertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		glm::vec3 color = glm::vec3(1.0f);
	};

    class VertexBuffer
	{
	public:
    	VertexBuffer(uint32_t size);
    	VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t size);

		static std::shared_ptr<VertexBuffer> Create(uint32_t size);
		static std::shared_ptr<VertexBuffer> Create(float* vertices, uint32_t size);
	
	private:
		uint32_t m_RendererID;
	};

	// Currently Hazel only supports 32-bit index buffers
	class IndexBuffer
	{
	public:
    	IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_Count; }

		static std::shared_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t count);

  private:
    uint32_t m_RendererID;
    uint32_t m_Count;
	};
}