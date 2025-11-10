#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

namespace Core {

    enum CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // Default camera values
    const float YAW         = -90.0f;
    const float PITCH       =  0.0f;
    const float SPEED       =  2.5f;
    const float SENSITIVITY =  0.1f;
    const float ZOOM        =  45.0f;

    class Camera
    {
    public:
        Camera(float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f, float yaw = -90.0f, float pitch = 0.0f);

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

		glm::vec3 getPos() { return position; }
		float getPitch() { return Pitch; }
		float getYaw() { return Yaw; }

		void setPos(glm::vec3 pos) { position = pos; updateCameraVectors(); }
		void setPitch(float pitch) { Pitch = pitch; updateCameraVectors(); }
		void setYaw(float yaw) { Yaw = yaw; updateCameraVectors(); }

        void lookAt(glm::vec3 point);

        void ProcessKeyboard(CameraMovement direction, double deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

        float MovementSpeed;
        float MouseSensitivity;
        bool orthographic = false;

    private:
        void updateCameraVectors();

        glm::vec3 position;

        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 forward;
        glm::vec3 up;
        glm::vec3 right;

        float Yaw;
        float Pitch;
    };
}