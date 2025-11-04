#include "GameObject.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

GameObject::GameObject(std::shared_ptr<Core::VertexArray> vertexArray, std::shared_ptr<Core::Shader> shader, glm::vec3 position, ShaderType shaderType, BlinnPhongMaterial material)
	: vertexArray(vertexArray), shader(shader), position(position), shaderType(shaderType), material(material) {}

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
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		break;

	case BlinnPhong:
		shader->Bind();
		shader->set3f("viewPos", camera->getPos());
		shader->setmat4("viewMat", camera->getViewMatrix());
		shader->setmat4("projMat", camera->getProjectionMatrix());
		shader->setmat4("modelMat", glm::translate(glm::mat4(1.0f), position));
		MaterialGen::setBlinnPhongMaterial(shader, material);
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		break;

	case Textured:
		shader->Bind();

		shader->set3f("viewPos", camera->getPos());
		shader->setmat4("viewMat", camera->getViewMatrix());
		shader->setmat4("projMat", camera->getProjectionMatrix());
		shader->setmat4("modelMat", glm::translate(glm::mat4(1.0f), position));
		
		shader->setInt("material.diffuse", 0);
		shader->setInt("material.specular", 1);
		shader->setInt("material.emission", 2);
		shader->setFloat("material.shininess", 32.0f);

		glActiveTexture(GL_TEXTURE0);
		textures[0]->Bind();
		glActiveTexture(GL_TEXTURE1);
		textures.size() > 1 ? textures[1]->Bind() : textures[0]->Unbind();
		glActiveTexture(GL_TEXTURE2);
		textures.size() > 2 ? textures[2]->Bind() : textures[0]->Unbind();

		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		break;
	}
}