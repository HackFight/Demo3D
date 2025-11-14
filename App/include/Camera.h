#pragma once

// Engine
#include "Renderer/Camera.h"

//std
#include <cstdint>

namespace App
{
    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

        void SetSkybox(uint32_t skyboxVertexArray, uint32_t skyboxTexture, uint32_t skyboxShader);
        void RenderSkybox();

        Core::Camera coreCamera;

    private:
        uint32_t skyboxShader;
        uint32_t skyboxVertexArray;
        uint32_t skyboxTexture;
    };
}