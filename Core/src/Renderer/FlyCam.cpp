#include "Renderer/FlyCam.h"

namespace Core {
    FlyCam::FlyCam(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float speed, float sensitivity)
    : Camera3D(
        position,
        up,
        yaw,
        pitch
    ),
    movementSpeed(speed),
    mouseSensitivity(sensitivity)
    { }
    FlyCam::~FlyCam() {}

    void FlyCam::ProcessKeyboard(CameraMovement direction, double deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            position += forward * velocity;
        if (direction == BACKWARD)
            position -= forward * velocity;
        if (direction == LEFT)
            position -= glm::cross(forward, up) * velocity;
        if (direction == RIGHT)
            position += glm::cross(forward, up) * velocity;
    }
    void FlyCam::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }
}