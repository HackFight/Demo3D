#include "MainLayer.h"

#include "Core/Application.h"
#include "GLFW/glfw3.h"
#include "MeshGen.h"
#include "RendererAPI/RendererAPI.h"
#include "Renderer/Camera.h"

// libs
#include <GL/gl.h>

// std
#include <iostream>
#include <memory>

MainLayer::MainLayer()
{
    vertexArray = MeshGen::GetQuad();

    defaultShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/default.frag");
    phongShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/phong.frag");

    renderer = std::make_unique<Core::RendererAPI>();
    renderer->Init();
    renderer->SetClearColor(glm::vec4(0.0f));

    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    double xpos, ypos;
    glfwGetCursorPos(Core::Application::Get().GetWindow()->GetHandle(), &xpos, &ypos);
    lastX = xpos; lastY = ypos;
    
    camera = std::make_shared<Core::Camera>(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);


    phongShader->Bind();
    phongShader->set3f("light.position",  0.0f, 1.0f, 2.0f);
    phongShader->set3f("light.ambient",  0.2f, 0.2f, 0.2f);
    phongShader->set3f("light.diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    phongShader->set3f("light.specular", 1.0f, 1.0f, 1.0f); 

    phongShader->set3f("material.ambient", 1.0f, 0.5f, 0.31f);
    phongShader->set3f("material.diffuse", 1.0f, 0.5f, 0.31f);
    phongShader->set3f("material.specular", 0.5f, 0.5f, 0.5f);
    phongShader->setFloat("material.shininess", 32.0f);
}
MainLayer::~MainLayer() {}

void MainLayer::OnUpdate(double ts)
{
    timeAcc += ts;
    frameCounter++;

    if(timeAcc >= 1.0)
    {
        timeAcc -= 1.0;
        std::cout << frameCounter << " FPS\n";
        frameCounter = 0;
    }

    ProcessInput(ts);
}
void MainLayer::OnRender()
{
    renderer->Clear();
    glm::vec2 viewportSize = Core::Application::Get().GetFramebufferSize();
    renderer->SetViewport(0, 0, viewportSize.x, viewportSize.y);

    phongShader->set3f("viewPos", camera->getPos());
    renderer->DrawIndexed(vertexArray);
}

void MainLayer::ProcessInput(double ts)
{
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
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera->ProcessMouseMovement(xoffset, yoffset);
}