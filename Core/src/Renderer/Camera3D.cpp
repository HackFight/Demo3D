#include "Renderer/Camera3D.h"
#include <glm/trigonometric.hpp>

namespace Core {
    Camera3D::Camera3D(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float aspectRatio, float fovy, float zNear, float zFar)
    : position(position), up(up), yaw(yaw), pitch(pitch), aspectRatio(aspectRatio), fovy(fovy), zNear(zNear), zFar(zFar)
    {
        updateCameraVectors();
    }
    Camera3D::~Camera3D() {}

    glm::mat4 Camera3D::getViewMatrix()
    {
        return glm::lookAt(position, position + forward, up);
    }
    glm::mat4 Camera3D::getProjectionMatrix()
    {
        return glm::perspective(fovy, aspectRatio, zNear, zFar);
    }

    void Camera3D::lookAt(glm::vec3 point)
    {
        forward = glm::normalize(point - position);
        pitch = glm::degrees(asin(forward.y));
        yaw = glm::degrees(atan2(forward.z, forward.x));
        updateCameraVectors();
    }

    void Camera3D::updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward = glm::normalize(front);

        right = glm::normalize(glm::cross(forward, worldUp));
        up    = glm::normalize(glm::cross(right, forward));
    }
}