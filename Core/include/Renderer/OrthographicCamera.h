#pragma once

// libs
#include "Renderer/Camera.h"

namespace Core {
    const float DEFAULT_CAMERA_HEIGHT = 10.0f;
    const glm::vec3 DEFAULT_ORTHO_POSITION = glm::vec3(0.0f, 0.0f, 1.0f);

    class OrthographicCamera : public Camera {
    public:
        OrthographicCamera(
            glm::vec3 position = DEFAULT_ORTHO_POSITION,
            glm::vec3 up = DEFAULT_UP,
            float yaw = DEFAULT_YAW,
            float pitch = DEFAULT_PITCH,
            float cameraHeight = DEFAULT_CAMERA_HEIGHT,
            float aspectRatio = DEFAULT_AR,
            float zNear = DEFAULT_ZNEAR,
            float zFar = DEFAULT_ZFAR
        );
        ~OrthographicCamera();

        glm::mat4 getProjectionMatrix();

        float cameraHeight = DEFAULT_CAMERA_HEIGHT;
    };
}