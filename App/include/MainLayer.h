#pragma once

#include "Core/Layer.h"
#include "Renderer/Camera.h"
#include "RendererAPI/Buffer.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/Shader.h"
#include "RendererAPI/VertexArray.h"
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

	std::unique_ptr<Core::RendererAPI> renderer;
	std::shared_ptr<Core::Camera> camera;

	std::shared_ptr<Core::VertexBuffer> vertexBuffer;
	std::shared_ptr<Core::IndexBuffer> indexBuffer;
	std::shared_ptr<Core::VertexArray> vertexArray;

	std::shared_ptr<Core::Shader> defaultShader;
	std::shared_ptr<Core::Shader> phongShader;

	double timeAcc = 0.0;
	int frameCounter = 0;
	bool mouseDisabled = true;
	bool canPress = true;

	double lastX, lastY;
};