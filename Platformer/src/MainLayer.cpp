#include "MainLayer.h"

// std
#include <cstddef>
#include <iostream>

// Platformer
#include "ResourceManager.h"
#include "Sprite.h"
#include "Layer.h"

//Engine
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/FramebufferManager.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/TextureManager.h"
#include "Core/Application.h"

//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// libs
#include <glm/trigonometric.hpp>

MainLayer::MainLayer() {

    Core::RendererAPI::Init();
	Core::RendererAPI::SetClearColor(glm::vec4(0.0f));
    oldFbSize = Core::Application::Get().GetFramebufferSize();

    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwGetCursorPos(Core::Application::Get().GetWindow()->GetHandle(), &lastMouseX, &lastMouseY);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Core::Application::Get().GetWindow()->GetHandle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    //##### Variables init #####
	tickAccumulator = secondsAccumulator = timer = 0.0;
	frameCounter = tickCounter = 0;

	LoadAssets();
}
MainLayer::~MainLayer() {

    Platformer::ResourceManager::ReleaseAll();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MainLayer::OnUpdate(double ts) {
    timer += ts;
    tickAccumulator += ts;
    secondsAccumulator += ts;

    if(secondsAccumulator >= 1.0) {
        PrintStats();
        tickCounter = frameCounter = 0;
        secondsAccumulator -= 1.0;
    }

    double fixedTimeStep = 1.0/TPS;
    if(tickAccumulator >= fixedTimeStep) {
        FixedUpdate(fixedTimeStep);
        tickCounter++;
        tickAccumulator -= fixedTimeStep;
    }

    frameCounter++;
}

void MainLayer::OnRender() {
    // Bind multisampled frame buffer
    Core::FramebufferManager::Bind(msaa ? multisampledFramebuffer : postFramebuffer);

    // Clear frame buffer
    Core::RendererAPI::ClearColor();
    Core::RendererAPI::ClearDepth();

    // Check viewport size and resize stuff if needed
    glm::vec2 fb = Core::Application::Get().GetFramebufferSize();
    if (oldFbSize != fb) {
        Core::RendererAPI::SetViewport(0, 0, fb.x, fb.y);
        Core::FramebufferManager::Resize(multisampledFramebuffer, fb.x, fb.y);
        Core::FramebufferManager::Resize(postFramebuffer, fb.x, fb.y);
        camera.aspectRatio = fb.x / fb.y;
        oldFbSize = fb;
    }

    // Render sprites
    for(Platformer::Sprite& sprite : sprites) {
        sprite.Render(camera);
    }

    if (msaa) {
        // Unbind previous frame buffer and render it's texture on a fullscreen quad with post-processing.
        Core::FramebufferManager::Bind(postFramebuffer);
        Core::RendererAPI::ClearColor();
        Core::RendererAPI::ClearDepth();
        Core::FramebufferManager::Blit(multisampledFramebuffer, postFramebuffer);
    }

    Core::FramebufferManager::Unbind();
    Core::RendererAPI::ClearColor();
    Core::RendererAPI::ClearDepth();

    for(Platformer::Layer& layer : layers) {
        layer.Render();
    }

    RenderGUI();
}

void MainLayer::RenderGUI() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Random cool things");
    ImGui::Text("Hemlo :3");
	ImGui::Checkbox("4xMSAA", &msaa);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainLayer::FixedUpdate(double fixedTimeStep) {
    sprites.at(0).rotation += 6.0f * fixedTimeStep;
}

void MainLayer::LoadAssets() {
    Platformer::ResourceManager::Init();

    size_t whitePixelTexture = Platformer::ResourceManager::CreatePlainRGBATexture(1, 1);
    sprites.push_back(Platformer::Sprite(whitePixelTexture, {0.0f, 0.0f}, 2.0f));

    size_t redstoneOreTexture = Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/redstone-ore.png", true, true);
    sprites.push_back(Platformer::Sprite(redstoneOreTexture, {2.0f, 0.0f}));

    size_t boxTexture = Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/box.png", true, true);
    sprites.push_back(Platformer::Sprite(boxTexture, {-2.0f, 0.0f}));

    camera = Core::OrthographicCamera();
    camera.cameraHeight = 4.0f;

    size_t multisampledFramebufferTexture = Core::TextureManager::CreateTexture(GL_TEXTURE_2D_MULTISAMPLE, GL_RGB8, oldFbSize.x, oldFbSize.y, 0, 0, 0, true, 4);
    size_t multisampledRenderbuffer = Core::RenderbufferManager::CreateRenderbuffer(GL_DEPTH24_STENCIL8, oldFbSize.x, oldFbSize.y, true, 4);
    multisampledFramebuffer = Core::FramebufferManager::CreateFramebuffer();
    Core::FramebufferManager::AttachTexture(multisampledFramebuffer, multisampledFramebufferTexture);
    Core::FramebufferManager::AttachRenderbuffer(multisampledFramebuffer, multisampledRenderbuffer);

    size_t postFramebufferTexture = Core::TextureManager::CreateTexture(GL_TEXTURE_2D, GL_RGB8, oldFbSize.x, oldFbSize.y, GL_RGB, GL_UNSIGNED_BYTE, nullptr, false, 0);
    Core::TextureManager::SetParameters(postFramebufferTexture, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    postFramebuffer = Core::FramebufferManager::CreateFramebuffer();
    Core::FramebufferManager::AttachTexture(postFramebuffer, postFramebufferTexture);

    layers.push_back(Platformer::Layer(postFramebufferTexture, Platformer::ResourceManager::GetShader(Platformer::ResourceManager::Shader::PostProcessing)));
}

void MainLayer::PrintStats() const {
    std::cout
    << "##### Stats #####\nTime : " << timer
    << "s\nFPS : " << frameCounter
    << "\nTPS : " << tickCounter
    << "\nTextures : " << Core::TextureManager::GetTexturesCount()
    << "\nVertex buffers : " << Core::VertexBufferManager::GetBuffersCount()
    << "\nIndex buffers : " << Core::IndexBufferManager::GetBuffersCount()
    << "\n";
}