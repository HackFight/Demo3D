#include "Renderer/Camera3D.h"
#include <glm/ext/matrix_clip_space.hpp>

namespace Core {
    Camera3D::Camera3D(
        glm::vec3 position,
        glm::vec3 up,
        float yaw,
        float pitch,
        float fovy,
        float aspectRatio,
        float zNear,
        float zFar
    ) : Camera(
        position,
        up,
        yaw,
        pitch,
        aspectRatio,
        zNear,
        zFar
    ), fovy(fovy) {}
    Camera3D::~Camera3D() {}

    glm::mat4 Camera3D::getProjectionMatrix() const {
        return glm::perspective(fovy, aspectRatio, zNear, zFar);
    }
}