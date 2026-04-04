#pragma once

// libs
#include <glm/glm.hpp>

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
		static void SetViewport(int x, int y, int width, int height);
		static void SetScissors(int x, int y, int width, int height);
		static void EnableScissors(bool b);

		static void SetClearColor(glm::vec4 color);
		static void ClearColor();
		static void ClearDepth();
		static void DepthTest(bool b);
		static void SetDepthFunc(int func);
		static void SRGBColorSpace(bool b);
		static void UnbindAllTextures();
		static void SetCullMode(CullSide cull);

		static void DrawIndexed(size_t vertexArray, size_t indexCount = 0);
		static void DrawLines(size_t vertexArray, size_t vertexCount);
		static void DrawPoints(size_t vertexArray, size_t vertexCount);
		
		static void SetLineWidth(float width);
	};
}