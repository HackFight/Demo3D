#pragma once

// libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Core {
    class Camera3D {
    public:
        Camera3D(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float aspectRatio = 16.0f/9.0f, float fovy = glm::radians(45.0f), float zNear = 0.1f, float zFar = 100.0f);
        ~Camera3D();

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

        void lookAt(glm::vec3 point);

        void updateCameraVectors();

        glm::vec3 position;
        float aspectRatio, fovy, zNear, zFar;

        float yaw, pitch;
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 forward, up, right;
    };
}