#pragma once

#include "RendererAPI/RendererAPI.h"
#include <Renderer/Camera.h>
#include "RendererAPI/Shader.h"
#include "RendererAPI/Texture.h"
#include "RendererAPI/VertexArray.h"
#include "MaterialGen.h"

#include <memory>
#include <vector>

class GameObject
{
public:
	enum ShaderType
	{
		Default,
		BlinnPhong,
		Textured
	};

	GameObject(std::shared_ptr<Core::VertexArray> mesh, std::shared_ptr<Core::Shader> shader, glm::vec3 position = glm::vec3(0.0f), ShaderType shaderType = BlinnPhong, BlinnPhongMaterial material = WhitePlastic);
	~GameObject();

	void AddTexture(std::shared_ptr<Core::Texture> texture)
	{
		textures.push_back(texture);
	}

	void Render(std::shared_ptr<Core::RendererAPI> renderer, std::shared_ptr<Core::Camera> camera);

private:
	std::shared_ptr<Core::VertexArray> mesh;
	std::shared_ptr<Core::Shader> shader;
	ShaderType shaderType;
	BlinnPhongMaterial material;
	std::vector<std::shared_ptr<Core::Texture>> textures;

	glm::vec3 position;
};