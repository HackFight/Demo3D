#include "Camera.h"

#include <glad/glad.h>

Camera::Camera()
{
    coreCamera = std::make_shared<Core::Camera>();
}
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
    coreCamera = std::make_shared<Core::Camera>(position, up, yaw, pitch);
}
Camera::~Camera() {}

void Camera::SetSkybox(std::shared_ptr<Core::VertexArray> vertexArray, std::shared_ptr<Core::Texture> texture, std::shared_ptr<Core::Shader> shader)
{
    skyboxVertexArray = vertexArray;
    skyboxTexture = texture;
    skyboxShader = shader;
}

void Camera::RenderSkybox()
{
    glDepthFunc(GL_LEQUAL);

    skyboxShader->Bind();
    skyboxShader->setmat4("projMat", coreCamera->getProjectionMatrix());
    skyboxShader->setmat4("viewMat", glm::mat4(glm::mat3(coreCamera->getViewMatrix())));
    skyboxShader->setInt("skybox", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture->GetRendererID());

    skyboxVertexArray->Bind();
    glDrawElements(GL_TRIANGLES, skyboxVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

    glDepthFunc(GL_LESS);
}