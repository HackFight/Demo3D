#include "Camera.h"

// libs
#include <glad/glad.h>

// Engine
#include "Renderer/FlyCam.h"
#include "RendererAPI/RendererAPI.h"
#include "RendererAPI/ShaderManager.h"

namespace App
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float speed, float sensitivity)
		: skyboxShader(0), skyboxModel(Core::Model()), Core::FlyCam(position, up, yaw, pitch, speed, sensitivity)
    { }

    void Camera::SetSkybox(Core::Model model, size_t shader)
    {
        skyboxShader = shader;
        skyboxModel = model;
    }

    void Camera::RenderSkybox() const
    {
        Core::RendererAPI::SetDepthFunc(GL_LEQUAL);

        Core::ShaderManager::setmat4(skyboxShader, "viewMat", glm::mat4(glm::mat3(getViewMatrix())));
        Core::ShaderManager::setmat4(skyboxShader, "projMat", getProjectionMatrix());

        Core::ShaderManager::setInt(skyboxShader, "skybox", 0);

        skyboxModel.Draw(skyboxShader);

        Core::RendererAPI::SetDepthFunc(GL_LESS);
    }
}