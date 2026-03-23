#pragma once

// Engine
#include "Renderer/Camera.h"
#include "Core/Model.h"

// App
#include "MaterialGen.h"

//std
#include <cstdint>

namespace App
{
	class GameObject
	{
	public:
		GameObject(Core::Model model = Core::Model(), uint32_t shader = 0, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), BlinnPhongMaterial material = WhitePlastic);
		~GameObject();

		void Render(Core::Camera camera, uint32_t forcedShader);
		void Render(Core::Camera camera);

		Core::Model GetModel() { return m_Model; }

		glm::vec3 m_Position;
		glm::vec3 m_Scale;
	private:
		Core::Model m_Model;
		uint32_t m_Shader;
		BlinnPhongMaterial m_Material;
	};
}