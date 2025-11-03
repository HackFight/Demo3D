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
	GameObject(std::shared_ptr<Core::VertexArray> mesh, std::shared_ptr<Core::Shader> blinnPhongShader, BlinnPhongMaterial material = WhitePlastic, glm::vec3 position = glm::vec3(0.0f));
	GameObject(std::shared_ptr<Core::VertexArray> mesh, std::shared_ptr<Core::Shader> texturedShader, std::shared_ptr<Core::Texture> texture, glm::vec3 position = glm::vec3(0.0f));
	~GameObject();

	void Render(std::shared_ptr<Core::RendererAPI> renderer, std::shared_ptr<Core::Camera> camera);

private:

	enum ShaderType
	{
		BlinnPhong,
		Textured
	};

	std::shared_ptr<Core::VertexArray> mesh;
	std::shared_ptr<Core::Shader> shader;
	ShaderType shaderType;
	BlinnPhongMaterial material;
	std::vector<std::shared_ptr<Core::Texture>> textures;

	glm::vec3 position;
};