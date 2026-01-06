#pragma once

//Engine
#include "Core/Layer.h"

// libs
#include <glm/glm.hpp>

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

    static const unsigned int TPS = 60;

    glm::vec2 oldFbSize;
    double lastMouseX, lastMouseY, tickAccumulator, secondsAccumulator, timer;
    unsigned int frameCounter, tickCounter;
};