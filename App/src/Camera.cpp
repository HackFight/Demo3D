#include "Camera.h"

// libs
#include <glad/glad.h>

//std
#include <cstdint>

// Engine
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"
#include "RendererAPI/TextureManager.h"

namespace App
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
		: skyboxShader(0), skyboxModel(Core::Model())
    {
        coreCamera = Core::Camera(position.x, position.y, position.z, up.x, up.y, up.z, yaw, pitch);
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
}