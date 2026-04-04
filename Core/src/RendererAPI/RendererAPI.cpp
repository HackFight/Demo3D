#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/BufferManager.h"

// libs
#include <cstddef>
#include <cstdint>
#include <glad/glad.h>

namespace Core {

	void RendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void RendererAPI::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}
	void RendererAPI::SetScissors(int x, int y, int width, int height) {
		glScissor(x, y, width, height);
	}
	void RendererAPI::EnableScissors(bool b) {
		b ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
	}

	void RendererAPI::SetClearColor(glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RendererAPI::ClearColor()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void RendererAPI::ClearDepth()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	void RendererAPI::DepthTest(bool b)
	{
		b ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}
	void RendererAPI::SetDepthFunc(int func)
	{
		glDepthFunc(func);
	}
	void RendererAPI::SRGBColorSpace(bool b)
	{
		b ? glEnable(GL_FRAMEBUFFER_SRGB) : glDisable(GL_FRAMEBUFFER_SRGB);
	}
	void RendererAPI::UnbindAllTextures()
	{
		for (int i = 0; i < 16; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void RendererAPI::SetCullMode(CullSide cull)
	{
		switch (cull)
		{
		case None:
			glCullFace(GL_NONE);
			break;
		case Front:
			glCullFace(GL_FRONT);
			break;
		case Back:
			glCullFace(GL_BACK);
			break;
		case Both:
			glCullFace(GL_FRONT_AND_BACK);
			break;
		default:
			glCullFace(GL_BACK);
			break;
		}
	}

	void RendererAPI::DrawIndexed(size_t vertexArray, size_t indexCount)
	{
		VertexArrayManager::Bind(vertexArray);
		uint32_t count = indexCount ? indexCount : VertexArrayManager::GetIndexCount(vertexArray);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void RendererAPI::DrawLines(size_t vertexArray, size_t vertexCount)
	{
		VertexArrayManager::Bind(vertexArray);
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void RendererAPI::DrawPoints(size_t vertexArray, size_t vertexCount)
	{
		VertexArrayManager::Bind(vertexArray);
		glDrawArrays(GL_POINT, 0, vertexCount);
	}

	void RendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}
}