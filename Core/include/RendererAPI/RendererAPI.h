#pragma once

#include "RendererAPI/VertexArray.h"

// libs
#include <glm/glm.hpp>

// std
#include <memory>
#include <stdint.h>

namespace Core {

	enum CullSide
	{
		None,
		Front,
		Back,
		Both
	};

	class RendererAPI
	{
	public:
		void Init();
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		void SetClearColor(const glm::vec4& color);
		void ClearColor();
		void ClearDepth();
		void DepthTest(bool b);
		void SRGBColorSpace(bool b);
		void UnbindAllTextures();
		void SetCullMode(CullSide cull);

		void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
		void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount);
		
		void SetLineWidth(float width);
	};


}