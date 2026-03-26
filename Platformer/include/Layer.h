#pragma once

// std
#include <cstddef>

namespace Platformer {
    class Layer {
    public:
        Layer(size_t texture, size_t shader);
        ~Layer();

        void Render(size_t shader);
        void Render();

        size_t vao = -1, shader = -1, texture = -1;
    };
}