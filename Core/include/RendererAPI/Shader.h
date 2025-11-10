#pragma once

// libs
#include <glm/glm.hpp>

// std
#include <stdint.h>
#include <string>

namespace Core {

    class Shader
    {
    public:
		Shader();
		Shader(const char* vertexPath, const char* fragmentPath);
		~Shader();

		void Bind() const;
        static void Unbind();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void set2f(const std::string& name, float v1, float v2) const;
		void set2f(const std::string& name, glm::vec2 v) const;
		void set3f(const std::string& name, float v1, float v2, float v3) const;
		void set3f(const std::string& name, glm::vec3 v) const;
		void setmat4(const std::string& name, glm::mat4 mat) const;
	
	private:
		uint32_t m_RendererID;
    };
}