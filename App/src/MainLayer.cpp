#include "MainLayer.h"

#include "Core/Application.h"
#include "MeshGen.h"

// libs
#include <GLFW/glfw3.h>
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

    for (GameObject object : gameObjects)
    {
        object.Render(renderer, camera);
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
    std::shared_ptr<Core::Shader> blinnPhongShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong.frag");
    blinnPhongShader->Bind();
    blinnPhongShader->set3f("light.position", 0.0f, 3.0f, 0.0f);
    blinnPhongShader->set3f("light.ambient", 0.0f, 0.0f, 0.0f);
    blinnPhongShader->set3f("light.diffuse", 1.0f, 1.0f, 1.0f);
    blinnPhongShader->set3f("light.specular", 1.0f, 1.0f, 1.0f);

    std::shared_ptr<Core::Shader> texturedShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/textured.frag");
    texturedShader->Bind();
    texturedShader->set3f("light.position", 0.0f, 3.0f, 0.0f);
    texturedShader->set3f("light.ambient", 0.0f, 0.0f, 0.0f);
    texturedShader->set3f("light.diffuse", 1.0f, 1.0f, 1.0f);
    texturedShader->set3f("light.specular", 1.0f, 1.0f, 1.0f);

    std::shared_ptr<Core::Texture> defaultTexture = std::make_shared<Core::Texture>(RESOURCES_PATH "textures/default.png", GL_LINEAR, GL_RGBA);
	
    std::shared_ptr<Core::Texture> boxAlbedo = std::make_shared<Core::Texture>(RESOURCES_PATH "textures/box.png", GL_LINEAR, GL_RGBA);
    std::shared_ptr<Core::Texture> boxSpecular = std::make_shared<Core::Texture>(RESOURCES_PATH "textures/box-specular.png", GL_LINEAR, GL_RGBA);
    
    std::shared_ptr<Core::Texture> redstoneOreAlbedo = std::make_shared<Core::Texture>(RESOURCES_PATH "textures/redstone-ore.png", GL_NEAREST, GL_RGB);
    std::shared_ptr<Core::Texture> redstoneOreSpecular = std::make_shared<Core::Texture>(RESOURCES_PATH "textures/redstone-ore-specular.png", GL_NEAREST, GL_RGB);
    std::shared_ptr<Core::Texture> redstoneOreEmission = std::make_shared<Core::Texture>(RESOURCES_PATH "textures/redstone-ore-emission.png", GL_NEAREST, GL_RGB);

    GameObject ground = GameObject(MeshGen::GetPlane(1000), texturedShader, glm::vec3(0.0f), GameObject::ShaderType::Textured);
    ground.AddTexture(defaultTexture);
    gameObjects.push_back(ground);

	std::shared_ptr<Core::VertexArray> cubeMesh = MeshGen::GetCube();
    for (int i = 0; i < 24; i++)
    {
		gameObjects.push_back(GameObject(cubeMesh, blinnPhongShader, glm::vec3((i % 6) * 2.0f - 5.0f, 0.5f, (i / 6) * 2.0f - 3.0f), GameObject::ShaderType::BlinnPhong, static_cast<BlinnPhongMaterial>(i)));
    }

    GameObject box = GameObject(cubeMesh, texturedShader, glm::vec3(-1.0f, 0.5f, -5.0f), GameObject::ShaderType::Textured);
	box.AddTexture(boxAlbedo);
	box.AddTexture(boxSpecular);
	gameObjects.push_back(box);

    GameObject redstoneOre = GameObject(cubeMesh, texturedShader, glm::vec3(1.0f, 0.5f, -5.0f), GameObject::ShaderType::Textured);
	redstoneOre.AddTexture(redstoneOreAlbedo);
    redstoneOre.AddTexture(redstoneOreSpecular);
    redstoneOre.AddTexture(redstoneOreEmission);
	gameObjects.push_back(redstoneOre);

    camera = std::make_shared<Core::Camera>(0.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);
}