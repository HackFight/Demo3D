#include "RendererAPI/BufferManager.h"
#include <cstdint>

namespace Core
{
    uint32_t VertexBufferManager::CreateVertexBuffer()
    {
        GLuint buffer;

        glCreateBuffers(1, &buffer);

        buffers.push_back(buffer);
        return buffers.size() - 1;
    }
    uint32_t VertexBufferManager::CreateVertexBuffer(GLsizeiptr size)
    {
        uint32_t buffer = CreateVertexBuffer();
        SetData(buffer, nullptr, size);
        return buffer;
    }
    uint32_t VertexBufferManager::CreateVertexBuffer(float* vertices, GLsizeiptr size)
    {
        uint32_t buffer = CreateVertexBuffer();
        SetData(buffer, vertices, size);
        return buffer;
    }

    void VertexBufferManager::Bind(uint32_t buffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer]);
    }
    void VertexBufferManager::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void VertexBufferManager::SetData(uint32_t buffer, const void *data, GLsizeiptr size)
    {
		Bind(buffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void VertexBufferManager::ReleaseAll()
    {
        for(GLuint buffer : buffers)
        {
            glDeleteBuffers(1, &buffer);
        }
    }



    uint32_t IndexBufferManager::CreateIndexBuffer()
    {
        IndexBufferInfo buffer;

        glCreateBuffers(1, &buffer.RendererID);

        buffers.push_back(buffer);
        return buffers.size() - 1;
    }
    uint32_t IndexBufferManager::CreateIndexBuffer(uint32_t* indices, GLsizeiptr count)
    {
        uint32_t buffer = CreateIndexBuffer();
        buffers[buffer].Count = count;

        Bind(buffer);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

        return buffer;
    }

    void IndexBufferManager::Bind(uint32_t buffer)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[buffer].RendererID);
    }
    void IndexBufferManager::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLsizeiptr IndexBufferManager::GetCount(uint32_t buffer)
    {
        return buffers[buffer].Count;
    }

    void IndexBufferManager::ReleaseAll()
    {
        for (IndexBufferInfo info : buffers)
        {
            glDeleteBuffers(1, &info.RendererID);
        }
    }



    uint32_t VertexArrayManager::CreateVertexArray()
    {
        VertexArrayInfo vertexArray;

        glCreateBuffers(1, &vertexArray.RendererID);

        vertexArrays.push_back(vertexArray);
        return vertexArrays.size() - 1;
    }

    void VertexArrayManager::Bind(uint32_t vertexArray)
    {
        glBindVertexArray(vertexArrays[vertexArray].RendererID);
    }
    void VertexArrayManager::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArrayManager::AddVertexBuffer(uint32_t vertexArray, uint32_t buffer)
    {
        Bind(vertexArray);
        VertexBufferManager::Bind(buffer);

        vertexArrays[vertexArray].vertexBuffer = buffer;

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(3);
    }

    void VertexArrayManager::SetIndexBuffer(uint32_t vertexArray, uint32_t buffer)
    {
        Bind(vertexArray);
        IndexBufferManager::Bind(buffer);

        vertexArrays[vertexArray].indexBuffer = buffer;
    }

    GLsizeiptr VertexArrayManager::GetIndexCount(uint32_t vertexArray)
    {
        return IndexBufferManager::GetCount(vertexArrays[vertexArray].indexBuffer);
    }

    void VertexArrayManager::ReleaseAll()
    {
        for (VertexArrayInfo info : vertexArrays)
        {
            glDeleteVertexArrays(1, &info.RendererID);
            info.RendererID = 0;
        }
    }
}