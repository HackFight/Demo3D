#pragma once


// Core
#include "RendererAPI/Buffer.h"

namespace Core {

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        static void Unbind();

		uint32_t GetIndexCount() const { return m_IndexCount; }

        void AddVertexBuffer(const VertexBuffer vertexBuffer);
        void SetIndexBuffer(const IndexBuffer indexBuffer);

    private:
        uint32_t m_RendererID;
        uint32_t m_IndexCount;
    };
}