#include "GameObject.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

GameObject::GameObject(std::shared_ptr<Core::VertexArray> mesh, std::shared_ptr<Core::Shader> blinnPhongShader, BlinnPhongMaterial material, glm::vec3 position)
	: mesh(mesh), shader(blinnPhongShader), material(material), position(position), shaderType(BlinnPhong) {}

GameObject::GameObject(std::shared_ptr<Core::VertexArray> mesh, std::shared_ptr<Core::Shader> texturedShader, std::shared_ptr<Core::Texture> texture, glm::vec3 position)
	:  mesh(mesh), shader(texturedShader), position(position), shaderType(Textured)
{
	textures.push_back(texture);
}

GameObject::~GameObject() {}

void GameObject::Render(std::shared_ptr<Core::RendererAPI> renderer, std::shared_ptr<Core::Camera> camera)
{
	switch(shaderType)
	{
	case BlinnPhong:
		shader->Bind();
		shader->set3f("viewPos", camera->getPos());
		shader->setmat4("viewMat", camera->getViewMatrix());
		shader->setmat4("projMat", camera->getProjectionMatrix());
		shader->setmat4("modelMat", glm::translate(glm::mat4(1.0f), position));
		MaterialGen::setBlinnPhongMaterial(shader, material);
		renderer->DrawIndexed(mesh);
		break;

	case Textured:
		shader->Bind();

		shader->set3f("viewPos", camera->getPos());
		shader->setmat4("viewMat", camera->getViewMatrix());
		shader->setmat4("projMat", camera->getProjectionMatrix());
		shader->setmat4("modelMat", glm::translate(glm::mat4(1.0f), position));
		
		glActiveTexture(GL_TEXTURE0);
		textures[0]->Bind();
		shader->setInt("material.diffuse", 0);
		shader->set3f("material.specular", 1.0f, 1.0f, 1.0f);
		shader->setFloat("material.shininess", 32.0f);
		
		renderer->DrawIndexed(mesh);
		break;
	}
}