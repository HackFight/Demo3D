#pragma once

//Engine
#include "Core/Layer.h"

//App
#include "Camera.h"

//std
#include <stdint.h>

class TestLayer : public Core::Layer
{
public:
    TestLayer();
	virtual ~TestLayer();

	virtual void OnUpdate(double ts) override;
	virtual void OnRender() override;

private:
    void ProcessInput(double ts);

    uint32_t vertexBuffer;
    uint32_t indexBuffer;
    uint32_t vertexArray;
    uint32_t shader;

    App::Camera camera;
    bool mouseDisabled = true;
    bool canPress = true;
    double lastX, lastY;

    double timeAcc = 0.0;
	int frameCounter = 0;
};