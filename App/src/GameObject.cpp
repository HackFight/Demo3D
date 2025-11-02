#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(std::shared_ptr<Core::VertexArray> mesh, std::shared_ptr<Core::Shader> shader, BlinnPhongMaterial material, glm::vec3 position)
	: mesh(mesh), shader(shader), material(material), position(position) {}
GameObject::~GameObject() {}

void GameObject::Render(Core::RendererAPI* renderer)
{
	shader->Bind();
	shader->setmat4("modelMat", glm::translate(glm::mat4(1.0f), position));
	MaterialGen::setBlinnPhongMaterial(shader.get(), material);
	renderer->DrawIndexed(mesh);
}