#pragma once

#include "RendererAPI/Shader.h"
#include "RendererAPI/Texture.h"
#include "RendererAPI/VertexArray.h"
#include <Renderer/Camera.h>

namespace App
{
    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

        void SetSkybox(Core::VertexArray skyboxVertexArray, Core::Texture skyboxTexture, Core::Shader skyboxShader);
        void RenderSkybox();

        Core::Camera coreCamera;

    private:
        Core::Shader skyboxShader;
        Core::VertexArray skyboxVertexArray;
        Core::Texture skyboxTexture;
    };
}