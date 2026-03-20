#include "Renderer/Camera3D.h"
#include <glm/ext/matrix_clip_space.hpp>

namespace Core {
    Camera3D::Camera3D(
        glm::vec3 position,
        glm::vec3 up,
        float yaw,
        float pitch,
        float aspectRatio,
        float fovy,
        float zNear,
        float zFar
    ) :
    Camera(
        position,
        up,
        yaw,
        pitch,
        aspectRatio
    ), fovy(fovy), zNear(zNear), zFar(zFar) {}
    Camera3D::~Camera3D() {}

    glm::mat4 Camera3D::getProjectionMatrix()
    {
        return glm::perspective(fovy, aspectRatio, zNear, zFar);
    }
}