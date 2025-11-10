#include "RendererAPI/VertexArray.h"
#include "RendererAPI/Buffer.h"
#include <glad/glad.h>

namespace Core {

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }
    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }
    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const VertexBuffer vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer.Bind();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(3);
	}

	void VertexArray::SetIndexBuffer(const IndexBuffer indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer.Bind();
	}
}