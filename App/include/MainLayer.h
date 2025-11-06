#pragma once

#include "Core/Layer.h"
#include "Camera.h"
#include "RendererAPI/RendererAPI.h"
#include "GameObject.h"

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
	std::unique_ptr<Camera> camera;

	std::vector<GameObject> gameObjects;

	double timeAcc = 0.0;
	int frameCounter = 0;
	bool mouseDisabled = true;
	bool canPress = true;

	double lastX, lastY;
};