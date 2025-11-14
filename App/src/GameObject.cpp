#include "GameObject.h"

// Engine
#include "Renderer/Camera.h"
#include "RendererAPI/ShaderManager.h"

// App
#include "MaterialGen.h"
#include <cstdint>

namespace App
{
	GameObject::GameObject(Core::Model model, uint32_t shader, glm::vec3 position, BlinnPhongMaterial material)
		: m_Model(model), m_Shader(shader), m_Position(position), m_Material(material) {}

	GameObject::~GameObject() {}

	void GameObject::Render(Core::Camera camera, uint32_t shader)
	{
		Core::ShaderManager::setmat4(shader, "viewMat", camera.getViewMatrix());
		Core::ShaderManager::setmat4(shader, "projMat", camera.getProjectionMatrix());
		Core::ShaderManager::setmat4(shader, "modelMat", glm::translate(glm::mat4(1.0f), m_Position));

		Core::ShaderManager::setInt(shader, "material.texture_diffuse1", 0);
		Core::ShaderManager::setInt(shader, "material.texture_specular1", 1);
		Core::ShaderManager::setInt(shader, "material.texture_emission1", 2);
		MaterialGen::setBlinnPhongMaterial(std::move(shader), m_Material);

		Core::ShaderManager::set3f(shader, "viewPos", camera.getPos());
		m_Model.Draw(std::move(shader));
	}

	void GameObject::Render(Core::Camera camera)
	{
		Render(camera, std::move(m_Shader));
	}
}