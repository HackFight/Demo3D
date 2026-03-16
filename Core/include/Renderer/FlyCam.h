#pragma once

#include "Renderer/Camera3D.h"

namespace Core {

    enum CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class FlyCam : public Camera3D {
    public:
        FlyCam(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float speed = 1.5f, float sensitivity = 0.1f);
        ~FlyCam();

        void ProcessKeyboard(CameraMovement direction, double deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

        float movementSpeed, mouseSensitivity;
    };
}