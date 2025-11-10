#include "Camera.h"

// libs
#include <glad/glad.h>

// Engine
#include "RendererAPI/RendererAPI.h"

namespace App
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    {
        coreCamera = Core::Camera(position.x, position.y, position.z, up.x, up.y, up.z, yaw, pitch);
    }

    void Camera::SetSkybox(Core::VertexArray vertexArray, Core::Texture texture, Core::Shader shader)
    {
        skyboxVertexArray = vertexArray;
        skyboxTexture = texture;
        skyboxShader = std::move(shader);
    }

    void Camera::RenderSkybox()
    {
        Core::RendererAPI::SetDepthFunc(GL_EQUAL);

        skyboxShader.Bind();
        skyboxShader.setmat4("projMat", coreCamera.getProjectionMatrix());
        skyboxShader.setmat4("viewMat", glm::mat4(glm::mat3(coreCamera.getViewMatrix())));
        skyboxShader.setInt("skybox", 0);

        skyboxTexture.Bind(0);

        Core::RendererAPI::DrawIndexed(skyboxVertexArray);

        Core::RendererAPI::SetDepthFunc(GL_LESS);
    }
}