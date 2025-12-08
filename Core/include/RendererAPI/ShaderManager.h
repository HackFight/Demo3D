#pragma once

//libs
#include <glad/glad.h>
#include <glm/glm.hpp>

//std
#include <stdint.h>
#include <string>
#include <vector>

namespace Core
{
    class ShaderManager
    {
    public:
        static uint32_t CreateShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    
        static void Bind(uint32_t shader);
        static void Unbind();

        static void setBool(uint32_t shader, const std::string& name, bool value);
		static void setInt(uint32_t shader, const std::string& name, int value);
		static void setFloat(uint32_t shader, const std::string& name, float value);
		static void set2f(uint32_t shader, const std::string& name, float v1, float v2);
		static void set2f(uint32_t shader, const std::string& name, glm::vec2 v);
		static void set3f(uint32_t shader, const std::string& name, float v1, float v2, float v3);
		static void set3f(uint32_t shader, const std::string& name, glm::vec3 v);
		static void setmat4(uint32_t shader, const std::string& name, glm::mat4 mat);

        static void ReleaseAll();

    private:
        static std::vector<GLuint> shaders;
    };
}