#pragma once

// libs
#include <glm/glm.hpp>

// Core
#include "RendererAPI/VertexArray.h"

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
		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void SetClearColor(const glm::vec4 color);
		static void ClearColor();
		static void ClearDepth();
		static void DepthTest(bool b);
		static void SetDepthFunc(uint32_t func);
		static void SRGBColorSpace(bool b);
		static void UnbindAllTextures();
		static void SetCullMode(CullSide cull);

		static void DrawIndexed(const VertexArray vertexArray, uint32_t indexCount = 0);
		static void DrawLines(const VertexArray vertexArray, uint32_t vertexCount);
		
		static void SetLineWidth(float width);
	};


}