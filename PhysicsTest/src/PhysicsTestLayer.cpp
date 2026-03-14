#include "PhysicsTestLayer.h"

//Engine
#include "Core/Model.h"
#include "GameObject.h"
#include "MaterialGen.h"
#include "PhysicsConstraints.h"
#include "RendererAPI/BufferManager.h"
#include "RendererAPI/FramebufferManager.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"
#include "Core/Application.h"

//App
#include "Camera.h"
#include "ModelGen.h"
#include "SoftBody.h"

//Libs
#include "glm/fwd.hpp"

//std
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

PhysicsTestLayer::PhysicsTestLayer()
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
PhysicsTestLayer::~PhysicsTestLayer()
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

void PhysicsTestLayer::OnUpdate(double ts)
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

    jellyCube.m_Model.m_Constraints.at(0) = std::make_shared<App::GroundConstraint>(std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 7}, -jellyCube.m_Position.y);
    jellyCube.m_Model.Update(ts, 10);
	jellyCube.m_Model.UpdateGPUBuffer();

    ProcessInput(ts);
}

void PhysicsTestLayer::OnRender()
{
    // Render shadowmap
    Core::RendererAPI::SetViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);

    Core::FramebufferManager::Bind(shadowbuffer);
    Core::RendererAPI::ClearDepth();

    glm::mat4 lightSpaceMat = lightCamera.coreCamera.getProjectionMatrix() * lightCamera.coreCamera.getViewMatrix();

    Core::ShaderManager::setmat4(shadowShader, "lightSpaceMat", lightSpaceMat);
    for(App::GameObject& obj : gameObjects)
    {
        obj.Render(lightCamera.coreCamera, shadowShader);
	}

    // Ensure viewport matches the current framebuffer size every frame
    glm::vec2 fb = Core::Application::Get().GetFramebufferSize();
    Core::RendererAPI::SetViewport(0, 0, (int)fb.x, (int)fb.y);

    // Resize the buffer if needed
    if(oldFbSize != fb)
    {
        Core::TextureManager::Resize(framebufferColor, fb.x, fb.y);
        Core::RenderbufferManager::Resize(renderbuffer, fb.x, fb.y);
        camera.coreCamera.aspectRatio = fb.x / fb.y;
        oldFbSize = fb;
    }

    //Render scene
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
        obj.Render(camera.coreCamera);
	}

    /*
    Core::ShaderManager::Bind(normalsShader);
    Core::RendererAPI::DrawPoints(gameObjects.at(1).GetModel().GetMesh(0).GetVertexArray(),Core::VertexArrayManager::GetIndexCount(gameObjects.at(1).GetModel().GetMesh(0).GetVertexArray()));
    */

    camera.RenderSkybox();

    //Final pass
    Core::FramebufferManager::Unbind();
    Core::RendererAPI::ClearColor();
    Core::RendererAPI::ClearDepth();

    screenQuad.Render(camera.coreCamera);

    RenderGUI();
}

void PhysicsTestLayer::RenderGUI()
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

void PhysicsTestLayer::ProcessInput(double ts)
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
        camera.ProcessKeyboard(App::FORWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(App::BACKWARD, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(App::LEFT, ts);
    if (glfwGetKey(Core::Application::Get().GetWindow()->GetHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(App::RIGHT, ts);

    double xpos, ypos;
    glfwGetCursorPos(Core::Application::Get().GetWindow()->GetHandle(), &xpos, &ypos);
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if(mouseDisabled)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

void PhysicsTestLayer::LoadAssets()
{
	//###### Shaders ######
    blinnPhongShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong-no-shadow.frag");
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

    //normalsShader = Core::ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH  "shaders/default.frag", RESOURCES_PATH "shaders/normals.geom");

    //###### Textures ######
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

    std::vector<Core::Vertex> cubeVertices =
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
    std::vector<uint32_t> cubeIndices
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
    std::vector<std::vector<size_t>> pointsAttach
    {
        {0, 13, 19},
        {1, 4, 18},
        {5, 8, 17},
        {9, 12, 16},

        {3, 14, 20},
        {2, 7, 21},
        {6, 11, 22},
		{10, 15, 23}
	};
    std::vector<App::PointMass> physicsPoints
    {
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 10.0f, 0.0f}, 1.0f},
        {{ 0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, 1.0f},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, 1.0f},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, 1.0f},

        {{ -0.5f, 0.5f, 0.5f}, {0.0f, 10.0f, 0.0f}, 1.0f},
        {{ 0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, 1.0f},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, 1.0f},
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, 1.0f}
    };
    std::vector<size_t> indices
    {
        0, 1, 2, 3, 4, 5, 6, 7
    };
    std::vector<std::shared_ptr<App::Constraint>> constraints;
    constraints.push_back(std::make_shared<App::GroundConstraint>(indices));

    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{0, 1}, 1.0f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{1, 2}, 1.0f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{2, 3}, 1.0f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{3, 0}, 1.0f));

    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{4, 5}, 1.0f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{5, 6}, 1.0f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{6, 7}, 1.0f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{7, 4}, 1.0f));

    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{0, 4}, 1.0f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{1, 5}, 1.0f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{2, 6}, 1.0f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{3, 7}, 1.0f));

    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{0, 2}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{4, 6}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{0, 5}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{1, 6}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{2, 7}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{3, 4}, 1.4142135f));

    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{1, 3}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{5, 7}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{1, 4}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{2, 5}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{3, 6}, 1.4142135f));
    constraints.push_back(std::make_shared<App::DistanceConstraint>(std::vector<size_t>{0, 7}, 1.4142135f));

	jellyCube = App::SoftBody(
        App::SoftBodyModel(
            cubeVertices,
            cubeIndices,
			pointsAttach,
			physicsPoints,
            constraints),
        blinnPhongShader,
        { 0.0f ,0.5f, 0.0f },
        CyanPlastic);
	gameObjects.push_back(jellyCube);
    
    Core::Model skyboxModel = ModelGen::GetReversedCube({ skyboxTexture });

    screenQuad = App::GameObject(ModelGen::GetQuad({ framebufferColor }), postProcessingShader);

	//###### Cameras ######
    camera = App::Camera(glm::vec3(0.0f, 1.0f, 3.0f));
	camera.SetSkybox(skyboxModel, skyboxShader);

    lightCamera = App::Camera(-sunLight.direction * 10.0f);
	//lightCamera.coreCamera.orthographic = true;
    lightCamera.coreCamera.lookAt({0.0f, 0.0f, 0.0f});
}