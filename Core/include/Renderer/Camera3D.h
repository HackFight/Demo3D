#pragma once

// libs
#include "Renderer/Camera.h"

namespace Core {
    const float DEFAULT_FOVY = glm::radians(45.0f);
    const float DEFAULT_ZNEAR = 0.1f;
    const float DEFAULT_ZFAR = 100.0f;

    class Camera3D : public Camera {
    public:
        Camera3D(
            glm::vec3 position = DEFAULT_POSITION,
            glm::vec3 up = DEFAULT_UP,
            float yaw = DEFAULT_YAW,
            float pitch = DEFAULT_PITCH,
            float aspectRatio = DEFAULT_AR,
            float fovy = DEFAULT_FOVY,
            float zNear = DEFAULT_ZNEAR,
            float zFar = DEFAULT_ZFAR
        );
        ~Camera3D();

        glm::mat4 getProjectionMatrix();

        float fovy, zNear, zFar;
    };
}