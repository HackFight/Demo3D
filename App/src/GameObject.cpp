#include "GameObject.h"

// Engine
#include "Renderer/Camera.h"

// App
#include "MaterialGen.h"

namespace App
{
	GameObject::GameObject() {}
	GameObject::GameObject(Core::Model model, Core::Shader shader, glm::vec3 position, BlinnPhongMaterial material)
		: m_Model(model), m_Shader(shader), m_Position(position), m_Material(material) {}

	GameObject::~GameObject() {}

	void GameObject::Render(Core::Camera camera, Core::Shader shader)
	{
		shader.Bind();
		shader.setmat4("viewMat", camera.getViewMatrix());
		shader.setmat4("projMat", camera.getProjectionMatrix());
		shader.setmat4("modelMat", glm::translate(glm::mat4(1.0f), m_Position));

		shader.setInt("material.texture_diffuse1", 0);
		shader.setInt("material.texture_specular1", 1);
		shader.setInt("material.texture_emission1", 2);
		MaterialGen::setBlinnPhongMaterial(shader, m_Material);

		shader.set3f("viewPos", camera.getPos());
		m_Model.Draw(shader);
	}

	void GameObject::Render(Core::Camera camera)
	{
		Render(camera, m_Shader);
	}
}