#pragma once

// libs
#include "Renderer/Camera.h"

namespace Core {
    class OrthographicCamera : public Camera {
    public:
        OrthographicCamera(
            glm::vec3 position = DEFAULT_POSITION,
            glm::vec3 up = DEFAULT_UP,
            float yaw = DEFAULT_YAW,
            float pitch = DEFAULT_PITCH,
            float aspectRatio = DEFAULT_AR
        );
        ~OrthographicCamera();

        glm::mat4 getProjectionMatrix();
    };
}