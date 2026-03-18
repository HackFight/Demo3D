#include "Renderer/Camera.h"

// libs
#include <glm/ext/matrix_transform.hpp>

namespace Core {   
    Camera::Camera(
            glm::vec3 position,
            glm::vec3 up,
            float yaw,
            float pitch,
            float aspectRatio,
            float fovy,
            float zNear,
            float zFar
        ) :
        position(position),
        up(up),
        yaw(yaw),
        pitch(pitch),
        aspectRatio(aspectRatio),
        fovy(fovy),
        zNear(zNear),
        zFar(zFar)
        {
            updateCameraVectors();
        }
    Camera::~Camera() { }

    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(position, position + forward, worldUp);
    }

    void Camera::lookAt(glm::vec3 point) {
        forward = glm::normalize(point - position);
        pitch = glm::degrees(asin(forward.y));
        yaw = glm::degrees(atan2(forward.z, forward.x));
        updateCameraVectors();
    }

    void Camera::updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward = glm::normalize(front);

        right = glm::normalize(glm::cross(forward, worldUp));
        up = glm::normalize(glm::cross(right, forward));
    }
}