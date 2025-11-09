#pragma once

#include "Core/Layer.h"
#include "Camera.h"
#include "RendererAPI/Framebuffer.h"
#include "RendererAPI/RendererAPI.h"
#include "GameObject.h"

#include <memory>

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

	std::shared_ptr<Core::RendererAPI> renderer;
	std::unique_ptr<Camera> lightCam;
	std::unique_ptr<Camera> camera;

	std::shared_ptr<Core::Texture> multiSampledtextureColorBuffer;
	std::shared_ptr<Core::Texture> multiSampledtextureDepthStencilBuffer;
	std::shared_ptr<Core::Framebuffer> multiSampledframebuffer;

	std::shared_ptr<Core::Texture> textureColorBuffer;
	std::shared_ptr<Core::Renderbuffer> renderbuffer;
	std::shared_ptr<Core::Framebuffer> framebuffer;
	std::shared_ptr<GameObject> screenQuad;

	std::shared_ptr<Core::Texture> shadowTexture;;
	std::shared_ptr<Core::Framebuffer> shadowDepthMapFramebuffer;
	std::shared_ptr<Core::Shader> shadowShader;
	glm::mat4 lightSpaceMat;

	std::shared_ptr<Core::Shader> blinnPhongShader;
	std::shared_ptr<Core::Shader> texturedShader;
	std::shared_ptr<Core::Shader> postProcessingShader;

	std::vector<GameObject> gameObjects;

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