#pragma once
//Engine
#include "Renderer/Camera.h"

namespace Platformer {

    class Sprite {
    public:
        Sprite(unsigned int texture, glm::vec2 position = {0.0f, 0.0f}, glm::vec2 scale = {0.5f, 0.5f}, glm::vec3 color = {1.0f, 1.0f, 1.0f});
        ~Sprite();

        void Render(Core::Camera camera, unsigned int shader);
        void Render(Core::Camera camera);

        unsigned int m_VAO, m_Shader, m_Texture;
        glm::vec2 m_Position, m_Scale;
        glm::vec3 m_Color;
    };
}