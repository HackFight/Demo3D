#pragma once

//Engine
#include "Renderer/Camera.h"

namespace Platformer {

    class Sprite {
    public:
        Sprite(size_t texture, glm::vec2 position = {0.0f, 0.0f}, glm::vec2 scale = {0.5f, 0.5f}, glm::vec3 color = {1.0f, 1.0f, 1.0f});
        ~Sprite();

        void Render(Core::Camera& camera, size_t shader);
        void Render(Core::Camera& camera);

        size_t m_VAO = -1, m_Shader = -1, m_Texture = -1;
        glm::vec2 m_Position = glm::vec2(0.0f), m_Scale = glm::vec2(0.5f);
        glm::vec3 m_Color = glm::vec3(1.0f);
    };
}