#pragma once

// Engine
#include "Renderer/Camera.h"
#include "Core/Model.h"

// App
#include "MaterialGen.h"

namespace App
{
	class GameObject
	{
	public:
		GameObject(Core::Model model = Core::Model(), size_t shader = 0, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), BlinnPhongMaterial material = WhitePlastic);
		~GameObject();

		void Render(Core::Camera& camera, size_t forcedShader) const;
		void Render(Core::Camera& camera) const;

		Core::Model GetModel() const { return m_Model; }

		glm::vec3 m_Position;
		glm::vec3 m_Scale;
	private:
		Core::Model m_Model;
		size_t m_Shader;
		BlinnPhongMaterial m_Material;
	};
}