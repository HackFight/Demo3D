#pragma once

#include "Renderer/VertexArray.h"

// libs
#include <glm/glm.hpp>

// std
#include <memory>
#include <stdint.h>

namespace Core {

	class RendererAPI
	{
	public:
		void Init();
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		void SetClearColor(const glm::vec4& color);
		void Clear();

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
		void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount);
		
		void SetLineWidth(float width);
	};


}