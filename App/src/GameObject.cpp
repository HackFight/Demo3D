#include "GameObject.h"

// Engine
#include "RendererAPI/ShaderManager.h"

// App
#include "MaterialGen.h"

// std
#include <cstdint>

// libs
#include <glm/ext/matrix_transform.hpp>

namespace App
{
	GameObject::GameObject(Core::Model model, uint32_t shader, glm::vec3 position, glm::vec3 scale, BlinnPhongMaterial material)
		: m_Model(model), m_Shader(shader), m_Position(position), m_Scale(scale), m_Material(material) {}

	GameObject::~GameObject() {}

	void GameObject::Render(Core::Camera& camera, uint32_t shader) const
	{
		Core::ShaderManager::setmat4(shader, "viewMat", camera.getViewMatrix());
		Core::ShaderManager::setmat4(shader, "projMat", camera.getProjectionMatrix());
		Core::ShaderManager::setmat4(shader, "modelMat", glm::scale(glm::translate(glm::mat4(1.0f), m_Position), m_Scale));

		Core::ShaderManager::setInt(shader, "material.texture_diffuse1", 0);
		Core::ShaderManager::setInt(shader, "material.texture_specular1", 1);
		Core::ShaderManager::setInt(shader, "material.texture_emission1", 2);
		MaterialGen::setBlinnPhongMaterial(shader, m_Material);

		Core::ShaderManager::set3f(shader, "viewPos", camera.position);
		m_Model.Draw(shader);
	}

	void GameObject::Render(Core::Camera& camera) const
	{
		Render(camera, m_Shader);
	}
}