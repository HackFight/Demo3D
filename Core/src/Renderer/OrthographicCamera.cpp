#include "Renderer/OrthographicCamera.h"
#include "Renderer/Camera.h"
#include <glm/ext/matrix_clip_space.hpp>

namespace Core {
    OrthographicCamera::OrthographicCamera(
            glm::vec3 position,
            glm::vec3 up,
            float yaw,
            float pitch,
            float cameraHeight,
            float aspectRatio,
            float zNear,
            float zFar
        ) : Camera(
            position,
            up,
            yaw,
            pitch,
            aspectRatio
        ), cameraHeight(cameraHeight) { }
    OrthographicCamera::~OrthographicCamera() {}
    
    glm::mat4 OrthographicCamera::getProjectionMatrix() {
        float cameraWidth = cameraHeight*aspectRatio;
        return glm::ortho(
            -cameraWidth/2.0f, cameraWidth/2.0f,
            -cameraHeight/2.0f, cameraHeight/2.0f,
            zNear, zFar
        );
    }
}