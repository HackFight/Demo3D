#include "Sprite.h"

// Engine
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"

// Platformer
#include "RendererAPI/TextureManager.h"
#include "ResourceManager.h"

// libs
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>


namespace Platformer {
    Sprite::Sprite(size_t texture, glm::vec2 position, float rotation, glm::vec2 scale, glm::vec3 color) :
    vao(ResourceManager::Primitive::Quad),
    //m_VAO(ResourceManager::GetVAO(ResourceManager::Primitive::Quad)),
    shader(ResourceManager::GetShader(ResourceManager::Shader::Texture)),
    texture(texture),
    position(position),
    rotation(rotation),
    scale(scale),
    color(color) {
        std::cout
        << "Created Sprite with:\n- VAO: " << vao
        << "\n- Shader: " << shader
        << "\n- Texture: " << texture
        << "\n- Position: (" << position.x << ", " << position.y
        << ")\n- Scale: (" << scale.x << ", " << scale.y
        << ")\n- Color: (" << color.r << ", " << color.g << ", " << color.b
        << ")\n";
    }
    Sprite::~Sprite() { }

    void Sprite::Render(Core::Camera& camera, size_t shader) {
        Core::ShaderManager::setmat4(shader, "viewMat", camera.getViewMatrix());
		Core::ShaderManager::setmat4(shader, "projMat", camera.getProjectionMatrix());
		Core::ShaderManager::setmat4(shader, "modelMat", glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)), glm::vec3(scale,1.0f)), glm::radians(rotation), {0.0f, 0.0f, 1.0f}));

        Core::TextureManager::Bind(texture, 0);
		Core::ShaderManager::setInt(shader, "texUnit", 0);

		Core::ShaderManager::set3f(shader, "viewPos", camera.position);
		Core::RendererAPI::DrawIndexed(vao);
    }

    void Sprite::Render(Core::Camera& camera) {
        Render(camera, shader);
    }
}