#pragma once

// Engine
#include "Core/Layer.h"
#include "RendererAPI/Framebuffer.h"

// App
#include "Camera.h"
#include "GameObject.h"

const int SHADOW_SIZE = 2048*8;

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class MainLayer : public Core::Layer
{
public:
	MainLayer();
	virtual ~MainLayer();

	virtual void OnUpdate(double ts) override;
	virtual void OnRender() override;

private:
	void RenderShadowMap();
	void RenderScene();
	void RenderPostProcessing();
	void RenderGUI();

	void ResizeBuffers();

	void ProcessInput(double ts);
	void LoadAssets();

	App::Camera camera, lightCam;

	Core::Texture multiSampledtextureColorBuffer, multiSampledtextureDepthStencilBuffer;
	Core::Framebuffer multiSampledframebuffer;

	Core::Texture textureColorBuffer;
	Core::Renderbuffer renderbuffer;
	Core::Framebuffer framebuffer;
	App::GameObject screenQuad;

	Core::Texture shadowTexture;
	Core::Framebuffer shadowDepthMapFramebuffer;
	glm::mat4 lightSpaceMat;

	Core::Shader shadowShader, blinnPhongShader, texturedShader, postProcessingShader;

	std::vector<App::GameObject> gameObjects;

	DirectionalLight sunLight
	{
		glm::vec3(1.0f, -2.0f, 2.0f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(2.0f, 2.0f, 2.0f)
	};

	double timeAcc = 0.0;
	int frameCounter = 0;
	bool mouseDisabled = true;
	bool canPress = true;

	double lastX, lastY;

	bool gammaCorrection = true;
	bool toneMapping = true;
	float exposure = 1.0f;
};