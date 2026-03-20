#pragma once

// libs
#include <glm/glm.hpp>

namespace Core {
    const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f);
    const glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    const float DEFAULT_YAW = -90.0f;
    const float DEFAULT_PITCH = 0.0f;
    const float DEFAULT_AR = 16.0f/9.0f;

    class Camera {        
    public:
        Camera(
            glm::vec3 position = DEFAULT_POSITION,
            glm::vec3 up = DEFAULT_UP,
            float yaw = DEFAULT_YAW,
            float pitch = DEFAULT_PITCH,
            float aspectRatio = DEFAULT_AR
        );
        ~Camera();

        glm::mat4 getViewMatrix();
        virtual glm::mat4 getProjectionMatrix() = 0;
        void lookAt(glm::vec3 point);
        void updateCameraVectors();

        glm::vec3 position, forward, up, right, worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        float yaw, pitch, aspectRatio;
    };
}