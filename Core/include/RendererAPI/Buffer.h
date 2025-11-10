#pragma once

// libs
#include <glm/glm.hpp>

namespace Core {

	struct Vertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		glm::vec3 color = glm::vec3(1.0f);
	};

    class VertexBuffer
	{
	public:
    	VertexBuffer(size_t size);
    	VertexBuffer(float* vertices, size_t size);
		~VertexBuffer();

		void Bind() const;
		static void Unbind();

		void SetData(const void* data, size_t size);
	
	private:
		uint32_t m_RendererID;
	};

	class IndexBuffer
	{
	public:
    	IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		static void Unbind();

		uint32_t GetCount() const { return m_Count; }

  private:
    uint32_t m_RendererID;
    uint32_t m_Count;
	};
}