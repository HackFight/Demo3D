#include "MainLayer.h"

#include "GLFW/glfw3.h"
#include "Core/Application.h"
#include "MeshGen.h"
#include "MaterialGen.h"
#include "RendererAPI/RendererAPI.h"
#include "Renderer/Camera.h"

// libs
#include <GL/gl.h>

// std
#include <iostream>
#include <memory>

MainLayer::MainLayer()
{
    // Setup
    renderer = std::make_shared<Core::RendererAPI>();
    renderer->Init();
    renderer->SetClearColor(glm::vec4(0.0f));

    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    double xpos, ypos;
    glfwGetCursorPos(Core::Application::Get().GetWindow()->GetHandle(), &xpos, &ypos);
    lastX = xpos; lastY = ypos;

    LoadAssets();
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

    blinnPhongShader->set3f("viewPos", camera->getPos());
    blinnPhongShader->setmat4("viewMat", camera->getViewMatrix());
    blinnPhongShader->setmat4("projMat", camera->getProjectionMatrix());
    for (GameObject object : gameObjects)
    {
        object.Render(renderer.get());
    }
}

void MainLayer::ProcessInput(double ts)
{
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS && canPress)
    {
        mouseDisabled ? glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL) : glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouseDisabled = !mouseDisabled;
        canPress = false;
    }
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_ESCAPE) == GLFW_RELEASE)
        canPress = true;
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

void MainLayer::LoadAssets()
{
    blinnPhongShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong.frag");

    gameObjects.push_back(GameObject(MeshGen::GetPlane(10), blinnPhongShader, CyanPlastic));

    for (int i = 0; i < 24; i++)
    {
		gameObjects.push_back(GameObject(MeshGen::GetCube(), blinnPhongShader, static_cast<BlinnPhongMaterial>(i), glm::vec3((i % 6) * 2.0f - 5.0f, 0.5f, (i / 6) * 2.0f - 3.0f)));
    }

    camera = std::make_shared<Core::Camera>(0.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);

    blinnPhongShader->Bind();
    blinnPhongShader->set3f("light.direction", -1.0f, -3.0f, -2.0f);
    blinnPhongShader->set3f("light.ambient", 1.0f, 1.0f, 1.0f);
    blinnPhongShader->set3f("light.diffuse", 1.0f, 1.0f, 1.0f);
    blinnPhongShader->set3f("light.specular", 1.0f, 1.0f, 1.0f);

    MaterialGen::setBlinnPhongMaterial(blinnPhongShader.get(), CyanPlastic);
}