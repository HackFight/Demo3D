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
		: skyboxShader(0), skyboxVertexArray(0), skyboxTexture(0)
    {
        coreCamera = Core::Camera(position.x, position.y, position.z, up.x, up.y, up.z, yaw, pitch);
    }

    void Camera::SetSkybox(uint32_t vertexArray, uint32_t texture, uint32_t shader)
    {
        skyboxVertexArray = vertexArray;
        skyboxTexture = texture;
        skyboxShader = shader;
    }

    void Camera::RenderSkybox()
    {
        Core::RendererAPI::SetDepthFunc(GL_EQUAL);

        Core::ShaderManager::setmat4(skyboxShader, "projMat", coreCamera.getProjectionMatrix());
        Core::ShaderManager::setmat4(skyboxShader, "viewMat", glm::mat4(glm::mat3(coreCamera.getViewMatrix())));
        Core::ShaderManager::setInt(skyboxShader, "skybox", 0);

        Core::TextureManager::Bind(skyboxTexture, 0);

        Core::RendererAPI::DrawIndexed(skyboxVertexArray);

        Core::RendererAPI::SetDepthFunc(GL_LESS);
    }
}