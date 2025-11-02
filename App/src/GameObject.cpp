#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(std::shared_ptr<Core::VertexArray> mesh, BlinnPhongMaterial material, glm::vec3 position)
	: mesh(mesh), shader(ShaderGen::GetShader(BlinnPhong)), material(material), position(position) {}
GameObject::~GameObject() {}

void GameObject::Render(std::shared_ptr<Core::RendererAPI> renderer, std::shared_ptr<Core::Camera> camera)
{
	shader->Bind();
	shader->set3f("viewPos", camera->getPos());
	shader->setmat4("viewMat", camera->getViewMatrix());
	shader->setmat4("projMat", camera->getProjectionMatrix());
	shader->setmat4("modelMat", glm::translate(glm::mat4(1.0f), position));
	MaterialGen::setBlinnPhongMaterial(shader, material);
	renderer->DrawIndexed(mesh);
}