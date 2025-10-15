#pragma once

#include "Core/Layer.h"
#include "Renderer/Buffer.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include <memory>

class MainLayer : public Core::Layer
{
public:
	MainLayer();
	virtual ~MainLayer();

	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;

private:
	std::unique_ptr<Core::RendererAPI> renderer;
	std::shared_ptr<Core::VertexBuffer> vertexBuffer;
	std::shared_ptr<Core::IndexBuffer> indexBuffer;
	std::shared_ptr<Core::VertexArray> vertexArray;
	std::shared_ptr<Core::Shader> shader;
};