#pragma once

//Engine
#include "Core/Layer.h"
#include "Layer.h"
#include "Renderer/OrthographicCamera.h"
#include "Sprite.h"

// libs
#include <glm/glm.hpp>
#include <vector>

class MainLayer : public Core::Layer {

public:
    MainLayer();
	virtual ~MainLayer();

	virtual void OnUpdate(double ts) override;
	virtual void OnRender() override;

private:
    void FixedUpdate(double fixedTimeStep);
	void LoadAssets();
    void PrintStats() const;
    void RenderGUI();

    static const unsigned int TPS = 60;
    bool msaa = true;

    glm::vec2 oldFbSize;
    double lastMouseX, lastMouseY, tickAccumulator, secondsAccumulator, timer;
    unsigned int frameCounter, tickCounter;
    size_t multisampledFramebuffer, postFramebuffer;

    Core::OrthographicCamera camera;

    std::vector<Platformer::Sprite> sprites;
    std::vector<Platformer::Layer> layers;
};