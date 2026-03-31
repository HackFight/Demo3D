#pragma once

// libs
#include "Renderer/Camera.h"
#include "Sprite.h"
#include <glm/glm.hpp>

namespace Platformer {
    class Player {
    public:
        Player(size_t texture = -1);
        ~Player();

        Sprite sprite;

        glm::vec2 size{1.0f, 1.0f};
        glm::vec2 position{0.0f, 0.0f};
        glm::vec2 velocity{0.0f, 0.0f};
        glm::vec2 acceleration{0.0f, 0.0f};

        void update(double dt);
        void move(double dt);
        void render(Core::Camera& camera);

    private:
        glm::vec2 defaultSpriteScale{0.5f, 0.5f};

        void updateConstraints();
    };
}