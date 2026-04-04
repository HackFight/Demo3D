#include "Layer.h"

// Platformer
#include "AssetsManager.h"

// Engine
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"

// std
#include <cstddef>

namespace Platformer {
    Layer::Layer(size_t texture, size_t shader) :
    texture(texture),
    shader(shader),
    vao(AssetsManager::GetVAO(AssetsManager::Quad))
    { }
    Layer::~Layer() {}

    void Layer::Render(size_t shader) {
        Core::TextureManager::Bind(texture, 0);
		Core::ShaderManager::setInt(shader, "texUnit", 0);
		Core::RendererAPI::DrawIndexed(vao);
    }
    void Layer::Render() {
        Render(shader);
    }
}