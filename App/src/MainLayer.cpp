#include "MainLayer.h"

#include "Core/Application.h"
#include <ModelGen.h>
#include <MeshGen.h>

// std
#include <iostream>
#include <memory>
#include <vector>

// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
MainLayer::~MainLayer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MainLayer::OnUpdate(double ts)
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
void MainLayer::OnRender()
{
    renderer->SRGBColorSpace(false); // disble gamma correction for intermediate steps
	RenderShadowMap();

	RenderScene();

    if (gammaCorrection)
        renderer->SRGBColorSpace(true); // enable gamma correction for final render
    RenderPostProcessing();

    RenderGUI();
}

void MainLayer::RenderShadowMap()
{
	renderer->SetViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);

    shadowDepthMapFramebuffer->Bind();
    renderer->ClearDepth();

    lightCam->coreCamera->setPos(-sunLight.direction * 10.0f + camera->coreCamera->getPos());
    lightSpaceMat = lightCam->coreCamera->getProjectionMatrix() * lightCam->coreCamera->getViewMatrix();

    shadowShader->Bind();
    shadowShader->setmat4("lightSpaceMat", lightSpaceMat);
    for (GameObject object : gameObjects)
    {
        object.Render(lightCam->coreCamera, shadowShader);
    }
}

void MainLayer::RenderScene()
{
    // Resize framebuffer attachments if needed
    glm::vec2 viewportSize = Core::Application::Get().GetFramebufferSize();
    renderer->SetViewport(0, 0, viewportSize.x, viewportSize.y );
    textureColorBuffer->SetData(0x881A, viewportSize.x, viewportSize.y, GL_RGB, NULL);
    renderbuffer->SetData(viewportSize.x, viewportSize.y);

    // Render scene in framebuffer
    framebuffer->Bind();
    renderer->ClearColor();
    renderer->ClearDepth();

    blinnPhongShader->Bind();
    blinnPhongShader->setmat4("lightSpaceMat", lightSpaceMat);

    texturedShader->Bind();
    texturedShader->setmat4("lightSpaceMat", lightSpaceMat);
    texturedShader->Unbind();

    for (GameObject object : gameObjects)
    {
        object.Render(camera->coreCamera);
    }

    camera->RenderSkybox();
}

void MainLayer::RenderPostProcessing()
{
    framebuffer->Unbind();
    renderer->ClearColor();
    renderer->DepthTest(false);

	postProcessingShader->Bind();
	postProcessingShader->setBool("toneMapping", toneMapping);
	postProcessingShader->setFloat("exposure", exposure);
    screenQuad->Render(camera->coreCamera);

    renderer->DepthTest(true);
}

void MainLayer::RenderGUI()
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
    if(mouseDisabled)
        camera->coreCamera->ProcessMouseMovement(xoffset, yoffset);
}

void MainLayer::LoadAssets()
{
	// load and setup the blinn-phong shader
    blinnPhongShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong.frag");
    blinnPhongShader->Bind();
    blinnPhongShader->set3f("light.direction", sunLight.direction);
    blinnPhongShader->set3f("light.ambient", sunLight.ambient);
    blinnPhongShader->set3f("light.diffuse", sunLight.diffuse);
    blinnPhongShader->set3f("light.specular", sunLight.specular);
    blinnPhongShader->setInt("shadowMap", 5);

	// load and setup the textured shader
    texturedShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/textured.frag");
    texturedShader->Bind();
    texturedShader->set3f("light.direction", sunLight.direction);
    texturedShader->set3f("light.ambient", sunLight.ambient);
    texturedShader->set3f("light.diffuse", sunLight.diffuse);
    texturedShader->set3f("light.specular", sunLight.specular);
    texturedShader->setInt("shadowMap", 5);

    std::shared_ptr<Core::Shader> skyboxShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/skybox.vert", RESOURCES_PATH "shaders/skybox.frag");

    postProcessingShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/post.vert", RESOURCES_PATH "shaders/post.frag");
    
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
        Core::Texture::Create(RESOURCES_PATH "textures/default.png", 0x8C40, GL_RGBA)
    };
	defaultTextures[0]->SetParameters(GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	defaultTextures[0]->GenerateMipmaps();

	// load the box textures
    std::vector<std::shared_ptr<Core::Texture>> boxTextures
    {
        Core::Texture::Create(RESOURCES_PATH "textures/box.png", 0x8C40, GL_RGBA),
        Core::Texture::Create(RESOURCES_PATH "textures/box-specular.png", GL_RGB, GL_RGBA)
    };
	boxTextures[0]->SetParameters(GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	boxTextures[0]->GenerateMipmaps();

	// load the redstone ore textures
    std::vector<std::shared_ptr<Core::Texture>> redstoneOreTextures
    {
        Core::Texture::Create(RESOURCES_PATH "textures/redstone-ore.png", 0x8C40, GL_RGB),
        Core::Texture::Create(RESOURCES_PATH "textures/redstone-ore-specular.png", GL_RGB, GL_RGB),
        Core::Texture::Create(RESOURCES_PATH "textures/redstone-ore-emission.png", 0x8C40, GL_RGB)
    };
	redstoneOreTextures[0]->SetParameters(GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
	redstoneOreTextures[0]->GenerateMipmaps();
    redstoneOreTextures[1]->SetParameters(GL_REPEAT, GL_NEAREST, GL_NEAREST);
    redstoneOreTextures[2]->SetParameters(GL_REPEAT, GL_NEAREST, GL_NEAREST);

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
    gameObjects.push_back(GameObject(Core::Model::Create(RESOURCES_PATH "models/vyse-helmet/vyse-helmet.obj"), blinnPhongShader, glm::vec3(0.0f, 1.0f, -7.0f), GameObject::ShaderType::BlinnPhong, FLASHBANG));

	// setup the camera
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    camera->SetSkybox(MeshGen::GetReversedCube(), std::make_shared<Core::Texture>(faces), skyboxShader);

    lightCam = std::make_unique<Camera>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
    lightCam->coreCamera->orthographic = true;
    lightCam->coreCamera->setPos(-sunLight.direction * 10.0f + camera->coreCamera->getPos());
    lightCam->coreCamera->lookAt(camera->coreCamera->getPos());

    // setup post processing
    textureColorBuffer = Core::Texture::Create(0x881A, 1920, 1200, GL_RGBA, NULL);
	textureColorBuffer->SetParameters(GL_REPEAT, GL_LINEAR, GL_LINEAR);

    std::vector<std::shared_ptr<Core::Texture>> tempVec;
    tempVec.push_back(textureColorBuffer);

    renderbuffer = Core::Renderbuffer::Create(Core::Depth_Stencil, 1920, 1200);
    
    framebuffer = Core::Framebuffer::Create();
    framebuffer->AttachTexture(Core::AttachementType::Color, textureColorBuffer);
    framebuffer->AttachRenderBuffer(renderbuffer);

    screenQuad = std::make_shared<GameObject>(ModelGen::GetQuad(tempVec), postProcessingShader);

	// setup shadow mapping framebuffer
    shadowTexture = Core::Texture::Create(GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, GL_DEPTH_COMPONENT, NULL);
    shadowTexture->SetParameters(0x812D, GL_LINEAR, GL_LINEAR);
    shadowTexture->SetBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
	shadowTexture->Bind(5); // No texture should interfere with the shadow map texture unit

    shadowDepthMapFramebuffer = Core::Framebuffer::Create();
    shadowDepthMapFramebuffer->AttachTexture(Core::AttachementType::Depth, shadowTexture);
    
    shadowShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/empty.frag");
}