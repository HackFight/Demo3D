#pragma once

//Engine
#include "Core/Layer.h"
#include "Renderer/OrthographicCamera.h"

// libs
#include <glm/glm.hpp>
#include <vector>

// Platformer
#include "AssetsManager.h"

namespace Platformer {
    const glm::vec2 RENDER_BUFFER_SCALE = {1920/PIXEL_SIZE, 1080/PIXEL_SIZE};
    const float ASPECT_RATIO = 16.0f/9.0f;

    class MainLayer : public Core::Layer {
    public:
        MainLayer();
        virtual ~MainLayer();

        virtual void OnUpdate(double ts) override;
        virtual void OnRender() override;

    private:
        void FixedUpdate(double fixedTimeStep);
        void LoadAssets();
        void PrintStats();
        void RenderGUI();

        uint32_t TPS = 60;

        glm::vec2 oldFbSize;
        double lastMouseX, lastMouseY, tickAccumulator, secondsAccumulator, timer;
        uint32_t frameCounter, tickCounter;
        size_t intermediaryFramebuffer, postFramebuffer;

        Core::OrthographicCamera camera;

        std::vector<size_t> layers;
        std::vector<size_t> layer0_sprites;
        size_t postSprite;
    };
}