#pragma once

//Engine
#include "Core/Layer.h"

//App
#include "Camera.h"
#include "Renderer/OrthographicCamera.h"
#include "SoftBody.h"

//std
#include <stdint.h>
#include <vector>

const int SHADOW_SIZE = 2048;
const int SHADOWMAP_TEXTURE_UNIT = 5;

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class PhysicsTestLayer : public Core::Layer
{
public:
    PhysicsTestLayer();
	virtual ~PhysicsTestLayer();

	virtual void OnUpdate(double ts) override;
	virtual void OnRender() override;

private:
    void RenderGUI();
    void ProcessInput(double ts);
	void LoadAssets();

    uint32_t blinnPhongShader, texturedShader, skyboxShader, postProcessingShader;
    
    uint32_t framebuffer, framebufferColor, renderbuffer;
    App::GameObject screenQuad;

    uint32_t shadowbuffer, shadowmap, shadowShader, normalsShader;

    App::Camera camera;
    Core::OrthographicCamera lightCamera;
    bool mouseDisabled = true;
    bool canPress = true;
    double lastX, lastY;

    double timeAcc = 0.0;
	int frameCounter = 0;

    glm::vec2 oldFbSize;

	bool gammaCorrection = true;
	bool toneMapping = true;
	float exposure = 1.0f;

    DirectionalLight sunLight
    {
        glm::vec3(1.0f, -2.0f, 2.0f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(2.0f, 2.0f, 2.0f)
    };

    App::SoftBody jellyCube;
	std::vector<App::GameObject> gameObjects;
};