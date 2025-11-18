#pragma once

//Engine
#include "Core/Layer.h"

//App
#include "Camera.h"
#include "GameObject.h"

//std
#include <stdint.h>

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class TestLayer : public Core::Layer
{
public:
    TestLayer();
	virtual ~TestLayer();

	virtual void OnUpdate(double ts) override;
	virtual void OnRender() override;

private:
    void RenderGUI();
    void ProcessInput(double ts);
	void LoadAssets();

    uint32_t blinnPhongShader, texturedShader;

    App::Camera camera;
    bool mouseDisabled = true;
    bool canPress = true;
    double lastX, lastY;

    double timeAcc = 0.0;
	int frameCounter = 0;

    DirectionalLight sunLight
    {
        glm::vec3(1.0f, -2.0f, 2.0f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(2.0f, 2.0f, 2.0f)
    };

	std::vector<App::GameObject> gameObjects;
};