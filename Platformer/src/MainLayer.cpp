#include "MainLayer.h"

//std
#include <iostream>

//Engine
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/RendererAPI.h"
#include "Core/Application.h"

//ImGui
#include "RendererAPI/TextureManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
    // Clear frame buffer
    Core::RendererAPI::ClearColor();
    Core::RendererAPI::ClearDepth();

    // Check viewport size and resize stuff if needed
    glm::vec2 fb = Core::Application::Get().GetFramebufferSize();
    if (oldFbSize != fb) {
        Core::RendererAPI::SetViewport(0, 0, (int)fb.x, (int)fb.y);
        camera.aspectRatio = fb.x / fb.y;
        oldFbSize = fb;
    }

    // Render sprites
    for(Platformer::Sprite& sprite : sprites) {
        sprite.Render(camera);
    }
}

void MainLayer::FixedUpdate(double fixedTimeStep) {

}

void MainLayer::LoadAssets() {
    Platformer::ResourceManager::Init();

    size_t whitePixelTexture = Platformer::ResourceManager::CreatePlainRGBATexture(1, 1);
    sprites.push_back(Platformer::Sprite(whitePixelTexture));

    size_t redstoneOreTexture = Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/redstone-ore.png", true);
    sprites.push_back(Platformer::Sprite(redstoneOreTexture, {2.0f, 0.0f}));

    size_t boxTexture = Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/box.png", true);
    sprites.push_back(Platformer::Sprite(boxTexture, {-2.0f, 0.0f}));

    camera = Core::OrthographicCamera();
}

void MainLayer::PrintStats() const {
    std::cout
    << "##### Stats #####\nTime : " << timer
    << "s\nFPS : " << frameCounter
    << "\nTPS : " << tickCounter
    << "\nTextures : " << Core::TextureManager::GetTextureCount()
    << "\nVertex buffers : " << Core::VertexBufferManager::GetBuffersCount()
    << "\nIndex buffers : " << Core::IndexBufferManager::GetBuffersCount()
    << "\n";
}