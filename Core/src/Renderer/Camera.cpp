#include "Renderer/Camera.h"
#include "Core/Application.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"

#include <glad/glad.h>

namespace Core {

    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
        forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
    {
        position = position;
        worldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
        forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
    {
        position = glm::vec3(posX, posY, posZ);
        worldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(position, position + forward, up);
    }
    glm::mat4 Camera::getProjectionMatrix()
    {
        glm::vec2 viewportSize = Application::Get().GetFramebufferSize();
        return glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.1f, 100.0f);
    }

    glm::vec3 Camera::getPos()
    {
        return position;
    }

    void Camera::ProcessKeyboard(CameraMovement direction, double deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            position += forward * velocity;
        if (direction == BACKWARD)
            position -= forward * velocity;
        if (direction == LEFT)
            position -= glm::cross(forward, up) * velocity;
        if (direction == RIGHT)
            position += glm::cross(forward, up) * velocity;
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void Camera::updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        forward = glm::normalize(front);

        right = glm::normalize(glm::cross(forward, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, forward));
    }
}