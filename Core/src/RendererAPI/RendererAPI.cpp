#include "RendererAPI/RendererAPI.h"

// libs
#include <glad/glad.h>
#include <memory>

namespace Core {

	void RendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void RendererAPI::SetClearColor(const glm::vec4& color)
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
	void RendererAPI::UnbindAllTextures()
	{
		for (int i = 0; i < 16; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void RendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void RendererAPI::DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void RendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

}