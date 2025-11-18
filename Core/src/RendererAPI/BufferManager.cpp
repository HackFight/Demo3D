#include "RendererAPI/BufferManager.h"
#include <iostream>

namespace Core
{
    std::vector<GLuint> VertexBufferManager::vertexBuffers;
    uint32_t VertexBufferManager::CreateVertexBuffer()
    {
        GLuint buffer;

        glCreateBuffers(1, &buffer);

        vertexBuffers.push_back(buffer);
        return vertexBuffers.size() - 1;
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
		if (glIsBuffer(vertexBuffers[buffer]))
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[buffer]);
		else
			std::cout << "Warning: Trying to bind invalid Vertex Buffer ID " << buffer << std::endl;
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
        for(GLuint buffer : vertexBuffers)
        {
            if (buffer != 0)
            {
                glDeleteBuffers(1, &buffer);
            }
        }
        vertexBuffers.clear();
    }



    std::vector<IndexBufferInfo> IndexBufferManager::indexBuffers;
    uint32_t IndexBufferManager::CreateIndexBuffer()
    {
        IndexBufferInfo buffer;

        glCreateBuffers(1, &buffer.RendererID);

        indexBuffers.push_back(buffer);
        return indexBuffers.size() - 1;
    }
    uint32_t IndexBufferManager::CreateIndexBuffer(uint32_t* indices, GLsizeiptr count)
    {
        uint32_t buffer = CreateIndexBuffer();
        indexBuffers[buffer].Count = count;

        Bind(buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

        return buffer;
    }

    void IndexBufferManager::Bind(uint32_t buffer)
    {
        if (glIsBuffer(indexBuffers[buffer].RendererID))
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[buffer].RendererID);
        else
            std::cout << "Warning: Trying to bind invalid Index Buffer ID " << buffer << std::endl;
    }
    void IndexBufferManager::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLsizeiptr IndexBufferManager::GetCount(uint32_t buffer)
    {
        return indexBuffers[buffer].Count;
    }

    void IndexBufferManager::ReleaseAll()
    {
        for (IndexBufferInfo& info : indexBuffers)
        {
            if (info.RendererID != 0)
            {
                glDeleteBuffers(1, &info.RendererID);
				info.RendererID = 0;
            }
        }
        indexBuffers.clear();
    }



    std::vector<VertexArrayInfo> VertexArrayManager::vertexArrays;
    uint32_t VertexArrayManager::CreateVertexArray()
    {
        VertexArrayInfo vertexArray;

        glCreateVertexArrays(1, &vertexArray.RendererID);

        vertexArrays.push_back(vertexArray);
        return vertexArrays.size() - 1;
    }

    void VertexArrayManager::Bind(uint32_t vertexArray)
    {
        if(glIsVertexArray(vertexArrays[vertexArray].RendererID))
            glBindVertexArray(vertexArrays[vertexArray].RendererID);
        else
			std::cout << "Warning: Trying to bind invalid Vertex Array ID " << vertexArray << std::endl;
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
        for (VertexArrayInfo& info : vertexArrays)
        {
            if (info.RendererID != 0)
            {
                glDeleteVertexArrays(1, &info.RendererID);
                info.RendererID = 0;
            }
        }
        vertexArrays.clear();
    }
}