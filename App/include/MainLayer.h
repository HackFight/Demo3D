#pragma once

// Engine
#include "Core/Layer.h"

// App
#include "GameObject.h"
#include "Camera.h"

//std
#include <cstdint>

const int SHADOW_SIZE = 2048;

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
	
	uint32_t multiSampledtextureColorBuffer, multiSampledtextureDepthStencilBuffer;
	uint32_t multiSampledframebuffer;

	uint32_t textureColorBuffer;
	uint32_t renderbuffer;
	uint32_t framebuffer;
	uint32_t screenQuad;
	
	uint32_t shadowTexture;
	uint32_t shadowDepthMapFramebuffer;
	glm::mat4 lightSpaceMat;
	
	uint32_t shadowShader, blinnPhongShader, texturedShader, postProcessingShader;
	
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
	
	glm::vec2 oldViewportSize = glm::vec2(1920, 1200);
};