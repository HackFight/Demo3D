#pragma once

// libs
#include "Renderer/Camera.h"

namespace Core {
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
    };
}