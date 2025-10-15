#pragma once

#include "Core/Layer.h"
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

	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;

private:
	std::unique_ptr<Core::RendererAPI> renderer;
	std::shared_ptr<Core::VertexBuffer> vertexBuffer;
	std::shared_ptr<Core::IndexBuffer> indexBuffer;
	std::shared_ptr<Core::VertexArray> vertexArray;
	std::shared_ptr<Core::Shader> shader;
};