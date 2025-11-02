#pragma once

#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/Shader.h"
#include "RendererAPI/VertexArray.h"
#include "MaterialGen.h"

#include <memory>

class GameObject
{
public:
	GameObject(std::shared_ptr<Core::VertexArray> mesh, std::shared_ptr<Core::Shader> shader, BlinnPhongMaterial material = WhitePlastic, glm::vec3 position = glm::vec3(0.0f));
	~GameObject();

	void Render(Core::RendererAPI* renderer);

private:
	std::shared_ptr<Core::VertexArray> mesh;
	std::shared_ptr<Core::Shader> shader;
	BlinnPhongMaterial material;

	glm::vec3 position;
};