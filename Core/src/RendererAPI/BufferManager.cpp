#include "RendererAPI/BufferManager.h"
#include <iostream>

namespace Core
{
    std::vector<GLuint> VertexBufferManager::vertexBuffers;
    size_t VertexBufferManager::CreateVertexBuffer()
    {
        GLuint buffer;

        glCreateBuffers(1, &buffer);

        vertexBuffers.push_back(buffer);
        return vertexBuffers.size() - 1;
    }
    size_t VertexBufferManager::CreateVertexBuffer(GLsizeiptr size, bool staticDraw)
    {
        size_t buffer = CreateVertexBuffer();
        SetData(buffer, nullptr, size, staticDraw);
        return buffer;
    }
    size_t VertexBufferManager::CreateVertexBuffer(const float* vertices, GLsizeiptr size, bool staticDraw)
    {
        size_t buffer = CreateVertexBuffer();
        SetData(buffer, vertices, size, staticDraw);
        return buffer;
    }

    void VertexBufferManager::Bind(size_t buffer)
    {
		if (glIsBuffer(vertexBuffers.at(buffer)))
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers.at(buffer));
		else
			std::cout << "Warning: Trying to bind invalid Vertex Buffer ID " << buffer << std::endl;
    }
    void VertexBufferManager::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void VertexBufferManager::SetData(size_t buffer, const void *data, GLsizeiptr size, bool staticDraw)
    {
		VertexBufferManager::Bind(buffer);
        GLenum usage = staticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }
    void VertexBufferManager::SetSubData(size_t buffer, const void* data, GLsizeiptr size, GLsizeiptr offset)
    {
        VertexBufferManager::Bind(buffer);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
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
    size_t IndexBufferManager::CreateIndexBuffer()
    {
        IndexBufferInfo buffer;

        glCreateBuffers(1, &buffer.rendererID);

        indexBuffers.push_back(buffer);
        return indexBuffers.size() - 1;
    }
    size_t IndexBufferManager::CreateIndexBuffer(const uint32_t* indices, GLsizeiptr count)
    {
        size_t buffer = CreateIndexBuffer();
        indexBuffers.at(buffer).count = count;

        IndexBufferManager::Bind(buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(size_t), indices, GL_STATIC_DRAW);

        return buffer;
    }

    void IndexBufferManager::Bind(size_t buffer)
    {
        if (glIsBuffer(indexBuffers.at(buffer).rendererID))
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers.at(buffer).rendererID);
        else
            std::cout << "Warning: Trying to bind invalid Index Buffer ID " << buffer << std::endl;
    }
    void IndexBufferManager::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLsizeiptr IndexBufferManager::GetCount(size_t buffer)
    {
        return indexBuffers.at(buffer).count;
    }

    void IndexBufferManager::ReleaseAll()
    {
        for (IndexBufferInfo& info : indexBuffers)
        {
            if (info.rendererID != 0)
            {
                glDeleteBuffers(1, &info.rendererID);
				info.rendererID = 0;
            }
        }
        indexBuffers.clear();
    }



    std::vector<VertexArrayInfo> VertexArrayManager::vertexArrays;
    size_t VertexArrayManager::CreateVertexArray()
    {
        VertexArrayInfo vertexArray;

        glCreateVertexArrays(1, &vertexArray.rendererID);

        vertexArrays.push_back(vertexArray);
        return vertexArrays.size() - 1;
    }

    void VertexArrayManager::Bind(size_t vertexArray)
    {
        if(glIsVertexArray(vertexArrays.at(vertexArray).rendererID))
            glBindVertexArray(vertexArrays.at(vertexArray).rendererID);
        else
			std::cout << "Warning: Trying to bind invalid Vertex Array ID " << vertexArray << std::endl;
    }
    void VertexArrayManager::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArrayManager::AddVertexBuffer(size_t vertexArray, size_t buffer)
    {
        Bind(vertexArray);
        VertexBufferManager::Bind(buffer);

        vertexArrays.at(vertexArray).vertexBuffer = buffer;

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(4);

        Unbind();
    }

    void VertexArrayManager::SetIndexBuffer(size_t vertexArray, size_t buffer)
    {
        Bind(vertexArray);
        IndexBufferManager::Bind(buffer);

        vertexArrays.at(vertexArray).indexBuffer = buffer;
        Unbind();
    }

    GLsizeiptr VertexArrayManager::GetIndexCount(size_t vertexArray)
    {
        return IndexBufferManager::GetCount(vertexArrays.at(vertexArray).indexBuffer);
    }
    VertexArrayInfo VertexArrayManager::GetVAOInfo(size_t vertexArray)
    {
        return vertexArrays.at(vertexArray);
    }

    void VertexArrayManager::ReleaseAll()
    {
        for (VertexArrayInfo& info : vertexArrays)
        {
            if (info.rendererID != 0)
            {
                glDeleteVertexArrays(1, &info.rendererID);
                info.rendererID = 0;
            }
        }
        vertexArrays.clear();
    }
}