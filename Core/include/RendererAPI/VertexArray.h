#pragma once

#include "RendererAPI/Buffer.h"

namespace Core {

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        static void Unbind();

		GLsizei GetIndexCount() const { return m_IndexCount; }

        void AddVertexBuffer(const VertexBuffer vertexBuffer);
        void SetIndexBuffer(const IndexBuffer indexBuffer);

    private:
        uint32_t m_RendererID;
        GLsizei m_IndexCount;
    };
}