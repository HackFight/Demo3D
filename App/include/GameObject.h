#pragma once

#include "RendererAPI/RendererAPI.h"
#include <Renderer/Camera.h>
#include "RendererAPI/Shader.h"
#include "RendererAPI/Texture.h"
#include "RendererAPI/VertexArray.h"
#include "MaterialGen.h"

#include <memory>
#include <vector>
#include <Core/Model.h>

class GameObject
{
public:
	enum ShaderType
	{
		Default,
		BlinnPhong
	};

	GameObject(std::shared_ptr<Core::Model> model, std::shared_ptr<Core::Shader> shader, glm::vec3 position = glm::vec3(0.0f), ShaderType shaderType = BlinnPhong, BlinnPhongMaterial material = WhitePlastic);
	~GameObject();

	void Render(std::shared_ptr<Core::Camera> camera);

private:
	std::shared_ptr<Core::Model> model;
	std::shared_ptr<Core::Shader> shader;
	ShaderType shaderType;
	BlinnPhongMaterial material;

	glm::vec3 position;
};