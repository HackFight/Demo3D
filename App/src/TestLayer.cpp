#include "TestLayer.h"

//Engine
#include "Core/Model.h"
#include "GameObject.h"
#include "MaterialGen.h"
#include "Renderer/FlyCam.h"
#include "Renderer/OrthographicCamera.h"
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/FramebufferManager.h"
#include "RendererAPI/RendererAPI.h"
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
    Core::RenderbufferManager::ReleaseAll();
    Core::FramebufferManager::ReleaseAll();

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
    // Render shadowmap
    Core::RendererAPI::SetViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);

    Core::FramebufferManager::Bind(shadowbuffer);
    Core::RendererAPI::ClearDepth();

    glm::mat4 lightSpaceMat = lightCamera.getProjectionMatrix() * lightCamera.getViewMatrix();

    Core::ShaderManager::setmat4(shadowShader, "lightSpaceMat", lightSpaceMat);
    for(App::GameObject& obj : gameObjects)
    {
        obj.Render(lightCamera, shadowShader);
	}

    // Ensure viewport matches the current framebuffer size every frame
    glm::vec2 fb = Core::Application::Get().GetFramebufferSize();
    Core::RendererAPI::SetViewport(0, 0, (int)fb.x, (int)fb.y);

    // Resize the buffer if needed
    if(oldFbSize != fb)
    {
        Core::TextureManager::Resize(framebufferColor, fb.x, fb.y);
        Core::RenderbufferManager::Resize(renderbuffer, fb.x, fb.y);
        camera.aspectRatio = fb.x / fb.y;
        oldFbSize = fb;
    }

    Core::FramebufferManager::Bind(framebuffer);
    // Clear before drawing
    Core::RendererAPI::ClearColor();
    Core::RendererAPI::ClearDepth();
    
    Core::ShaderManager::setmat4(blinnPhongShader, "lightSpaceMat", lightSpaceMat);
    Core::ShaderManager::setmat4(texturedShader, "lightSpaceMat", lightSpaceMat);
    Core::ShaderManager::setBool(postProcessingShader, "gammaCorrection", gammaCorrection);
	Core::ShaderManager::setBool(postProcessingShader, "toneMapping", toneMapping);
	Core::ShaderManager::setFloat(postProcessingShader, "exposure", exposure);

    Core::TextureManager::Bind(shadowmap, SHADOWMAP_TEXTURE_UNIT);

    for(App::GameObject& obj : gameObjects)
    {
        obj.Render(camera);
	}

	camera.RenderSkybox();

    Core::FramebufferManager::Unbind();
    Core::RendererAPI::ClearColor();
    Core::RendererAPI::ClearDepth();

    screenQuad.Render(camera);

    RenderGUI();
}

void TestLayer::RenderGUI()
{
    ImGui::Begin("ImGui test");
    ImGui::Text("Hemlo :3");
	ImGui::Checkbox("Gamma Correction", &gammaCorrection);
	ImGui::Checkbox("Tone Mapping", &toneMapping);
	ImGui::SliderFloat("Exposure", &exposure, 0.1f, 5.0f);
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
        camera.ProcessKeyboard(Core::FORWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Core::BACKWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Core::LEFT, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Core::RIGHT, ts);

    double xpos, ypos;
    glfwGetCursorPos(Core::Application::Get().GetWindow()->GetHandle(), &xpos, &ypos);
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if(mouseDisabled)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

void TestLayer::LoadAssets()
{
	//###### Shaders ######
    blinnPhongShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong.frag");
    Core::ShaderManager::set3f(blinnPhongShader, "light.direction", sunLight.direction);
    Core::ShaderManager::set3f(blinnPhongShader, "light.ambient", sunLight.ambient);
    Core::ShaderManager::set3f(blinnPhongShader, "light.diffuse", sunLight.diffuse);
    Core::ShaderManager::set3f(blinnPhongShader, "light.specular", sunLight.specular);
    Core::ShaderManager::setInt(blinnPhongShader, "shadowMap", SHADOWMAP_TEXTURE_UNIT);

    texturedShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/textured.frag");
    Core::ShaderManager::set3f(texturedShader, "light.direction", sunLight.direction);
    Core::ShaderManager::set3f(texturedShader, "light.ambient", sunLight.ambient);
    Core::ShaderManager::set3f(texturedShader, "light.diffuse", sunLight.diffuse);
    Core::ShaderManager::set3f(texturedShader, "light.specular", sunLight.specular);
    Core::ShaderManager::setInt(texturedShader, "shadowMap", SHADOWMAP_TEXTURE_UNIT);

    skyboxShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/skybox.vert", RESOURCES_PATH "shaders/skybox.frag");

    postProcessingShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/post.vert", RESOURCES_PATH "shaders/post.frag");
    Core::ShaderManager::setBool(postProcessingShader, "gammaCorrection", gammaCorrection);
    Core::ShaderManager::setBool(postProcessingShader, "toneMapping", toneMapping);
    Core::ShaderManager::setFloat(postProcessingShader, "exposure", 1.0f);

    shadowShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/empty.frag");

	normalsDebug = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/normal.frag");

    //###### Textures ######
    std::vector<GLubyte> emptyData(128 * 128 * 4, 0);
    uint32_t emptyTexture = Core::TextureManager::CreateTexture(GL_TEXTURE_2D, GL_RGBA8, 128, 128, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0], false, 0);

    uint32_t skyboxTexture = Core::TextureManager::CreateCubemap({
        RESOURCES_PATH "textures/skyboxes/default/right.jpg",
        RESOURCES_PATH "textures/skyboxes/default/left.jpg",
        RESOURCES_PATH "textures/skyboxes/default/top.jpg",
        RESOURCES_PATH "textures/skyboxes/default/bottom.jpg",
        RESOURCES_PATH "textures/skyboxes/default/front.jpg",
        RESOURCES_PATH "textures/skyboxes/default/back.jpg"
    });

    std::vector<uint32_t> groundTextures =
    {
        Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/default.png")
    };
    Core::TextureManager::SetParameters(groundTextures.at(0), GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Core::TextureManager::GenerateMipmaps(groundTextures.at(0));

    std::vector<uint32_t> wallTextures =
    {
        Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/brick-wall.png"),
        emptyTexture,
        emptyTexture,
        Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/brick-wall-normal.png")
    };
    Core::TextureManager::SetParameters(wallTextures.at(0), GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Core::TextureManager::GenerateMipmaps(wallTextures.at(0));

    std::vector<uint32_t> boxTextures =
    {
        Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/box.png", true),
        Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/box-specular.png", true)
    };
    Core::TextureManager::SetParameters(boxTextures.at(0), GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Core::TextureManager::GenerateMipmaps(boxTextures.at(0));

    framebufferColor = Core::TextureManager::CreateTexture(GL_TEXTURE_2D, GL_RGB16F, oldFbSize.x, oldFbSize.y, GL_RGB, GL_FLOAT, nullptr, false, 0);

	shadowmap = Core::TextureManager::CreateTexture(GL_TEXTURE_2D, GL_DEPTH_COMPONENT24, SHADOW_SIZE, SHADOW_SIZE, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr, false, 0);
    Core::TextureManager::Bind(shadowmap, SHADOWMAP_TEXTURE_UNIT);

    //###### Frame & Render buffers ######
    renderbuffer = Core::RenderbufferManager::CreateRenderbuffer(GL_DEPTH24_STENCIL8, oldFbSize.x, oldFbSize.y, false, 0);

    framebuffer = Core::FramebufferManager::CreateFramebuffer();
    Core::FramebufferManager::AttachTexture(framebuffer, framebufferColor);
    Core::FramebufferManager::AttachRenderbuffer(framebuffer, renderbuffer);

	shadowbuffer = Core::FramebufferManager::CreateFramebuffer();
	Core::FramebufferManager::AttachTexture(shadowbuffer, shadowmap);

	//###### GameObjects ######
    gameObjects.push_back(App::GameObject(ModelGen::GetPlane(10, groundTextures), texturedShader));
    gameObjects.push_back(App::GameObject(ModelGen::GetQuad(wallTextures), texturedShader, {-1.5f ,0.5f, 0.0f}, {0.5f, 0.5f, 0.5f}, Gold));
    gameObjects.push_back(App::GameObject(ModelGen::GetCube(), blinnPhongShader, { 0.0f ,0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, Gold));
    gameObjects.push_back(App::GameObject(Core::Model(RESOURCES_PATH "models/vyse-helmet/vyse-helmet.obj"), blinnPhongShader, { 0.0f ,1.5f, 0.0f }, {1.0f, 1.0f, 1.0f}, FLASHBANG));
    gameObjects.push_back(App::GameObject(ModelGen::GetCube(boxTextures), texturedShader, {1.5f, 0.5f, 0.0f}));
    //gameObjects.push_back(App::GameObject(Core::Model(RESOURCES_PATH "models/backpack/backpack.obj"), texturedShader, {0.0f, 2.0f, -2.0f}));
	gameObjects.push_back(App::GameObject(Core::Model(RESOURCES_PATH "models/sponza/sponza.obj"), texturedShader, { 0.0f, 0.0f, 0.0f }));
	gameObjects.back().m_Scale = glm::vec3(0.01f);

    Core::Model skyboxModel = ModelGen::GetReversedCube({ skyboxTexture });

    screenQuad = App::GameObject(ModelGen::GetQuad({ framebufferColor }), postProcessingShader);

	//###### Cameras ######
    camera = App::Camera(glm::vec3(0.0f, 1.0f, 3.0f));
	camera.SetSkybox(skyboxModel, skyboxShader);

    lightCamera = Core::OrthographicCamera(-sunLight.direction * 10.0f);
    lightCamera.cameraHeight=1000.0f;
    lightCamera.lookAt({0.0f, 0.0f, 0.0f});
}