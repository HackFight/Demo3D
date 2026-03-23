#include "Sprite.h"

// Engine
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"

// Platformer
#include "ResourceManager.h"

// libs
#include <glm/ext/matrix_transform.hpp>
#include <iostream>


namespace Platformer {
    Sprite::Sprite(size_t texture, glm::vec2 position, glm::vec2 scale, glm::vec3 color) :
    m_VAO(ResourceManager::Primitive::Quad),
    //m_VAO(ResourceManager::GetVAO(ResourceManager::Primitive::Quad)),
    m_Shader(ResourceManager::GetShader(ResourceManager::Shader::Texture)),
    m_Texture(texture),
    m_Position(position),
    m_Scale(scale),
    m_Color(color) {
        std::cout
        << "Created Sprite with:\n- VAO: " << m_VAO
        << "\n- Shader: " << m_Shader
        << "\n- Texture: " << m_Texture
        << "\n- Position: (" << m_Position.x << ", " << m_Position.y
        << ")\n- Scale: (" << m_Scale.x << ", " << m_Scale.y
        << ")\n- Color: (" << m_Color.r << ", " << m_Color.g << ", " << m_Color.b
        << ")\n";
    }
    Sprite::~Sprite() { }

    void Sprite::Render(Core::Camera& camera, size_t shader) {
        Core::ShaderManager::setmat4(shader, "viewMat", camera.getViewMatrix());
		Core::ShaderManager::setmat4(shader, "projMat", camera.getProjectionMatrix());
		Core::ShaderManager::setmat4(shader, "modelMat", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f)), glm::vec3(m_Scale,1.0f)));

		Core::ShaderManager::setInt(shader, "texUnit", 0);

		Core::ShaderManager::set3f(shader, "viewPos", camera.position);
		Core::RendererAPI::DrawIndexed(m_VAO);
    }

    void Sprite::Render(Core::Camera& camera) {
        Render(camera, m_Shader);
    }
}