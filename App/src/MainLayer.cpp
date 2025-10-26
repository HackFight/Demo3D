#include "MainLayer.h"

#include "Core/Application.h"
#include "RendererAPI/RendererAPI.h"
#include "Renderer/Camera.h"
#include "RendererAPI/Buffer.h"
#include "RendererAPI/VertexArray.h"

// std
#include <memory>
#include <vector>

MainLayer::MainLayer()
{
    std::vector<Core::Vertex> vertices
    {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
    };
    vertexBuffer = Core::VertexBuffer::Create((float*)vertices.data(), vertices.size()*sizeof(Core::Vertex));
    uint32_t indices[]{
        0, 1, 2,
        0, 2, 3
    };
    indexBuffer = Core::IndexBuffer::Create(indices, 6);

    vertexArray = std::make_shared<Core::VertexArray>();
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    shader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/default.frag");

    renderer = std::make_unique<Core::RendererAPI>();
    renderer->Init();
    renderer->SetClearColor(glm::vec4(0.0f));
}
MainLayer::~MainLayer() {}

void MainLayer::OnUpdate(float ts) {}
void MainLayer::OnRender()
{
    renderer->Clear();
    glm::vec2 viewportSize = Core::Application::Get().GetFramebufferSize();
    renderer->SetViewport(0, 0, viewportSize.x, viewportSize.y);

    shader->Bind();
    shader->setmat4("modelMat", glm::mat4(1.0f));
    shader->setmat4("viewMat", glm::mat4(1.0f));
    shader->setmat4("projMat", glm::mat4(1.0f));
    renderer->DrawIndexed(vertexArray);
}