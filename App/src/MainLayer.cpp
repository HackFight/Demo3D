#include "MainLayer.h"

// std
#include <iostream>
#include <vector>
#include <cstdint>

// imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Engine
#include "Core/Application.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/FramebufferManager.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"

// App
#include "ModelGen.h"
#include "MeshGen.h"

using namespace Core;

MainLayer::MainLayer()
{
    // Setup
    RendererAPI::Init();
    RendererAPI::SetClearColor(glm::vec4(0.0f));
    
    glfwSetInputMode(Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    double xpos, ypos;
    glfwGetCursorPos(Application::Get().GetWindow()->GetHandle(), &xpos, &ypos);
    lastX = xpos; lastY = ypos;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Application::Get().GetWindow()->GetHandle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    LoadAssets();
}
MainLayer::~MainLayer()
{
	VertexBufferManager::ReleaseAll();
    IndexBufferManager::ReleaseAll();
	VertexBufferManager::ReleaseAll();
	ShaderManager::ReleaseAll();
	TextureManager::ReleaseAll();
	RenderbufferManager::ReleaseAll();
	FramebufferManager::ReleaseAll();

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
		TextureManager::DebugPrintSummary();
        frameCounter = 0;
    }

    ProcessInput(ts);
}
void MainLayer::OnRender()
{
    glm::vec2 viewportSize = Application::Get().GetFramebufferSize();

    RendererAPI::SRGBColorSpace(false); // disble gamma correction for intermediate steps
    FramebufferManager::Bind(shadowDepthMapFramebuffer);
	RenderShadowMap();

    ResizeBuffers();
    RendererAPI::SetViewport(0, 0, viewportSize.x, viewportSize.y);
    FramebufferManager::Bind(multiSampledframebuffer);
	RenderScene();

    FramebufferManager::Blit(multiSampledframebuffer, framebuffer);

    FramebufferManager::Unbind();
    if (gammaCorrection)
        RendererAPI::SRGBColorSpace(true); // enable gamma correction for final render
    RenderPostProcessing();

    RenderGUI();
}

void MainLayer::RenderShadowMap()
{
	RendererAPI::SetViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
    RendererAPI::ClearDepth();

    lightCam.coreCamera.setPos(-sunLight.direction * 10.0f + camera.coreCamera.getPos());
    lightSpaceMat = lightCam.coreCamera.getProjectionMatrix() * lightCam.coreCamera.getViewMatrix();

    ShaderManager::setmat4(shadowShader, "lightSpaceMat", lightSpaceMat);
    for (App::GameObject object : gameObjects)
    {
        object.Render(lightCam.coreCamera, shadowShader);
    }
}

void MainLayer::RenderScene()
{
    RendererAPI::ClearColor();
    RendererAPI::ClearDepth();

    ShaderManager::setmat4(blinnPhongShader, "lightSpaceMat", lightSpaceMat);
    ShaderManager::setmat4(texturedShader, "lightSpaceMat", lightSpaceMat);

    for ( App::GameObject object : gameObjects)
    {
        object.Render(camera.coreCamera);
    }

    camera.RenderSkybox();
}

void MainLayer::RenderPostProcessing()
{
    RendererAPI::ClearColor();
    RendererAPI::DepthTest(false);

    ShaderManager::setBool(postProcessingShader, "toneMapping", toneMapping);
    ShaderManager::setFloat(postProcessingShader, "exposure", exposure);
    gameObjects[screenQuad].Render(camera.coreCamera);

    RendererAPI::DepthTest(true);
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

void MainLayer::ResizeBuffers()
{
    glm::vec2 viewportSize = Application::Get().GetFramebufferSize();

    if (viewportSize == oldViewportSize || viewportSize.x == 0 || viewportSize.y == 0)
		return;

    RendererAPI::SetViewport(0, 0, viewportSize.x, viewportSize.y);

    TextureManager::Resize(multiSampledtextureColorBuffer, viewportSize.x, viewportSize.y);
    TextureManager::Resize(multiSampledtextureDepthStencilBuffer, viewportSize.x, viewportSize.y);

    TextureManager::Resize(textureColorBuffer, viewportSize.x, viewportSize.y);
    RenderbufferManager::Resize(renderbuffer, viewportSize.x, viewportSize.y);

	oldViewportSize = viewportSize;
}

void MainLayer::ProcessInput(double ts)
{
    if (glfwGetKey(Application::Get().GetWindow()->GetHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS && canPress)
    {
        mouseDisabled ? glfwSetInputMode(Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL) : glfwSetInputMode(Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouseDisabled = !mouseDisabled;
        canPress = false;
    }
    if (glfwGetKey(Application::Get().GetWindow()->GetHandle(), GLFW_KEY_ESCAPE) == GLFW_RELEASE)
        canPress = true;
    if (glfwGetKey(Application::Get().GetWindow()->GetHandle(), GLFW_KEY_W) == GLFW_PRESS)
        camera.coreCamera.ProcessKeyboard(FORWARD, ts);
    if (glfwGetKey(Application::Get().GetWindow()->GetHandle(), GLFW_KEY_S) == GLFW_PRESS)
        camera.coreCamera.ProcessKeyboard(BACKWARD, ts);
    if (glfwGetKey(Application::Get().GetWindow()->GetHandle(), GLFW_KEY_A) == GLFW_PRESS)
        camera.coreCamera.ProcessKeyboard(LEFT, ts);
    if (glfwGetKey(Application::Get().GetWindow()->GetHandle(), GLFW_KEY_D) == GLFW_PRESS)
        camera.coreCamera.ProcessKeyboard(RIGHT, ts);

    double xpos, ypos;
    glfwGetCursorPos(Application::Get().GetWindow()->GetHandle(), &xpos, &ypos);
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if(mouseDisabled)
        camera.coreCamera.ProcessMouseMovement(xoffset, yoffset);
}

void MainLayer::LoadAssets()
{
	// load and setup the blinn-phong shader
    blinnPhongShader = ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong.frag");
    ShaderManager::set3f(blinnPhongShader, "light.direction", sunLight.direction);
    ShaderManager::set3f(blinnPhongShader, "light.ambient", sunLight.ambient);
    ShaderManager::set3f(blinnPhongShader, "light.diffuse", sunLight.diffuse);
    ShaderManager::set3f(blinnPhongShader, "light.specular", sunLight.specular);
    ShaderManager::setInt(blinnPhongShader, "shadowMap", 5);

	// load and setup the textured shader
    texturedShader = ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/textured.frag");
    ShaderManager::Bind(texturedShader);
    ShaderManager::set3f(texturedShader, "light.direction", sunLight.direction);
    ShaderManager::set3f(texturedShader, "light.ambient", sunLight.ambient);
    ShaderManager::set3f(texturedShader, "light.diffuse", sunLight.diffuse);
    ShaderManager::set3f(texturedShader, "light.specular", sunLight.specular);
    ShaderManager::setInt(texturedShader, "shadowMap", 5);

    uint32_t skyboxShader = ShaderManager::CreateShader(RESOURCES_PATH "shaders/skybox.vert", RESOURCES_PATH "shaders/skybox.frag");

    postProcessingShader = ShaderManager::CreateShader(RESOURCES_PATH "shaders/post.vert", RESOURCES_PATH "shaders/post.frag");
    
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
    std::vector<uint32_t> defaultTextures
    {
        TextureManager::CreateTexture(RESOURCES_PATH "textures/default.png")
    };
	TextureManager::SetParameters(defaultTextures[0], GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    TextureManager::GenerateMipmaps(defaultTextures[0]);

	// load the box textures
    std::vector<uint32_t> boxTextures
    {
        TextureManager::CreateTexture(RESOURCES_PATH "textures/box.png"),
        TextureManager::CreateTexture(RESOURCES_PATH "textures/box-specular.png")
    };
    TextureManager::SetParameters(boxTextures[0], GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    TextureManager::GenerateMipmaps(boxTextures[0]);

	// load the redstone ore textures
    std::vector<uint32_t> redstoneOreTextures
    {
        TextureManager::CreateTexture(RESOURCES_PATH "textures/redstone-ore.png"),
        TextureManager::CreateTexture(RESOURCES_PATH "textures/redstone-ore-specular.png"),
        TextureManager::CreateTexture(RESOURCES_PATH "textures/redstone-ore-emission.png")
    };
	TextureManager::SetParameters(redstoneOreTextures[0], GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
    TextureManager::GenerateMipmaps(redstoneOreTextures[0]);

	// create the ground plane
    gameObjects.push_back( App::GameObject(ModelGen::GetPlane(1000, defaultTextures), texturedShader, glm::vec3(0.0f)));

	// create cubes with different materials
    for (int i = 0; i < 24; i++)
    {
		gameObjects.push_back( App::GameObject(ModelGen::GetCube(), blinnPhongShader, glm::vec3((i % 6) * 2.0f - 5.0f, 0.5f, (i / 6) * 2.0f - 3.0f), static_cast<BlinnPhongMaterial>(i)));
    }

    // create the textured box
	gameObjects.push_back( App::GameObject(ModelGen::GetCube(boxTextures), texturedShader, glm::vec3(-1.0f, 0.5f, -5.0f)));

	// create the redstone ore
	gameObjects.push_back( App::GameObject(ModelGen::GetCube(redstoneOreTextures), texturedShader, glm::vec3(1.0f, 0.5f, -5.0f)));

	// create the backpack
	gameObjects.push_back(App::GameObject(Model(RESOURCES_PATH "models/backpack/backpack.obj"), texturedShader, glm::vec3(3.0f, 2.0f, -7.0f)));

	// create the Vyse helmet
    gameObjects.push_back(App::GameObject(Model(RESOURCES_PATH "models/vyse-helmet/vyse-helmet.obj"), blinnPhongShader, glm::vec3(0.0f, 1.0f, -7.0f), FLASHBANG));

	// setup the camera
    camera = App::Camera(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    camera.SetSkybox(ModelGen::GetReversedCube({ TextureManager::CreateCubemap(faces) }), skyboxShader);

    lightCam = App::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
    lightCam.coreCamera.orthographic = true;
    lightCam.coreCamera.setPos(-sunLight.direction * 10.0f + camera.coreCamera.getPos());
    lightCam.coreCamera.lookAt(camera.coreCamera.getPos());

	// setup multisampled framebuffer
    multiSampledtextureColorBuffer = TextureManager::CreateTexture(GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F, 600, 600, GL_RGBA, GL_FLOAT, NULL, true, 4);
    multiSampledtextureDepthStencilBuffer = TextureManager::CreateTexture(GL_TEXTURE_2D_MULTISAMPLE, GL_DEPTH24_STENCIL8, 600, 600, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL, true, 4);

    multiSampledframebuffer = FramebufferManager::CreateFramebuffer();
    FramebufferManager::AttachTexture(multiSampledframebuffer, multiSampledtextureColorBuffer);
    FramebufferManager::AttachTexture(multiSampledframebuffer, multiSampledtextureDepthStencilBuffer);

    // setup post processing
    textureColorBuffer = TextureManager::CreateTexture(GL_TEXTURE_2D, GL_RGBA16F, 600, 600, GL_RGBA, GL_FLOAT, NULL, false, 0);
    
    gameObjects.push_back(App::GameObject(ModelGen::GetQuad(std::vector<uint32_t>{textureColorBuffer}), postProcessingShader));
    screenQuad = gameObjects.size() - 1;

    renderbuffer = RenderbufferManager::CreateRenderbuffer(GL_DEPTH24_STENCIL8, 600, 600, false, 0);
    
    framebuffer = FramebufferManager::CreateFramebuffer();
    FramebufferManager::AttachTexture(framebuffer, textureColorBuffer);
    FramebufferManager::AttachRenderbuffer(framebuffer, renderbuffer);


	// setup shadow mapping framebuffer
    shadowTexture = TextureManager::CreateTexture(GL_TEXTURE_2D, GL_DEPTH_COMPONENT32F, SHADOW_SIZE, SHADOW_SIZE, GL_DEPTH_COMPONENT, GL_FLOAT, NULL, false, 0);
    TextureManager::SetParameters(shadowTexture, GL_CLAMP_TO_BORDER, GL_LINEAR, GL_LINEAR);
    TextureManager::SetBorderColor(shadowTexture, 1.0f, 1.0f, 1.0f, 1.0f);

    shadowDepthMapFramebuffer = FramebufferManager::CreateFramebuffer();
    FramebufferManager::AttachTexture(shadowDepthMapFramebuffer, shadowTexture);
    
    shadowShader = ShaderManager::CreateShader(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/empty.frag");
}