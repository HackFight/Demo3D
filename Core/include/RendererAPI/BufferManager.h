#pragma once

//libs
#include <glad/glad.h>
#include <glm/glm.hpp>

//std
#include <vector>

namespace Core
{
    struct Vertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		glm::vec3 tangent = glm::vec3(0.0f);
		glm::vec3 color = glm::vec3(1.0f);
	};

    class VertexBufferManager
    {
    public:
        static size_t CreateVertexBuffer();
        static size_t CreateVertexBuffer(GLsizeiptr size, bool staticDraw = true);
        static size_t CreateVertexBuffer(const float* vertices, GLsizeiptr size, bool staticDraw = true);

        static void Bind(size_t buffer);
        static void Unbind();

        static void SetData(size_t buffer, const void* data, GLsizeiptr size, bool staticDraw);
		static void SetSubData(size_t buffer, const void* data, GLsizeiptr size, GLsizeiptr offset);

        static void ReleaseAll();

    private:
        static std::vector<GLuint> vertexBuffers;
    };

    struct IndexBufferInfo
    {
        GLuint rendererID = 0;
        GLsizeiptr count = 0;
    };

    class IndexBufferManager
    {
    public:
        static size_t CreateIndexBuffer(const uint32_t* indices, GLsizeiptr count);

        static void Bind(size_t buffer);
        static void Unbind();

        static GLsizeiptr GetCount(size_t buffer);

        static void ReleaseAll();

    private:
        static size_t CreateIndexBuffer();
        static std::vector<IndexBufferInfo> indexBuffers;
    };

    struct VertexArrayInfo
    {
        GLuint rendererID = 0;
        size_t vertexBuffer = 0;
        size_t indexBuffer = 0;
    };

    class VertexArrayManager
    {
    public:
        static size_t CreateVertexArray();

        static void Bind(size_t vertexArray);
        static void Unbind();

        static void AddVertexBuffer(size_t vertexArray, size_t buffer);
        static void SetIndexBuffer(size_t vertexArray, size_t buffer);

        static GLsizeiptr GetIndexCount(size_t vertexArray);
        static VertexArrayInfo GetVAOInfo(size_t vertexArray);

        static void ReleaseAll();

    private:
        static std::vector<VertexArrayInfo> vertexArrays;
    };
}