#pragma once

#include "Renderer/Camera.h"
#include "RendererAPI/Shader.h"
#include "Core/Model.h"
#include "MaterialGen.h"

namespace App
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(Core::Model model, Core::Shader shader, glm::vec3 position = glm::vec3(0.0f), BlinnPhongMaterial material = WhitePlastic);
		~GameObject();

		void Render(Core::Camera camera, Core::Shader forcedShader);
		void Render(Core::Camera camera);

	private:
		Core::Model m_Model;
		Core::Shader m_Shader;
		BlinnPhongMaterial m_Material;

		glm::vec3 m_Position;
	};
}