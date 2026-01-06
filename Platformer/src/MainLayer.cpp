#include "MainLayer.h"

//std
#include <iostream>

//Engine
#include "RendererAPI/RendererAPI.h"
#include "Core/Application.h"

//ImGui
#include "ResourceManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

MainLayer::MainLayer() {

    Core::RendererAPI::Init();
	Core::RendererAPI::SetClearColor(glm::vec4(0.0f));
    oldFbSize = Core::Application::Get().GetFramebufferSize();

    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

}

void MainLayer::FixedUpdate(double fixedTimeStep) {

}

void MainLayer::LoadAssets() {

}

void MainLayer::PrintStats() {
    std::cout << "##### Stats #####\nTime: " << timer << "s\nFPS: " << frameCounter << "\nTPS: " << tickCounter << "\n";
}