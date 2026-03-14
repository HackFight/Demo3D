#include "Camera.h"

// libs
#include <glad/glad.h>

//std
#include <cstdint>

// Engine
#include "Core/Application.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"

namespace App
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float speed, float sensitivity)
		: skyboxShader(0), skyboxModel(Core::Model()), movementSpeed(speed), mouseSensitivity(sensitivity)
    {
        glm::vec2 fbSize = Core::Application::Get().GetFramebufferSize();
        coreCamera = Core::Camera3D(position, up, yaw, pitch, fbSize.x/fbSize.y);
    }

    void Camera::SetSkybox(Core::Model model, uint32_t shader)
    {
        skyboxShader = shader;
        skyboxModel = model;
    }

    void Camera::RenderSkybox()
    {
        Core::RendererAPI::SetDepthFunc(GL_LEQUAL);

        Core::ShaderManager::setmat4(skyboxShader, "viewMat", glm::mat4(glm::mat3(coreCamera.getViewMatrix())));
        Core::ShaderManager::setmat4(skyboxShader, "projMat", coreCamera.getProjectionMatrix());

        Core::ShaderManager::setInt(skyboxShader, "skybox", 0);

        skyboxModel.Draw(skyboxShader);

        Core::RendererAPI::SetDepthFunc(GL_LESS);
    }

    void Camera::ProcessKeyboard(CameraMovement direction, double deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            coreCamera.position += coreCamera.forward * velocity;
        if (direction == BACKWARD)
            coreCamera.position -= coreCamera.forward * velocity;
        if (direction == LEFT)
            coreCamera.position -= glm::cross(coreCamera.forward, coreCamera.up) * velocity;
        if (direction == RIGHT)
            coreCamera.position += glm::cross(coreCamera.forward, coreCamera.up) * velocity;
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        coreCamera.yaw += xoffset;
        coreCamera.pitch += yoffset;

        if (constrainPitch)
        {
            if (coreCamera.pitch > 89.0f)
                coreCamera.pitch = 89.0f;
            if (coreCamera.pitch < -89.0f)
                coreCamera.pitch = -89.0f;
        }

        coreCamera.updateCameraVectors();
    }
}