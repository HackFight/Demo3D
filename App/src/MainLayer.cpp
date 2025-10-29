#include "MainLayer.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "RendererAPI/RendererAPI.h"
#include "Renderer/Camera.h"
#include "RendererAPI/Buffer.h"
#include "RendererAPI/VertexArray.h"

// std
#include <iostream>
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

    camera = std::make_shared<Core::Camera>(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);
}
MainLayer::~MainLayer() {}

void MainLayer::OnUpdate(float ts)
{
    timeAcc += ts;
    frameCounter++;

    if(timeAcc >= 1.0)
    {
        timeAcc -= 1.0;
        std::cout << frameCounter << " FPS\n";
        frameCounter = 0;
    }

    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(Core::FORWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(Core::BACKWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(Core::LEFT, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(Core::RIGHT, ts);

    double xpos, ypos;
    glfwGetCursorPos(Core::Application::Get().GetWindow()->GetHandle(), &xpos, &ypos);
}
void MainLayer::OnRender()
{
    renderer->Clear();
    glm::vec2 viewportSize = Core::Application::Get().GetFramebufferSize();
    renderer->SetViewport(0, 0, viewportSize.x, viewportSize.y);

    shader->Bind();
    shader->setmat4("modelMat", glm::mat4(1.0f));
    shader->setmat4("viewMat", camera->getViewMatrix());
    shader->setmat4("projMat", camera->getProjectionMatrix());
    renderer->DrawIndexed(vertexArray);
}