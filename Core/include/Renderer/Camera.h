#pragma once

#include <glm/glm.hpp>

namespace Core {

    class Camera
    {
    public:
        Camera();
        ~Camera();

        glm::mat4 GetProjectionMatrix();
        glm::mat4 GetViewMatrix();

        void SetPos(glm::vec3 pos);
    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
    };
}