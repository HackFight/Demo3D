#include "Camera.h"

#include <glad/glad.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
    coreCamera = std::make_shared<Core::Camera>(position.x, position.y, position.z, up.x, up.y, up.z, yaw, pitch);
}

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

void Camera::SetCameraForLighting(glm::vec3 lightPos)
{
	// Save current position and orientation
	position = coreCamera->getPos();
	pitch = coreCamera->getPitch();
	yaw = coreCamera->getYaw();

	coreCamera->orthographic = true;
	coreCamera->setPos(lightPos + position);
    coreCamera->lookAt(position);
}

void Camera::SetCameraForScene()
{
	// Restore position and orientation
    coreCamera->orthographic = false;
	coreCamera->setPos(position);
    coreCamera->setPitch(pitch);
    coreCamera->setYaw(yaw);
}