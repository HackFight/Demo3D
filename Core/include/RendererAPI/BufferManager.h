#pragma once

//libs
#include <glad/glad.h>
#include <glm/glm.hpp>

//std
#include <stdint.h>
#include <vector>

namespace Core
{
    struct Vertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		glm::vec3 color = glm::vec3(1.0f);
	};

    class VertexBufferManager
    {
    public:
        static uint32_t CreateVertexBuffer();
        static uint32_t CreateVertexBuffer(GLsizeiptr size);
        static uint32_t CreateVertexBuffer(float* vertices, GLsizeiptr size);

        static void Bind(uint32_t buffer);
        static void Unbind();

        static void SetData(uint32_t buffer, const void* data, GLsizeiptr size);

        static void ReleaseAll();

    private:
        static std::vector<GLuint> vertexBuffers;
    };

    struct IndexBufferInfo
    {
        GLuint RendererID = 0;
        GLsizeiptr Count = 0;
    };

    class IndexBufferManager
    {
    public:
        static uint32_t CreateIndexBuffer(uint32_t* indices, GLsizeiptr count);

        static void Bind(uint32_t buffer);
        static void Unbind();

        static GLsizeiptr GetCount(uint32_t buffer);

        static void ReleaseAll();

    private:
        static uint32_t CreateIndexBuffer();
        static std::vector<IndexBufferInfo> indexBuffer;
    };

    struct VertexArrayInfo
    {
        GLuint RendererID = 0;
        uint32_t vertexBuffer = -1;
        uint32_t indexBuffer = -1;
    };

    class VertexArrayManager
    {
    public:
        static uint32_t CreateVertexArray();

        static void Bind(uint32_t vertexArray);
        static void Unbind();

        static void AddVertexBuffer(uint32_t vertexArray, uint32_t buffer);
        static void SetIndexBuffer(uint32_t vertexArray, uint32_t buffer);

        static GLsizeiptr GetIndexCount(uint32_t vertexArray);

        static void ReleaseAll();

    private:
        static std::vector<VertexArrayInfo> vertexArrays;
    };
}