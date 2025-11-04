#pragma once

#include "Core/Layer.h"
#include "Core/Model.h"
#include "Renderer/Camera.h"
#include "RendererAPI/RendererAPI.h"
#include "GameObject.h"
#include "RendererAPI/Shader.h"
#include <memory>

class MainLayer : public Core::Layer
{
public:
	MainLayer();
	virtual ~MainLayer();

	virtual void OnUpdate(double ts) override;
	virtual void OnRender() override;

private:
	void ProcessInput(double ts);
	void LoadAssets();

	std::shared_ptr<Core::RendererAPI> renderer;
	std::shared_ptr<Core::Camera> camera;

	std::vector<GameObject> gameObjects;
	std::shared_ptr<Core::Shader> texturedShader;
	std::shared_ptr<Core::Model> backpack;

	double timeAcc = 0.0;
	int frameCounter = 0;
	bool mouseDisabled = true;
	bool canPress = true;

	double lastX, lastY;
};