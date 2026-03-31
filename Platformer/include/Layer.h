#pragma once

// std
#include <cstddef>

namespace Platformer {
    class Layer {
    public:
        Layer(size_t texture = -1, size_t shader = -1);
        ~Layer();

        void Render(size_t shader);
        void Render();

        size_t vao = -1, shader = -1, texture = -1;
    };
}