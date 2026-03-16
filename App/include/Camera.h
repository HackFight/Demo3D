#pragma once

// Engine
#include "Core/Model.h"
#include "Renderer/FlyCam.h"

//std
#include <cstdint>

namespace App
{
    enum CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };
    
    class Camera : public Core::FlyCam
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float speed = 1.5f, float sensitivity = 0.1f);

        void SetSkybox(Core::Model model, uint32_t skyboxShader);
        void RenderSkybox();

    private:
        uint32_t skyboxShader;
		Core::Model skyboxModel;
    };
}