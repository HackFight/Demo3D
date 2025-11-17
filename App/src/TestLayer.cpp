#include "TestLayer.h"

//Engine
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"
#include "Core/Application.h"

//App
#include "Camera.h"
#include "MaterialGen.h"

//Libs
#include "glm/fwd.hpp"

//std
#include <iostream>
#include <vector>

// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

TestLayer::TestLayer()
{
	Core::RendererAPI::Init();
	Core::RendererAPI::SetClearColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

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

    std::vector<Core::Vertex> vertices
    {
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},

        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},

        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},

        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}
    };
    std::vector<uint32_t> indices
    {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8,  9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };
    vertexBuffer = Core::VertexBufferManager::CreateVertexBuffer((float*) vertices.data(), vertices.size() * sizeof(Core::Vertex));
    indexBuffer = Core::IndexBufferManager::CreateIndexBuffer(indices.data(), indices.size());
    vertexArray = Core::VertexArrayManager::CreateVertexArray();
    Core::VertexArrayManager::AddVertexBuffer(vertexArray, vertexBuffer);
    Core::VertexArrayManager::SetIndexBuffer(vertexArray, indexBuffer);

    shader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong-no-shadow.frag");
    Core::ShaderManager::set3f(shader, "light.direction", glm::vec3(-1.0f, -1.0f, -1.0f));
    Core::ShaderManager::set3f(shader, "light.ambient", glm::vec3(0.0f));
    Core::ShaderManager::set3f(shader, "light.diffuse", glm::vec3(1.0f));
    Core::ShaderManager::set3f(shader, "light.specular", glm::vec3(2.0f));
    MaterialGen::setBlinnPhongMaterial(shader, CyanPlastic);

	camera = App::Camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
		Core::TextureManager::DebugPrintSummary();
        frameCounter = 0;
    }

    ProcessInput(ts);
}

void TestLayer::OnRender()
{
    // Ensure viewport matches the current framebuffer size every frame
    glm::vec2 fb = Core::Application::Get().GetFramebufferSize();
    Core::RendererAPI::SetViewport(0, 0, (int)fb.x, (int)fb.y);

    // Clear before drawing
    Core::RendererAPI::ClearColor();
    Core::RendererAPI::ClearDepth();

    Core::ShaderManager::setmat4(shader, "viewMat", camera.coreCamera.getViewMatrix());
    Core::ShaderManager::setmat4(shader, "projMat", camera.coreCamera.getProjectionMatrix());
    Core::ShaderManager::setmat4(shader, "modelMat", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));

    Core::ShaderManager::setInt(shader, "material.texture_diffuse1", 0);
    Core::ShaderManager::setInt(shader, "material.texture_specular1", 1);
    Core::ShaderManager::setInt(shader, "material.texture_emission1", 2);

    Core::ShaderManager::set3f(shader, "viewPos", camera.coreCamera.getPos());
    Core::ShaderManager::Bind(shader);
    Core::RendererAPI::DrawIndexed(vertexArray);

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