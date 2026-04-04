#include "MainLayer.h"

// std
#include <iostream>

// Platformer
#include "AssetsManager.h"
#include "Layer.h"

//Engine
#include "Renderer/Camera.h"
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

namespace Platformer {
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

        Platformer::AssetsManager::ReleaseAll();

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
        // Bind intermediaryFramebuffer
        Core::FramebufferManager::Bind(intermediaryFramebuffer);

        for (int i=0; i<16; i++) {
            Core::RendererAPI::SetViewport((int)(i%4) * RENDER_BUFFER_SCALE.x, (int)(i/4) * RENDER_BUFFER_SCALE.y, RENDER_BUFFER_SCALE.x, RENDER_BUFFER_SCALE.y);
            Core::RendererAPI::EnableScissors(true);
            Core::RendererAPI::SetScissors((int)(i%4) * RENDER_BUFFER_SCALE.x, (int)(i/4) * RENDER_BUFFER_SCALE.y, RENDER_BUFFER_SCALE.x, RENDER_BUFFER_SCALE.y);
            
            // Clear frame buffer
            Core::RendererAPI::SetClearColor(glm::vec4(i/16.0f, i/16.0f, i/16.0f, 1.0f));
            Core::RendererAPI::ClearColor();
            Core::RendererAPI::ClearDepth();

            // Render anything there is to render
            Platformer::SpritesManager::RenderSprites(camera);
        }
        Core::RendererAPI::EnableScissors(false);

        // Resize stuff
        glm::vec2 windowSize = Core::Application::Get().GetFramebufferSize();
        if (windowSize.x/windowSize.y == ASPECT_RATIO) {
            Core::RendererAPI::SetViewport(0, 0, windowSize.x, windowSize.y);
        } else if (windowSize.x/windowSize.y < ASPECT_RATIO) {
            int sizeX, sizeY;
            sizeX = windowSize.x;
            sizeX -= sizeX%4;
            sizeY = sizeX/ASPECT_RATIO;
            Core::RendererAPI::SetViewport(0, 0, sizeX, sizeY);
        } else {
            int sizeX, sizeY;
            sizeY = windowSize.y;
            sizeY -= sizeY%4;
            sizeX = sizeY*ASPECT_RATIO;
            Core::RendererAPI::SetViewport(0, 0, sizeX, sizeY);
        }

        if (oldFbSize != windowSize) {
            //Core::FramebufferManager::Resize(postFramebuffer, windowSize.x, windowSize.y);
            oldFbSize = windowSize;
        }

        Core::FramebufferManager::Unbind();
        Core::RendererAPI::ClearColor();
        Core::RendererAPI::ClearDepth();

        layers.at(0).Render();

        RenderGUI();
    }

    void MainLayer::RenderGUI() {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Random cool things");
        ImGui::Text("Hemlo :3");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void MainLayer::FixedUpdate(double fixedTimeStep) {
        SpritesManager::sprites.at(0).rotation += 10 * fixedTimeStep;
    }

    void MainLayer::LoadAssets() {
        Platformer::AssetsManager::Init();

        // Sprites
        size_t redstoneTexture = Core::TextureManager::CreateTexture(RESOURCES_PATH "textures/redstone-ore.png");
        Core::TextureManager::SetParameters(redstoneTexture, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        Platformer::SpritesManager::CreateSprite(redstoneTexture);

        // Cameras
        camera = Core::OrthographicCamera();
        camera.cameraHeight = RENDER_BUFFER_SCALE.y/16;

        // Framebuffers
        size_t intermediaryFramebufferTexture = Core::TextureManager::CreateTexture(GL_TEXTURE_2D, GL_RGBA8, RENDER_BUFFER_SCALE.x * 4, RENDER_BUFFER_SCALE.y * 4, GL_RGBA, GL_UNSIGNED_BYTE, nullptr, false, 0);
        Core::TextureManager::SetParameters(intermediaryFramebufferTexture, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        intermediaryFramebuffer = Core::FramebufferManager::CreateFramebuffer();
        Core::FramebufferManager::AttachTexture(intermediaryFramebuffer, intermediaryFramebufferTexture);

        size_t postFramebufferTexture = Core::TextureManager::CreateTexture(GL_TEXTURE_2D, GL_RGB8, oldFbSize.x, oldFbSize.y, GL_RGB, GL_UNSIGNED_BYTE, nullptr, false, 0);
        Core::TextureManager::SetParameters(postFramebufferTexture, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        postFramebuffer = Core::FramebufferManager::CreateFramebuffer();
        Core::FramebufferManager::AttachTexture(postFramebuffer, postFramebufferTexture);

        // Layers
        layers.push_back(Platformer::Layer(intermediaryFramebufferTexture, Platformer::AssetsManager::GetShader(Platformer::AssetsManager::Shader::Intermediary)));
    }

    void MainLayer::PrintStats() {
        std::cout
        << "##### Stats #####\nTime : " << timer
        << "s\nFPS : " << frameCounter
        << "\nTPS : " << tickCounter
        << "\nTextures : " << Core::TextureManager::GetTexturesCount()
        << "\nVertex buffers : " << Core::VertexBufferManager::GetBuffersCount()
        << "\nIndex buffers : " << Core::IndexBufferManager::GetBuffersCount()
        << "\nSprites : " << Platformer::SpritesManager::GetSpritesCount()
        << "\n";
    }
}