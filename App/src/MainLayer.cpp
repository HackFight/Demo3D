#include "MainLayer.h"

#include "Core/Application.h"
#include "Core/Model.h"
#include "GameObject.h"
#include "MeshGen.h"
#include "ModelGen.h"
#include "RendererAPI/Framebuffer.h"
#include "RendererAPI/Texture.h"

// libs
#include <GL/glext.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>
#include <memory>
#include <vector>

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
    glm::vec2 viewportSize = Core::Application::Get().GetFramebufferSize();
    renderer->SetViewport(0, 0, viewportSize.x, viewportSize.y);
    textureColorBuffer->SetData(GL_RGB, viewportSize.x, viewportSize.y, GL_LINEAR, GL_RGB, NULL);
    renderbuffer->SetData(viewportSize.x, viewportSize.y);

    // Render scene in framebuffer
    renderer->SRGBColorSpace(false); // disble gamma correction for intermediate steps
    framebuffer->Bind();
    renderer->ClearColor();
    renderer->ClearDepth();
    renderer->DepthTest(true);

    for (GameObject object : gameObjects)
    {
        object.Render(camera->coreCamera);
    }

    camera->RenderSkybox();

    // Render quad with framebuffer texture
    renderer->SRGBColorSpace(true); // enable gamma correction for final render
    framebuffer->Unbind();
    renderer->ClearColor();
    renderer->DepthTest(false);
    screenQuad->Render(camera->coreCamera);
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
        camera->coreCamera->ProcessKeyboard(Core::FORWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera->coreCamera->ProcessKeyboard(Core::BACKWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera->coreCamera->ProcessKeyboard(Core::LEFT, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera->coreCamera->ProcessKeyboard(Core::RIGHT, ts);

    double xpos, ypos;
    glfwGetCursorPos(Core::Application::Get().GetWindow()->GetHandle(), &xpos, &ypos);
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera->coreCamera->ProcessMouseMovement(xoffset, yoffset);
}

void MainLayer::LoadAssets()
{
	// load and setup the blinn-phong shader
    std::shared_ptr<Core::Shader> blinnPhongShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong.frag");
    blinnPhongShader->Bind();
    blinnPhongShader->set3f("light.direction", 1.0f, -3.0f, 1.0f);
    blinnPhongShader->set3f("light.ambient", 0.5f, 0.5f, 0.5f);
    blinnPhongShader->set3f("light.diffuse", 1.0f, 1.0f, 1.0f);
    blinnPhongShader->set3f("light.specular", 1.0f, 1.0f, 1.0f);

	// load and setup the textured shader
    std::shared_ptr<Core::Shader> texturedShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/textured.frag");
    texturedShader->Bind();
    texturedShader->set3f("light.direction", 1.0f, -3.0f, 1.0f);
    texturedShader->set3f("light.ambient", 0.5f, 0.5f, 0.5f);
    texturedShader->set3f("light.diffuse", 1.0f, 1.0f, 1.0f);
    texturedShader->set3f("light.specular", 1.0f, 1.0f, 1.0f);

    std::shared_ptr<Core::Shader> skyboxShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/skybox.vert", RESOURCES_PATH "shaders/skybox.frag");

    std::shared_ptr<Core::Shader> postProcessingShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/post.vert", RESOURCES_PATH "shaders/post.frag");
    
    std::vector<const char*> faces
    {
        RESOURCES_PATH "textures/skybox/right.jpg",
        RESOURCES_PATH "textures/skybox/left.jpg",
        RESOURCES_PATH "textures/skybox/top.jpg",
        RESOURCES_PATH "textures/skybox/bottom.jpg",
        RESOURCES_PATH "textures/skybox/front.jpg",
        RESOURCES_PATH "textures/skybox/back.jpg"
    };

    // load the default texture
    std::vector<std::shared_ptr<Core::Texture>> defaultTextures
    {
        std::make_shared<Core::Texture>(RESOURCES_PATH "textures/default.png", GL_SRGB, GL_LINEAR, GL_RGBA)
    };

	// load the box textures
    std::vector<std::shared_ptr<Core::Texture>> boxTextures
    {
    std::make_shared<Core::Texture>(RESOURCES_PATH "textures/box.png", GL_SRGB, GL_LINEAR, GL_RGBA),
    std::make_shared<Core::Texture>(RESOURCES_PATH "textures/box-specular.png", GL_RGB, GL_LINEAR, GL_RGBA)
    };

	// load the redstone ore textures
    std::vector<std::shared_ptr<Core::Texture>> redstoneOreTextures
    {
        std::make_shared<Core::Texture>(RESOURCES_PATH "textures/redstone-ore.png", GL_SRGB, GL_NEAREST, GL_RGB),
        std::make_shared<Core::Texture>(RESOURCES_PATH "textures/redstone-ore-specular.png", GL_RGB, GL_NEAREST, GL_RGB),
        std::make_shared<Core::Texture>(RESOURCES_PATH "textures/redstone-ore-emission.png", GL_SRGB, GL_NEAREST, GL_RGB)
    };

	// create the ground plane
    gameObjects.push_back(GameObject(ModelGen::GetPlane(1000, defaultTextures), texturedShader, glm::vec3(0.0f), GameObject::ShaderType::Default));

	// create cubes with different materials
    for (int i = 0; i < 24; i++)
    {
		gameObjects.push_back(GameObject(ModelGen::GetCube(), blinnPhongShader, glm::vec3((i % 6) * 2.0f - 5.0f, 0.5f, (i / 6) * 2.0f - 3.0f), GameObject::ShaderType::BlinnPhong, static_cast<BlinnPhongMaterial>(i)));
    }

    // create the textured box
	gameObjects.push_back(GameObject(ModelGen::GetCube(boxTextures), texturedShader, glm::vec3(-1.0f, 0.5f, -5.0f), GameObject::ShaderType::Default));

	// create the redstone ore
	gameObjects.push_back(GameObject(ModelGen::GetCube(redstoneOreTextures), texturedShader, glm::vec3(1.0f, 0.5f, -5.0f), GameObject::ShaderType::Default));

	// create the backpack
	gameObjects.push_back(GameObject(Core::Model::Create(RESOURCES_PATH "models/backpack/backpack.obj"), texturedShader, glm::vec3(3.0f, 2.0f, -7.0f), GameObject::ShaderType::Default));

	// create the Vyse helmet
    gameObjects.push_back(GameObject(Core::Model::Create(RESOURCES_PATH "models/vyse-helmet/vyse-helmet.obj"), blinnPhongShader, glm::vec3(0.0f, 1.0f, -7.0f)));

	// setup the camera
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    camera->SetSkybox(MeshGen::GetReversedCube(), std::make_shared<Core::Texture>(faces), skyboxShader);

    // setup post processing
    textureColorBuffer = Core::Texture::Create(GL_RGB, 1920, 1200, GL_LINEAR, GL_RGB, NULL);

    std::vector<std::shared_ptr<Core::Texture>> temp;
    temp.push_back(textureColorBuffer);

    renderbuffer = Core::Renderbuffer::Create(Core::Depth_Stencil, 1920, 1200);
    
    framebuffer = Core::Framebuffer::Create();
    framebuffer->AttachTexture(textureColorBuffer);
    framebuffer->AttachRenderBuffer(renderbuffer);

    screenQuad = std::make_shared<GameObject>(ModelGen::GetQuad(temp), postProcessingShader);
}