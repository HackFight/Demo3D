#include "GameObject.h"
#include "MainLayer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

GameObject::GameObject(std::shared_ptr<Core::Model> model, std::shared_ptr<Core::Shader> shader, glm::vec3 position, ShaderType shaderType, BlinnPhongMaterial material)
	: model(model), shader(shader), position(position), shaderType(shaderType), material(material) {}

GameObject::~GameObject() {}

void GameObject::Render(std::shared_ptr<Core::Camera> camera)
{
	switch(shaderType)
	{
	case Default:
		shader->Bind();
		shader->setmat4("viewMat", camera->getViewMatrix());
		shader->setmat4("projMat", camera->getProjectionMatrix());
		shader->setmat4("modelMat", glm::translate(glm::mat4(1.0f), position));

		shader->setInt("material.texture_diffuse1", 0);
		shader->setInt("material.texture_specular1", 1);
		shader->setInt("material.texture_emission1", 2);
		shader->setFloat("material.shininess", 32.0f);

		shader->set3f("viewPos", camera->getPos());
		shader->setBool("gamma", true);

		for (int i = 0; i < 16; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		model->Draw(shader);
		break;

	case BlinnPhong:
		shader->Bind();

		shader->setmat4("viewMat", camera->getViewMatrix());
		shader->setmat4("projMat", camera->getProjectionMatrix());
		shader->setmat4("modelMat", glm::translate(glm::mat4(1.0f), position));

		shader->set3f("viewPos", camera->getPos());
		shader->setBool("gamma", true);
		
		MaterialGen::setBlinnPhongMaterial(shader, material);
		model->Draw(shader);
		break;
	}
}