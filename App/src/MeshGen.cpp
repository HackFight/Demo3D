#include "MeshGen.h"

std::shared_ptr<Core::VertexArray> MeshGen::GetQuad()
{
    std::vector<Core::Vertex> vertices
    {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
    };
    std::shared_ptr<Core::VertexBuffer> vertexBuffer = Core::VertexBuffer::Create((float*)vertices.data(), vertices.size()*sizeof(Core::Vertex));
    uint32_t indices[]{
        0, 1, 2,
        0, 2, 3
    };
    std::shared_ptr<Core::IndexBuffer> indexBuffer = Core::IndexBuffer::Create(indices, 6);

    std::shared_ptr<Core::VertexArray> vertexArray = std::make_shared<Core::VertexArray>();
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    return vertexArray;
}

std::shared_ptr<Core::VertexArray> MeshGen::GetCube()
{
    std::vector<Core::Vertex> vertices
    {
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}}
    };
    std::shared_ptr<Core::VertexBuffer> vertexBuffer = Core::VertexBuffer::Create((float*)vertices.data(), vertices.size()*sizeof(Core::Vertex));
    uint32_t indices[]{
        0, 1, 2,
        0, 2, 3,

        1, 4, 7,
        1, 7, 2,

        4, 5, 6,
        4, 6, 7,

        5, 0, 3,
        5, 3, 6,

        0, 1, 4,
        0, 4, 5,

        3, 2, 7,
        3, 7, 6
    };
    std::shared_ptr<Core::IndexBuffer> indexBuffer = Core::IndexBuffer::Create(indices, 36);

    std::shared_ptr<Core::VertexArray> vertexArray = std::make_shared<Core::VertexArray>();
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    return vertexArray;
}