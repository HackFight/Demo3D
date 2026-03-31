#include "Player.h"

namespace Platformer {
    Player::Player(size_t texture)
    : sprite(Sprite(texture)) {
        defaultSpriteScale = sprite.scale;
    }
    Player::~Player() {}

    void Player::update(double dt) {
        sprite.position = position;
        sprite.scale = defaultSpriteScale * size;
        updateConstraints();
    }

    void Player::move(double dt) {
        velocity += acceleration * (float)dt;
        position += velocity * (float)dt;
    }

    void Player::render(Core::Camera& camera) {
        sprite.Render(camera);
    }

    void Player::updateConstraints() {
        if (position.y - size.y/2.0f < -1.0f) {
            position.y = -1.0f + size.y/2.0f;
            velocity.y = 0.0f;
        }
    }
}