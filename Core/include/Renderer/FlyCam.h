#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Camera3D.h"

namespace Core {

    const float DEFAULT_SPEED = 1.5f;
    const float DEFAULT_SENSITIVITY = 0.1f;

    enum CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class FlyCam : public Camera3D {
    public:
        FlyCam(
            glm::vec3 position = DEFAULT_POSITION,
            glm::vec3 up = DEFAULT_UP,
            float yaw = DEFAULT_YAW,
            float pitch = DEFAULT_PITCH,
            float speed = DEFAULT_SPEED,
            float sensitivity = DEFAULT_SENSITIVITY
        );
        ~FlyCam();

        void ProcessKeyboard(CameraMovement direction, double deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

        float movementSpeed = DEFAULT_SPEED;
        float mouseSensitivity = DEFAULT_SENSITIVITY;
    };
}