#include "TestLayer.h"

//Engine
#include "Core/Model.h"
#include "GameObject.h"
#include "MaterialGen.h"
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/FramebufferManager.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"
#include "Core/Application.h"

//App
#include "Camera.h"
#include "ModelGen.h"

//Libs
#include "glm/fwd.hpp"

//std
#include <cstdint>
#include <iostream>
#include <vector>

// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

TestLayer::TestLayer()
{
	Core::RendererAPI::Init();
	Core::RendererAPI::SetClearColor(glm::vec4(0.0f));
    oldFbSize = Core::Application::Get().GetFramebufferSize();

    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(Core::Application::Get().GetWindow()->GetHandle(), &lastX, &lastY);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Core::Application::Get().GetWindow()->GetHandle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

	LoadAssets();
}
TestLayer::~TestLayer()
{
    Core::VertexBufferManager::ReleaseAll();
    Core::IndexBufferManager::ReleaseAll();
    Core::VertexBufferManager::ReleaseAll();
    Core::ShaderManager::ReleaseAll();
    Core::TextureManager::ReleaseAll();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void TestLayer::OnUpdate(double ts)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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

void TestLayer::OnRender()
{
    // Ensure viewport matches the current framebuffer size every frame
    glm::vec2 fb = Core::Application::Get().GetFramebufferSize();
    Core::RendererAPI::SetViewport(0, 0, (int)fb.x, (int)fb.y);

    Core::FramebufferManager::Bind(framebuffer);
    // Clear before drawing
    Core::RendererAPI::ClearColor();
    Core::RendererAPI::ClearDepth();

    for(App::GameObject& obj : gameObjects)
    {
        obj.Render(camera.coreCamera);
	}

	camera.RenderSkybox();

    Core::FramebufferManager::Unbind();
    Core::RendererAPI::ClearColor();
    Core::RendererAPI::ClearDepth();

    screenQuad.Render(camera.coreCamera);

    RenderGUI();
}

void TestLayer::RenderGUI()
{
    ImGui::Begin("ImGui test");
    ImGui::Text("Hemlo :3");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TestLayer::ProcessInput(double ts)
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
        camera.coreCamera.ProcessKeyboard(Core::FORWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera.coreCamera.ProcessKeyboard(Core::BACKWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera.coreCamera.ProcessKeyboard(Core::LEFT, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera.coreCamera.ProcessKeyboard(Core::RIGHT, ts);

    double xpos, ypos;
    glfwGetCursorPos(Core::Application::Get().GetWindow()->GetHandle(), &xpos, &ypos);
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if(mouseDisabled)
        camera.coreCamera.ProcessMouseMovement(xoffset, yoffset);
}

void TestLayer::LoadAssets()
{
	//###### Shaders ######
    blinnPhongShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong-no-shadow.frag");
    Core::ShaderManager::set3f(blinnPhongShader, "light.direction", sunLight.direction);
    Core::ShaderManager::set3f(blinnPhongShader, "light.ambient", sunLight.ambient);
    Core::ShaderManager::set3f(blinnPhongShader, "light.diffuse", sunLight.diffuse);
    Core::ShaderManager::set3f(blinnPhongShader, "light.specular", sunLight.specular);

    texturedShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/textured-no-shadow.frag");
    Core::ShaderManager::set3f(texturedShader, "light.direction", sunLight.direction);
    Core::ShaderManager::set3f(texturedShader, "light.ambient", sunLight.ambient);
    Core::ShaderManager::set3f(texturedShader, "light.diffuse", sunLight.diffuse);
    Core::ShaderManager::set3f(texturedShader, "light.specular", sunLight.specular);

    skyboxShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/skybox.vert", RESOURCES_PATH "shaders/skybox.frag");

    postProcessingShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/post.vert", RESOURCES_PATH "shaders/post.frag");
    Core::ShaderManager::setBool(postProcessingShader, "toneMapping", false);
    Core::ShaderManager::setFloat(postProcessingShader, "exposure", 1.0f);

    //###### Textures ######
    uint32_t skyboxTexture = Core::TextureManager::CreateCubemap({
        RESOURCES_PATH "textures/skybox/right.jpg",
        RESOURCES_PATH "textures/skybox/left.jpg",
        RESOURCES_PATH "textures/skybox/top.jpg",
        RESOURCES_PATH "textures/skybox/bottom.jpg",
        RESOURCES_PATH "textures/skybox/front.jpg",
        RESOURCES_PATH "textures/skybox/back.jpg"
    });

    std::vector<uint32_t> groundTextures =
    {
        Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/default.png")
    };
    Core::TextureManager::SetParameters(groundTextures.at(0), GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Core::TextureManager::GenerateMipmaps(groundTextures.at(0));

    std::vector<uint32_t> boxTextures =
    {
        Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/box.png", true),
        Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/box-specular.png", true)
    };
    Core::TextureManager::SetParameters(boxTextures.at(0), GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Core::TextureManager::GenerateMipmaps(boxTextures.at(0));

    framebufferColor = Core::TextureManager::CreateTexture(GL_TEXTURE_2D, GL_RGB8, oldFbSize.x, oldFbSize.y, GL_RGB, GL_UNSIGNED_BYTE, nullptr, false, 0);

    //###### Frame & Render buffers ######
    renderbuffer = Core::RenderbufferManager::CreateRenderbuffer(GL_DEPTH24_STENCIL8, oldFbSize.x, oldFbSize.y, false, 0);

    framebuffer = Core::FramebufferManager::CreateFramebuffer();
    Core::FramebufferManager::AttachTexture(framebuffer, framebufferColor);
    Core::FramebufferManager::AttachRenderbuffer(framebuffer, renderbuffer);

	//###### GameObjects ######
    gameObjects.push_back(App::GameObject(ModelGen::GetPlane(10, groundTextures), texturedShader));
    gameObjects.push_back(App::GameObject(ModelGen::GetQuad(), blinnPhongShader, {-1.5f ,0.5f, 0.0f}, Gold));
    gameObjects.push_back(App::GameObject(ModelGen::GetCube(), blinnPhongShader, { 0.0f ,0.5f, 0.0f}, Gold));
    gameObjects.push_back(App::GameObject(ModelGen::GetCube(boxTextures), texturedShader, {1.5f, 0.5f, 0.0f}));
    gameObjects.push_back(App::GameObject(Core::Model(RESOURCES_PATH "models/backpack/backpack.obj"), texturedShader, {0.0f, 2.0f, -2.0f}));
    
    Core::Model skyboxModel = ModelGen::GetReversedCube({ skyboxTexture });

    screenQuad = App::GameObject(ModelGen::GetQuad({ framebufferColor }), postProcessingShader);

	//###### Cameras ######
    camera = App::Camera(glm::vec3(0.0f, 1.0f, 3.0f));
	camera.SetSkybox(skyboxModel, skyboxShader);
}