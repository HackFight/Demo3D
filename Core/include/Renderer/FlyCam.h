#pragma once

#include "Renderer/Camera3D.h"

namespace Core {
    class FlyCam : Camera3D {
        FlyCam(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float aspectRatio = 16.0f/9.0f, float fovy = glm::radians(45.0f), float zNear = 0.1f, float zFar = 100.0f);
        ~FlyCam();
    };
}