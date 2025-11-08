#pragma once

#include "RendererAPI/Texture.h"
#include "RendererAPI/VertexArray.h"
#include <Renderer/Camera.h>
#include <memory>

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    void SetSkybox(std::shared_ptr<Core::VertexArray> skyboxVertexArray, std::shared_ptr<Core::Texture> skyboxTexture, std::shared_ptr<Core::Shader> skyboxShader);
    void RenderSkybox();
	void SetCameraForLighting(glm::vec3 lightPos);
	void SetCameraForScene();

    std::shared_ptr<Core::Camera> coreCamera;

private:
    std::shared_ptr<Core::Shader> skyboxShader;
    std::shared_ptr<Core::VertexArray> skyboxVertexArray;
    std::shared_ptr<Core::Texture> skyboxTexture;

    glm::vec3 position;
	float pitch; float yaw;
};